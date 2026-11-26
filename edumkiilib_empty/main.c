/*
 * main.c
 * Inicializacion de perifericos y flujo de turnos de la partida.
 */

// General libraries
#include <msp430.h>
#include <driverlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

// GRLIB
#include <grlib.h>

// HAL headers
#include <hal/hal_lcd.h>
#include <hal/hal_i2c.h>
#include <hal/hal_digital_input.h>
#include <hal/hal_digital_output.h>
#include <hal/hal_uart.h>
#include <hal/hal_uart_cc.h>
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

// Own libraries
#include <own_libraries/tablero.h>
#include <own_libraries/interfaz_jugar.h>
#include <own_libraries/vista_tablero.h>
#include <own_libraries/enlace_juego.h>
#include <own_libraries/pantalla_estadisticas.h>
#include <own_libraries/Pantalla_drw.h>
#include <own_libraries/Place_ship.h>
#include <own_libraries/estilo_militar.h>
#include <own_libraries/menu_juego.h>
#include <own_libraries/partida_interfaz.h>
#include <own_libraries/protocolo_partida.h>
#include <own_libraries/tablero_juego.h>
#include <own_libraries/colocacion_flota.h>

/* Para una partida entre dos MSP430: compilar uno con 0 y el otro con 1. */
#define JUGADOR_LOCAL 1

// Global variables
Graphics_Context g_sContext;

// Un tablero por jugador: tableros[jugador][fila y][columna x]
static uint8_t tableros[2][FILAS_TABLERO][COLS_TABLERO];

void main(void)
{
    uint8_t jugador = JUGADOR_LOCAL;
    uint8_t fila, columna, casilla, resultado;
    uint8_t secuencia = 0;
    protocolo_partida_estado_t estado_protocolo = {
        false, 0, 0, RESULTADO_INVALIDO
    };
    bool turno_local;
    char linea[20];

    // Stop WDT
    WDT_A_hold(WDT_A_BASE);

    // Initialize clocks
    FRAMCtl_A_configureWaitStateControl(FRAMCTL_A_ACCESS_TIME_CYCLES_1);
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    // Initialize HAL GPIOs
    HAL_LCD_init_gpio();
    HAL_I2C_init_gpio();
    HAL_DIGOUT_init_gpio();
    HAL_UART_init_gpio();
    HAL_UART_CC_init_gpio();
    HAL_ADC_init_gpio();
    HAL_PWM_init_gpio();        // last on purpose: wins P3.4-P3.7 muxing
    HAL_DIGIN_init_gpio();      //Se retrasa a posta para que el boton del joystick gane el control de p3.1

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Configure HAL modules
    HAL_LCD_config();
    HAL_I2C_config();
    HAL_DIGOUT_config();

    HAL_UART_config();
    HAL_UART_CC_config();
    HAL_ADC_config();
    HAL_PWM_config();
    HAL_DIGIN_config();

    // Boton P5.6 de la LaunchPad (ver mi tablero), en vista_tablero.c
    vista_init();
    MENU_init();

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
    __delay_cycles(160000);

    if (jugador > 1)
        jugador = 0;
    memset(tableros, AGUA, sizeof(tableros));
    ESTADISTICAS_reiniciar();

    COLOCACION_colocar_flota(tableros[jugador], jugador);

    PARTIDA_pantalla_mensaje("Listo para jugar", "Jugador 2 pulsa S1", "Despues jugador 1");
    PARTIDA_esperar_S1();
    if (jugador == 0)
        PARTIDA_pantalla_mensaje("Buscando jugador 2", "Esperando enlace", "No apagues la radio");
    else
        PARTIDA_pantalla_mensaje("Esperando jugador 1", "Preparando enlace", "No apagues la radio");
    PROTOCOLO_iniciar(jugador);
    turno_local = (jugador == 0); /* jugador 1 empieza */

    while (1)
    {
        if (turno_local)
        {
            PARTIDA_esperar_inicio_turno();
            sprintf(linea, "Turno jugador %d", jugador + 1);
            PARTIDA_pantalla_mensaje(linea, "Elige una casilla", "P5.6: mi tablero");

            if (!seleccionar_disparo(tableros[1 - jugador], tableros[jugador],
                                     &fila, &columna))
                continue;

            casilla = fila * COLS_TABLERO + columna;
            PARTIDA_pantalla_mensaje("Disparo enviado", "Esperando respuesta", "No apagues la radio");
            resultado = PROTOCOLO_enviar_disparo_y_esperar(jugador, secuencia, casilla,
                                                           tableros[jugador],
                                                           &estado_protocolo);
            secuencia = (secuencia + 1) % 100;

            if (resultado == RESULTADO_INVALIDO)
                continue;

            ESTADISTICAS_registrar_resultado(jugador, resultado);
            TABLERO_actualizar_objetivo(tableros[1 - jugador], casilla, resultado);
            if (resultado == RES_AGUA)
                pantalla_militar("Agua", "", "Turno del rival", ESTILO_AGUA);
            else if (resultado <= NUM_BARCOS)
                pantalla_militar("Tocado", "", "Turno del rival", ESTILO_TOCADO);
            else
                pantalla_militar("Hundido", "", "Turno del rival", ESTILO_HUNDIDO);
            BUZ_wait_ms(PARTIDA_MS_PANTALLA_RESULTADO);

            if (TABLERO_flota_rival_hundida(tableros[1 - jugador]))
            {
                pantalla_militar("Has ganado", "Flota rival hundida", "P5.5: menu", ESTILO_GANADO);
                while (1)
                {
                    if (MENU_boton_pulsado())
                    {
                        MENU_abrir();
                        pantalla_militar("Has ganado", "Flota rival hundida",
                                         "P5.5: menu", ESTILO_GANADO);
                    }
                    PROTOCOLO_confirmar_resultado_ganador(jugador);
                    BUZ_wait_ms(1);
                }
            }
            turno_local = false;
        }
        else
        {
            PARTIDA_pantalla_mensaje("Turno del rival", "Esperando disparo", "No apagues la radio");
            casilla = 0;
            resultado = RESULTADO_INVALIDO;
            PROTOCOLO_esperar_disparo_rival(jugador, tableros[jugador],
                                            &estado_protocolo, &casilla,
                                            &resultado);
            ESTADISTICAS_registrar_resultado((uint8_t)(1 - jugador), resultado);
            sprintf(linea, "Disparo en %c%d", 'A' + casilla / COLS_TABLERO,
                    casilla % COLS_TABLERO + 1);
            if (resultado == RES_AGUA)
                pantalla_militar(linea, "Agua", "Tu turno despues", ESTILO_AGUA);
            else
                pantalla_militar(linea, "Te han tocado", "Tu turno despues", ESTILO_TOCADO);
            BUZ_wait_ms(PARTIDA_MS_PANTALLA_RESULTADO);

            if (TABLERO_flota_hundida(tableros[jugador]))
            {
                pantalla_militar("Has perdido", "Tu flota hundida", "P5.5: menu", ESTILO_PERDIDO);
                while (1)
                {
                    if (MENU_boton_pulsado())
                    {
                        MENU_abrir();
                        pantalla_militar("Has perdido", "Tu flota hundida", "P5.5: menu", ESTILO_PERDIDO);
                    }
                    BUZ_wait_ms(1);
                }
            }
            turno_local = true;
        }
    }
}
