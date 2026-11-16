/*
 * Empty C template with edumkiilib - MSP430FR5994 + EDUMKII BoosterPack
 *
 * Complete, working initialization of every HAL and BoosterPack device
 * (with the unified hal/adc/all and hal/pwm/all variants, so joystick +
 * accelerometer and RGB + buzzer can all be used together), plus an empty
 * main loop for your application.
 *
 * Note: the digital output HAL (LaunchPad LEDs + RGB) shares pins
 * P3.4-P3.6 with the PWM HAL. Its init runs BEFORE the PWM one, so the
 * PWM muxing wins on those pins: use LED_set_rgb() (PWM) for the RGB LED
 * and HAL_DIGOUT_set_LED1/2() for the LaunchPad LEDs only.
 */

// General libraries
#include <msp430.h>
#include <driverlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// GRLIB
#include <grlib.h>

// HAL headers
#include <hal/hal_lcd.h>
#include <hal/hal_i2c.h>
#include <hal/hal_digital_input.h>
#include <hal/hal_digital_output.h>
#include <hal/hal_uart.h>
#include <hal/hal_adc.h>
#include <hal/hal_pwm.h>

// Driver headers
#include <drivers/crystalfontz.h>
#include <drivers/joystick.h>
#include <drivers/kxtc9_2050.h>
#include <drivers/switch.h>
#include <drivers/tmp006.h>
#include <drivers/opt3001.h>
#include <drivers/led.h>
#include <drivers/buzzer.h>
#include <drivers/cli.h>

// Own libraries (Place_ship y pantalla_marcador NO se incluyen todavia:
// Place_ship no esta terminado y pantalla_marcador va en la otra placa)
#include <own_libraries/tablero.h>
#include <own_libraries/interfaz_jugar.h>
#include <own_libraries/vista_tablero.h>
#include <own_libraries/enlace_marcador.h>

// Global variables
Graphics_Context g_sContext;

// Un tablero por jugador: tableros[jugador][fila y][columna x]
static uint8_t tableros[2][FILAS_TABLERO][COLS_TABLERO];


/* ------------------------------------------------------------------
 * Tableros de prueba (TEMPORAL, hasta que Place_ship este terminado)
 * ------------------------------------------------------------------ */
static void colocar_barco(uint8_t t[FILAS_TABLERO][COLS_TABLERO],
                          uint8_t id, uint8_t longitud,
                          uint8_t fila, uint8_t col, bool horizontal)
{
    uint8_t i;
    for (i = 0; i < longitud; i++)
    {
        if (horizontal) t[fila][col + i] = id;
        else            t[fila + i][col] = id;
    }
}

static void tableros_de_prueba(void)
{
    memset(tableros, AGUA, sizeof(tableros));

    // Jugador 1 (fila A..J = 0..9, columna 1..10 = 0..9)
    colocar_barco(tableros[0], BARCO_2,  2, 0, 0, true);   // A1-A2
    colocar_barco(tableros[0], BARCO_3A, 3, 2, 2, false);  // C3-E3
    colocar_barco(tableros[0], BARCO_3B, 3, 1, 5, true);   // B6-B8
    colocar_barco(tableros[0], BARCO_4,  4, 6, 1, true);   // G2-G5
    colocar_barco(tableros[0], BARCO_5,  5, 4, 9, false);  // E10-I10

    // Jugador 2
    colocar_barco(tableros[1], BARCO_2,  2, 9, 8, true);   // J9-J10
    colocar_barco(tableros[1], BARCO_3A, 3, 0, 0, false);  // A1-C1
    colocar_barco(tableros[1], BARCO_3B, 3, 4, 4, true);   // E5-E7
    colocar_barco(tableros[1], BARCO_4,  4, 1, 7, false);  // B8-E8
    colocar_barco(tableros[1], BARCO_5,  5, 8, 1, true);   // I2-I6
}

/* ------------------------------------------------------------------
 * Utilidades de pantalla
 * ------------------------------------------------------------------ */
static void pantalla_mensaje(const char *l1, const char *l2, const char *l3)
{
    Graphics_setFont(&g_sContext, &g_sFontCm12);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_clearDisplay(&g_sContext);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)l1, AUTO_STRING_LENGTH, 64, 35, false);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)l2, AUTO_STRING_LENGTH, 64, 60, false);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)l3, AUTO_STRING_LENGTH, 64, 95, false);
}

static void esperar_S1(void)
{
    while (!SWITCH_get_edumkii_S1());
}

/* ------------------------------------------------------------------
 * Compara el tablero antes y despues de disparar() para saber que ha
 * pasado (disparar() no lo devuelve). Asi no hace falta tocar su codigo.
 * Devuelve false si no ha cambiado ninguna casilla.
 * ------------------------------------------------------------------ */
static bool analizar_disparo(uint8_t antes[FILAS_TABLERO][COLS_TABLERO],
                             uint8_t despues[FILAS_TABLERO][COLS_TABLERO],
                             uint8_t *resultado, uint8_t *barco)
{
    uint8_t x, y, a;

    for (y = 0; y < FILAS_TABLERO; y++)
    {
        for (x = 0; x < COLS_TABLERO; x++)
        {
            a = antes[y][x];
            // La casilla disparada es la unica que era agua o barco intacto
            // y ha cambiado (las demas que cambian, al hundir, eran tocadas)
            if (a != despues[y][x] && (a == AGUA || ES_BARCO_INTACTO(a)))
            {
                if (a == AGUA)
                {
                    *resultado = RES_AGUA;
                    *barco = BARCO_NINGUNO;
                }
                else
                {
                    *resultado = (despues[y][x] == HUNDIDO) ? RES_HUNDIDO : RES_TOCADO;
                    *barco = a - 1;     // indice 0..4 del marcador
                }
                return true;
            }
        }
    }
    return false;
}

static bool flota_hundida(uint8_t t[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t x, y;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (ES_BARCO_INTACTO(t[y][x]))
                return false;
    return true;
}

static void Clocks_init()
{
    // Increase FRAM access wait time. Needed to work properly at 16MHz
    FRAMCtl_A_configureWaitStateControl(FRAMCTL_A_ACCESS_TIME_CYCLES_1);

    // Set DCO at 16 MHz
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

void main(void)
{
    // Stop WDT
    WDT_A_hold(WDT_A_BASE);

    // Initialize clocks
    Clocks_init();

    // Initialize HAL GPIOs
    HAL_LCD_init_gpio();
    HAL_I2C_init_gpio();
    HAL_DIGIN_init_gpio();
    HAL_DIGOUT_init_gpio();
    HAL_UART_init_gpio();
    HAL_ADC_init_gpio();
    HAL_PWM_init_gpio();        // last on purpose: wins P3.4-P3.7 muxing

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Configure HAL modules
    HAL_LCD_config();
    HAL_I2C_config();
    HAL_DIGIN_config();
    HAL_DIGOUT_config();
    HAL_UART_config();
    HAL_ADC_config();
    HAL_PWM_config();

    // Boton P5.6 de la LaunchPad (ver mi tablero), en vista_tablero.c
    vista_init();

    // Enable global interrupts
    __bis_SR_register(GIE);

    // Initialize and configure external devices
    CRYSTALFONTZ_init();
    CRYSTALFONTZ_set_orientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext, &g_sCRYSTALFONTZ);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontCm12);
    Graphics_clearDisplay(&g_sContext);

    TMP006_config();
    OPT3001_config();

    // El joystick (ADC) se refresca solo en modo continuo
    HAL_ADC_trigger_continuous_conversion();

    while (1)
    {
        /* ---------------- Nueva partida ---------------- */
        uint8_t turno = 0;                       // 0 = PLAYER 1, 1 = PLAYER 2
        uint8_t copia[FILAS_TABLERO][COLS_TABLERO];
        uint8_t resultado, barco;
        char linea[20];

        tableros_de_prueba();                    // TODO: sustituir por Place_ship
        MARCADOR_enviar_inicio();

        pantalla_mensaje("HUNDIR LA FLOTA", "", "S1: empezar");
        esperar_S1();

        while (1)
        {
            uint8_t rival = 1 - turno;

            MARCADOR_enviar_turno(turno);

            // Pantalla de paso (para no ver el tablero del otro)
            sprintf(linea, "Turno jugador %d", turno + 1);
            pantalla_mensaje(linea, "S1: disparar", "P5.6: mi tablero");
            esperar_S1();

            // 1) Disparar al rival (interfaz_jugar). Dentro, P5.6 ensena
            //    mi tablero (vista_tablero) y otra pulsacion vuelve.
            memcpy(copia, tableros[rival], sizeof(copia));
            Graphics_setFont(&g_sContext, &g_sFontCm12);
            disparar(tableros[rival], tableros[turno]);

            // 2) Datos para el marcador
            if (analizar_disparo(copia, tableros[rival], &resultado, &barco))
            {
                MARCADOR_enviar_disparo(turno, resultado, barco);
            }

            // 3) Fin de partida?
            if (flota_hundida(tableros[rival]))
            {
                MARCADOR_enviar_fin(turno);
                sprintf(linea, "Gana jugador %d", turno + 1);
                pantalla_mensaje(linea, "Flota hundida", "S1: nueva partida");
                esperar_S1();
                break;
            }

            turno = rival;
        }
    }
}
