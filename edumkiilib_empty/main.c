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


// Own libraries (Place_ship y pantalla_marcador NO se incluyen todavia:
// Place_ship no esta terminado y pantalla_marcador va en la otra placa)
#include <own_libraries/tablero.h>
#include <own_libraries/interfaz_jugar.h>
#include <own_libraries/vista_tablero.h>
#include <own_libraries/enlace_marcador.h>
#include <own_libraries/pantalla_marcador.h>

//Own libraries
#include <own_libraries/Pantalla_drw.h>
#include <own_libraries/Place_ship.h>
#include <own_libraries/estilo_militar.h>

/* Compilar con 0 en la placa de juego y con 1 en la placa marcador. */
#define MODO_MARCADOR 0

/* Para una partida entre dos MSP430: compilar uno con 0 y el otro con 1. */
#ifndef JUGADOR_LOCAL
#define JUGADOR_LOCAL 0
#endif

/* 1 = tableros fijos (para probar rapido la radio sin colocar barcos),
 * 0 = cada jugador coloca sus barcos con Place_ship. */
#define USAR_TABLEROS_DE_PRUEBA 0

/* Tiempo que se ve la pantalla de Agua / Tocado / Hundido */
#define MS_PANTALLA_RESULTADO 2500


// Global variables
Graphics_Context g_sContext;

// Un tablero por jugador: tableros[jugador][fila y][columna x]
static uint8_t tableros[2][FILAS_TABLERO][COLS_TABLERO];
static bool musica_juego_activa;


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

static void tablero_de_prueba(uint8_t t[FILAS_TABLERO][COLS_TABLERO], uint8_t jugador)
{
    memset(t, AGUA, FILAS_TABLERO * COLS_TABLERO);

    if (jugador == 0)
    {
        colocar_barco(t, BARCO_2,  2, 0, 0, true);
        colocar_barco(t, BARCO_3A, 3, 2, 2, false);
        colocar_barco(t, BARCO_3B, 3, 1, 5, true);
        colocar_barco(t, BARCO_4,  4, 6, 1, true);
        colocar_barco(t, BARCO_5,  5, 4, 9, false);
    }
    else
    {
        colocar_barco(t, BARCO_2,  2, 9, 8, true);
        colocar_barco(t, BARCO_3A, 3, 0, 0, false);
        colocar_barco(t, BARCO_3B, 3, 4, 4, true);
        colocar_barco(t, BARCO_4,  4, 1, 7, false);
        colocar_barco(t, BARCO_5,  5, 8, 1, true);
    }
}

/* ------------------------------------------------------------------
 * Utilidades de pantalla
 * ------------------------------------------------------------------ */
static void esperar_ms(uint16_t ms);

/* Pantallas intermedias con estilo militar (ver estilo_militar.c) */
static void pantalla_mensaje(const char *l1, const char *l2, const char *l3)
{
    pantalla_militar(l1, l2, l3, ESTILO_NORMAL);
}

static void esperar_S1(void)
{
    while (!SWITCH_get_edumkii_S1())
        esperar_ms(1);
}

bool MENU_boton_pulsado(void)
{
    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) != GPIO_INPUT_PIN_LOW)
        return false;

    esperar_ms(20);
    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) != GPIO_INPUT_PIN_LOW)
        return false;

    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
        esperar_ms(1);
    esperar_ms(20);
    return true;
}

static void mostrar_estadisticas_menu(void)
{
    marcador_mostrar_estadisticas();
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"S1: volver al menu",
                                AUTO_STRING_LENGTH, 64, 118, TRANSPARENT_TEXT);
    while (!SWITCH_get_edumkii_S1())
        esperar_ms(1);
    while (SWITCH_get_edumkii_S1())
        esperar_ms(1);
}

void MENU_abrir(void)
{
    uint8_t opcion = 0, anterior;
    float y;

    menu_militar_dibujar(opcion, musica_juego_activa);
    while (1)
    {
        if (MENU_boton_pulsado())
            return;

        y = JOYSTICK_get_y();
        if (y > 30 || y < -30)
        {
            anterior = opcion;
            if (y > 30)
                opcion = (opcion + MENU_NUM_OPCIONES - 1) % MENU_NUM_OPCIONES;
            else
                opcion = (opcion + 1) % MENU_NUM_OPCIONES;

            /* Solo se redibujan las dos placas que cambian */
            menu_militar_placa(anterior, false, musica_juego_activa);
            menu_militar_placa(opcion, true, musica_juego_activa);
            esperar_ms(200);
        }

        if (SWITCH_get_edumkii_S1())
        {
            while (SWITCH_get_edumkii_S1())
                esperar_ms(1);

            if (opcion == 0)
            {
                mostrar_estadisticas_menu();
                menu_militar_dibujar(opcion, musica_juego_activa);
            }
            else if (opcion == 1)
            {
                musica_juego_activa = !musica_juego_activa;
                BUZ_music_set(musica_juego_activa);
                menu_militar_placa(opcion, true, musica_juego_activa);
            }
            else
                return;
        }
        esperar_ms(1);
    }
}

static void esperar_inicio_turno(void)
{
    pantalla_mensaje("Tu turno", "S1: elegir disparo", "P5.5: menu");
    while (!SWITCH_get_edumkii_S1())
    {
        if (MENU_boton_pulsado())
        {
            MENU_abrir();
            pantalla_mensaje("Tu turno", "S1: elegir disparo", "P5.5: menu");
        }
        esperar_ms(1);
    }
    while (SWITCH_get_edumkii_S1())
        esperar_ms(1);
}

static void registrar_estadistica_disparo(uint8_t jugador, uint8_t resultado)
{
    if (resultado == RES_AGUA)
        marcador_registrar_agua(jugador);
    else if (resultado >= 1 && resultado <= 2 * NUM_BARCOS)
        marcador_registrar_tocado(jugador,
                (resultado > NUM_BARCOS ? resultado - NUM_BARCOS : resultado) - 1);
}

/* ------------------------------------------------------------------
 * Colocacion de barcos con Place_ship (parte de Jon).
 * Antes sale una pantalla de paso para que el otro jugador no mire.
 * Cada place_ship_X guarda en el tablero el id de tablero.h:
 *   place_ship_2 -> 1, _31 -> 2, _32 -> 3, _4 -> 4, _5 -> 5
 * ------------------------------------------------------------------ */
static void colocar_flota(uint8_t t[FILAS_TABLERO][COLS_TABLERO], uint8_t jugador)
{
    char linea[20];

    memset(t, AGUA, FILAS_TABLERO * COLS_TABLERO);

    sprintf(linea, "Jugador %d", jugador + 1);
    pantalla_mensaje(linea, "Coloca tus barcos", "S1: empezar");
    esperar_S1();

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(&g_sContext);

    place_ship_2 (&g_sContext, t);
    place_ship_31(&g_sContext, t);
    place_ship_32(&g_sContext, t);
    place_ship_4 (&g_sContext, t);
    place_ship_5 (&g_sContext, t);
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

static void esperar_ms(uint16_t ms)
{
    while (ms--)
    {
        BUZ_music_tick(1);
        __delay_cycles(16000);
    }
}

static bool recibir_trama_timeout(uint8_t trama[5], uint16_t timeout_ms)
{
    while (timeout_ms--)
    {
        if (ENLACE_recibir_trama(trama))
            return true;
        esperar_ms(1);
    }
    return false;
}

static void iniciar_enlace_juego(uint8_t jugador)
{
    uint8_t trama[5];

    pantalla_mensaje("Listo para jugar", "Jugador 2 pulsa S1", "Despues jugador 1");
    esperar_S1();

    if (jugador == 0)
    {
        pantalla_mensaje("Buscando jugador 2", "Esperando enlace", "No apagues la radio");
        while (1)
        {
            if (recibir_trama_timeout(trama, 1000) &&
                trama[1] == CMD_ACK_LISTO && trama[2] == 1)
                return;

            ENLACE_enviar_listo(0);
        }
    }

    pantalla_mensaje("Esperando jugador 1", "Preparando enlace", "No apagues la radio");
    while (1)
    {
        if (recibir_trama_timeout(trama, 1000) &&
            trama[1] == CMD_LISTO && trama[2] == 0)
        {
            ENLACE_enviar_ack_listo(1);
            return;
        }
    }
}

/* Convierte el disparo recibido contra la unica flota almacenada en esta placa. */
static uint8_t resolver_disparo(uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                uint8_t casilla)
{
    uint8_t fila = casilla / COLS_TABLERO;
    uint8_t col = casilla % COLS_TABLERO;
    uint8_t valor = flota[fila][col];
    uint8_t x, y, restantes = 0;

    if (valor == AGUA)
    {
        flota[fila][col] = BOMBA_FALLIDA;
        return RES_AGUA;
    }

    if (!ES_BARCO_INTACTO(valor))
        return RESULTADO_INVALIDO;

    flota[fila][col] = valor + OFFSET_TOCADO;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (flota[y][x] == valor)
                restantes++;

    return restantes ? valor : (uint8_t)(valor + OFFSET_TOCADO);
}

static uint8_t responder_disparo(uint8_t trama[5],
                                 uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                 bool *hay_anterior, uint8_t *seq_anterior,
                                 uint8_t *casilla_anterior, uint8_t *resultado_anterior)
{
    uint8_t seq = trama[3];
    uint8_t casilla = trama[4];
    uint8_t resultado;

    if (*hay_anterior && seq == *seq_anterior && casilla == *casilla_anterior)
        resultado = *resultado_anterior;       /* reenvio de una peticion repetida */
    else if (casilla < FILAS_TABLERO * COLS_TABLERO)
    {
        resultado = resolver_disparo(flota, casilla);
        if (resultado != RESULTADO_INVALIDO)
        {
            *hay_anterior = true;
            *seq_anterior = seq;
            *casilla_anterior = casilla;
            *resultado_anterior = resultado;
        }
    }
    else
        resultado = RESULTADO_INVALIDO;

    return resultado;
}

static void esperar_ack_resultado(uint8_t jugador, uint8_t seq,
                                  uint8_t casilla, uint8_t resultado,
                                  uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                  bool *hay_anterior, uint8_t *seq_anterior,
                                  uint8_t *casilla_anterior,
                                  uint8_t *resultado_anterior)
{
    uint8_t trama[5];

    while (1)
    {
        ENLACE_enviar_resultado(jugador, seq, resultado);
        if (!recibir_trama_timeout(trama, 1000))
            continue;

        if (trama[1] == CMD_ACK_RESULTADO && trama[2] == jugador && trama[3] == seq)
            return;

        if (trama[1] == CMD_DISPARO_RX && trama[2] == jugador &&
            trama[3] == seq && trama[4] == casilla)
        {
            /* La peticion original se repitio: el resultado se reenviara arriba. */
            responder_disparo(trama, flota, hay_anterior, seq_anterior,
                              casilla_anterior, resultado_anterior);
        }
    }
}

static void actualizar_objetivo(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO],
                                uint8_t casilla, uint8_t resultado)
{
    uint8_t fila = casilla / COLS_TABLERO;
    uint8_t col = casilla % COLS_TABLERO;
    uint8_t barco, x, y;

    if (resultado == RES_AGUA)
    {
        objetivo[fila][col] = BOMBA_FALLIDA;
        return;
    }

    if (resultado >= 1 && resultado <= NUM_BARCOS)
    {
        objetivo[fila][col] = resultado + OFFSET_TOCADO;
        return;
    }

    if (resultado > NUM_BARCOS && resultado <= 2 * NUM_BARCOS)
    {
        barco = resultado - NUM_BARCOS;
        objetivo[fila][col] = HUNDIDO;
        for (y = 0; y < FILAS_TABLERO; y++)
            for (x = 0; x < COLS_TABLERO; x++)
                if (objetivo[y][x] == barco + OFFSET_TOCADO)
                    objetivo[y][x] = HUNDIDO;
    }
}

static bool flota_rival_hundida(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t x, y, hundidas = 0;
    for (y = 0; y < FILAS_TABLERO; y++)
        for (x = 0; x < COLS_TABLERO; x++)
            if (objetivo[y][x] == HUNDIDO)
                hundidas++;
    return hundidas == 17; /* longitud total: 2+3+3+4+5 */
}

static void esperar_disparo_rival(uint8_t jugador,
                                  uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                  bool *hay_anterior, uint8_t *seq_anterior,
                                  uint8_t *casilla_anterior, uint8_t *resultado_anterior)
{
    uint8_t trama[5];
    while (1)
    {
        if (!ENLACE_recibir_trama(trama))
        {
            esperar_ms(1);
            continue;
        }

        if (trama[1] == CMD_LISTO && trama[2] == 0 && jugador == 1)
        {
            ENLACE_enviar_ack_listo(1); /* ACK perdido: repetir */
            continue;
        }

        if (trama[1] == CMD_RESULTADO && trama[2] == (uint8_t)(1 - jugador) &&
            *hay_anterior && trama[3] == *seq_anterior)
        {
            ENLACE_enviar_ack_resultado(trama[2], trama[3]);
            continue;
        }

        if (trama[1] == CMD_DISPARO_RX && trama[2] == (uint8_t)(1 - jugador) &&
            trama[4] < FILAS_TABLERO * COLS_TABLERO)
        {
            uint8_t fila = trama[4] / COLS_TABLERO;
            uint8_t col = trama[4] % COLS_TABLERO;
            uint8_t resultado;
            bool nuevo = !*hay_anterior || trama[3] != *seq_anterior ||
                         trama[4] != *casilla_anterior;
            char mensaje[20];

            resultado = responder_disparo(trama, flota, hay_anterior, seq_anterior,
                                          casilla_anterior, resultado_anterior);
            if (nuevo && resultado != RESULTADO_INVALIDO)
                registrar_estadistica_disparo(trama[2], resultado);
            sprintf(mensaje, "Disparo en %c%d", 'A' + fila, col + 1);
            pantalla_militar(mensaje, resultado == RES_AGUA ? "Agua" : "Te han tocado",
                             "Tu turno despues",
                             resultado == RES_AGUA ? ESTILO_AGUA : ESTILO_TOCADO);
            esperar_ack_resultado(trama[2], trama[3], trama[4], resultado, flota,
                                  hay_anterior, seq_anterior, casilla_anterior,
                                  resultado_anterior);
            esperar_ms(MS_PANTALLA_RESULTADO);   /* que de tiempo a leerlo */
            return;
        }
    }
}

/* Al ganar, sigue confirmando el resultado por si se perdio el primer ACK. */
static void mantener_pantalla_ganador(uint8_t jugador)
{
    uint8_t trama[5];
    while (1)
    {
        if (MENU_boton_pulsado())
        {
            MENU_abrir();
            pantalla_militar("Has ganado", "Flota rival hundida", "P5.5: menu", ESTILO_GANADO);
        }
        if (ENLACE_recibir_trama(trama) && trama[1] == CMD_RESULTADO &&
            trama[2] == jugador)
            ENLACE_enviar_ack_resultado(jugador, trama[3]);
        esperar_ms(1);
    }
}

static uint8_t enviar_disparo_y_esperar(uint8_t jugador, uint8_t secuencia,
                                        uint8_t casilla,
                                        uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                        bool *hay_anterior, uint8_t *seq_anterior,
                                        uint8_t *casilla_anterior,
                                        uint8_t *resultado_anterior)
{
    uint8_t trama[5];

    while (1)
    {
        ENLACE_enviar_disparo(jugador, secuencia, casilla);
        if (!recibir_trama_timeout(trama, 1000))
            continue; /* no hubo respuesta: repetir el mismo disparo */

        if (trama[1] == CMD_RESULTADO && trama[2] == jugador &&
            trama[3] == secuencia && trama[4] <= 2 * NUM_BARCOS)
        {
            ENLACE_enviar_ack_resultado(jugador, secuencia);
            return trama[4];
        }

        /* Una petición anterior repetida se contesta aunque estemos jugando. */
        if (trama[1] == CMD_DISPARO_RX && trama[2] == (uint8_t)(1 - jugador) &&
            trama[4] < FILAS_TABLERO * COLS_TABLERO)
        {
            uint8_t resultado_rival = responder_disparo(trama, flota, hay_anterior,
                                                       seq_anterior, casilla_anterior,
                                                       resultado_anterior);
            esperar_ack_resultado(trama[2], trama[3], trama[4], resultado_rival,
                                  flota, hay_anterior, seq_anterior,
                                  casilla_anterior, resultado_anterior);
        }
    }
}

static void debug_mostrar_matriz(uint8_t t[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t fila, col;
        char txt[2] = {'0', '\0'};

        Graphics_setFont(&g_sContext, &g_sFontCm12);
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
        Graphics_clearDisplay(&g_sContext);

        for (fila = 0; fila < 10; fila++)
        {
            for (col = 0; col < 10; col++)
            {
                txt[0] = '0' + t[fila][col];
                Graphics_drawString(&g_sContext, (int8_t *)txt, AUTO_STRING_LENGTH,
                                    4 + col * 12, 4 + fila * 12, false);
            }
        }
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
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);

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

    while (1)
    {

        /*
         ---------------- Nueva partida ----------------
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
        */

#if MODO_MARCADOR
        marcador();
        while (1)
            MARCADOR_receptor_procesar();
#else
        {
            uint8_t jugador = JUGADOR_LOCAL;
            uint8_t fila, columna, casilla, resultado;
            uint8_t secuencia = 0;
            uint8_t seq_anterior = 0, casilla_anterior = 0;
            uint8_t resultado_anterior = RESULTADO_INVALIDO;
            bool hay_anterior = false;
            bool turno_local;
            char linea[20];

            if (jugador > 1)
                jugador = 0;
            memset(tableros, AGUA, sizeof(tableros));
            marcador_reiniciar_estadisticas();

#if USAR_TABLEROS_DE_PRUEBA
            tablero_de_prueba(tableros[jugador], jugador);
#else
            colocar_flota(tableros[jugador], jugador);
#endif

            iniciar_enlace_juego(jugador);
            turno_local = (jugador == 0); /* jugador 1 empieza */

            while (1)
            {
                if (turno_local)
                {
                    esperar_inicio_turno();
                    sprintf(linea, "Turno jugador %d", jugador + 1);
                    pantalla_mensaje(linea, "Elige una casilla", "P5.6: mi tablero");

                    if (!seleccionar_disparo(tableros[1 - jugador], tableros[jugador],
                                             &fila, &columna))
                        continue;

                    casilla = fila * COLS_TABLERO + columna;
                    pantalla_mensaje("Disparo enviado", "Esperando respuesta", "No apagues la radio");
                    resultado = enviar_disparo_y_esperar(jugador, secuencia, casilla,
                                                         tableros[jugador], &hay_anterior,
                                                         &seq_anterior, &casilla_anterior,
                                                         &resultado_anterior);
                    secuencia = (secuencia + 1) % 100;

                    if (resultado == RESULTADO_INVALIDO)
                        continue;

                    registrar_estadistica_disparo(jugador, resultado);
                    actualizar_objetivo(tableros[1 - jugador], casilla, resultado);
                    if (resultado == RES_AGUA)
                        pantalla_militar("Agua", "", "Turno del rival", ESTILO_AGUA);
                    else if (resultado <= NUM_BARCOS)
                        pantalla_militar("Tocado", "", "Turno del rival", ESTILO_TOCADO);
                    else
                        pantalla_militar("Hundido", "", "Turno del rival", ESTILO_HUNDIDO);
                    esperar_ms(MS_PANTALLA_RESULTADO);

                    if (flota_rival_hundida(tableros[1 - jugador]))
                    {
                        pantalla_militar("Has ganado", "Flota rival hundida", "P5.5: menu", ESTILO_GANADO);
                        mantener_pantalla_ganador(jugador);
                    }
                    turno_local = false;
                }
                else
                {
                    pantalla_mensaje("Turno del rival", "Esperando disparo", "No apagues la radio");
                    esperar_disparo_rival(jugador, tableros[jugador], &hay_anterior,
                                          &seq_anterior, &casilla_anterior,
                                          &resultado_anterior);

                    if (flota_hundida(tableros[jugador]))
                    {
                        pantalla_militar("Has perdido", "Tu flota hundida", "P5.5: menu", ESTILO_PERDIDO);
                        while (1)
                        {
                            if (MENU_boton_pulsado())
                            {
                                MENU_abrir();
                                pantalla_militar("Has perdido", "Tu flota hundida", "P5.5: menu", ESTILO_PERDIDO);
                            }
                            esperar_ms(1);
                        }
                    }
                    turno_local = true;
                }
            }
        }
#endif
    }
}
