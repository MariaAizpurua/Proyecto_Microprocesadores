/*
 * vista_tablero.c
 * Dibujo del tablero y movimiento del cursor.
 */

#include "vista_tablero.h"
#include "Pantalla_drw.h"
#include <msp430.h>
#include <driverlib.h>
#include <drivers/joystick.h>
#include <drivers/buzzer.h>

/* Convencion de ejes: x = columna, y = fila, matriz[y][x].
 * Ver vista_tablero.h. */

/* ---------------------------------------------------------------
 * Geometria: la misma que marcar_casillas() de interfaz_jugar.c y
 * las cuadriculas de Pantalla_drw.c (pantalla 128x128).
 *   columnas 0..4 (pantallas 11/21): xMin = 28 + x*20
 *   columnas 5..9 (pantallas 12/22): xMin = (x-5)*20 + 1
 *   filas    0..4 (pantallas 11/12): yMin = 28 + y*20
 *   filas    5..9 (pantallas 21/22): yMin = (y-5)*20 + 1
 * --------------------------------------------------------------- */
#define TAM_CASILLA     20
#define LADO_RELLENO    18      /* xMax = xMin + 18 (como en disparar) */
#define CASILLAS_LADO   5

#define UMBRAL_JOYSTICK   30          /* igual que en interfaz_jugar.c */
#define RETARDO_JOYSTICK  3200000     /* 200 ms a 16 MHz */
#define RETARDO_REBOTE    160000      /* 10 ms a 16 MHz */

/* Colores (misma paleta que la pantalla de disparar) */
#define COLOR_BARCO       GRAPHICS_COLOR_GRAY
#define COLOR_TOCADO      GRAPHICS_COLOR_LIGHT_CORAL
#define COLOR_HUNDIDO     GRAPHICS_COLOR_DARK_RED
#define COLOR_FALLO       GRAPHICS_COLOR_LIGHT_SKY_BLUE
#define COLOR_AGUA        GRAPHICS_COLOR_WHITE
#define COLOR_CURSOR      GRAPHICS_COLOR_RED
#define COLOR_INDICADOR   GRAPHICS_COLOR_LIGHT_GRAY

/* ---------------------------------------------------------------
 * Boton P5.6 (S1 de la LaunchPad)
 * Activo a nivel bajo: pull-up interno, al pulsar se lee 0.
 * La interrupcion guarda la pulsacion para no perderla aunque el
 * bucle que pregunta tenga retardos largos (disparar() tiene 400 ms).
 * OJO: este fichero define la ISR del puerto 5. Si algun dia otro
 * modulo necesita PORT5_VECTOR, hay que juntarlas en una sola.
 * --------------------------------------------------------------- */
static volatile bool flag_p56 = false;
static bool p56_pulsado = false;

void vista_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_selectInterruptEdge(GPIO_PORT_P5, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P5, GPIO_PIN6);
    flag_p56 = false;
    p56_pulsado = false;
}

#pragma vector=PORT5_VECTOR
__interrupt void vista_port5_isr(void)
{
    if (GPIO_getInterruptStatus(GPIO_PORT_P5, GPIO_PIN6) & GPIO_PIN6)
    {
        flag_p56 = true;
        GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN6);
    }
}

/* Misma maquina de estados que switch.c: una pulsacion = un evento,
 * con antirrebote al pulsar y al soltar. */
bool vista_boton_pulsado(void)
{
    if (!p56_pulsado)
    {
        if (flag_p56)
        {
            __delay_cycles(RETARDO_REBOTE);
            flag_p56 = false;
            p56_pulsado = true;
            return true;
        }
    }
    else if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6) == GPIO_INPUT_PIN_HIGH)
    {
        __delay_cycles(RETARDO_REBOTE);
        flag_p56 = false;
        if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN6) == GPIO_INPUT_PIN_HIGH)
        {
            p56_pulsado = false;
        }
    }
    return false;
}

/* ---------------------------------------------------------------
 * Funciones auxiliares de dibujo (privadas)
 * --------------------------------------------------------------- */

/* Primera casilla (0 o 5) del cuadrante que contiene "valor". */
static uint8_t inicio_cuadrante(uint8_t valor)
{
    return (valor >= CASILLAS_LADO) ? CASILLAS_LADO : 0;
}

static int16_t px_casilla(uint8_t x)
{
    return (x < CASILLAS_LADO) ? 28 + x * TAM_CASILLA
                               : (x - CASILLAS_LADO) * TAM_CASILLA + 1;
}

static int16_t py_casilla(uint8_t y)
{
    return (y < CASILLAS_LADO) ? 28 + y * TAM_CASILLA
                               : (y - CASILLAS_LADO) * TAM_CASILLA + 1;
}

static uint32_t color_de_estado(uint8_t estado)
{
    if (ES_BARCO_INTACTO(estado)) return COLOR_BARCO;
    if (ES_BARCO_TOCADO(estado))  return COLOR_TOCADO;
    if (estado == HUNDIDO)        return COLOR_HUNDIDO;
    if (estado == BOMBA_FALLIDA)  return COLOR_FALLO;
    return COLOR_AGUA;
}

/* Pinta la casilla (x, y) si esta en el cuadrante visible; si es la
 * del cursor, le pone un borde rojo de 2 px (se sigue viendo el
 * color de la casilla). */
static void pintar_casilla(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t x, uint8_t y,
                           uint8_t cursor_x, uint8_t cursor_y)
{
    uint8_t ini_x = inicio_cuadrante(cursor_x);
    uint8_t ini_y = inicio_cuadrante(cursor_y);
    Graphics_Rectangle rect;

    if (x < ini_x || x >= ini_x + CASILLAS_LADO) return;
    if (y < ini_y || y >= ini_y + CASILLAS_LADO) return;

    rect.xMin = px_casilla(x);
    rect.yMin = py_casilla(y);
    rect.xMax = rect.xMin + LADO_RELLENO;
    rect.yMax = rect.yMin + LADO_RELLENO;

    Graphics_setForegroundColor(g, color_de_estado(matriz[y][x]));
    Graphics_fillRectangle(g, &rect);

    if (x == cursor_x && y == cursor_y)
    {
        Graphics_setForegroundColor(g, COLOR_CURSOR);
        Graphics_drawRectangle(g, &rect);
        rect.xMin++; rect.yMin++; rect.xMax--; rect.yMax--;
        Graphics_drawRectangle(g, &rect);
    }
}

/* Cuadricula y letras/numeros del cuadrante (Pantalla_drw.c) y "YO"
 * en la esquina libre que queda entre letras y numeros. */
static void dibujar_marco(Graphics_Context *g, uint8_t cursor_x, uint8_t cursor_y)
{
    Graphics_Rectangle esquina;
    bool derecha = (cursor_x >= CASILLAS_LADO);
    bool abajo   = (cursor_y >= CASILLAS_LADO);

    if (!derecha && !abajo)      pantalla11_drw(g);
    else if (derecha && !abajo)  pantalla12_drw(g);
    else if (!derecha && abajo)  pantalla21_drw(g);
    else                         pantalla22_drw(g);

    esquina.xMin = derecha ? 102 : 1;
    esquina.xMax = esquina.xMin + 24;
    esquina.yMin = abajo ? 102 : 1;
    esquina.yMax = esquina.yMin + 24;

    Graphics_setForegroundColor(g, COLOR_INDICADOR);
    Graphics_fillRectangle(g, &esquina);
    Graphics_setForegroundColor(g, GRAPHICS_COLOR_BLACK);
    Graphics_drawStringCentered(g, (int8_t *)"YO", AUTO_STRING_LENGTH,
                                esquina.xMin + 12, esquina.yMin + 12, false);
}

/* ---------------------------------------------------------------
 * Funciones publicas
 * --------------------------------------------------------------- */

void vista_dibujar_tablero(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t cursor_x, uint8_t cursor_y)
{
    uint8_t dx, dy;
    uint8_t ini_x = inicio_cuadrante(cursor_x);
    uint8_t ini_y = inicio_cuadrante(cursor_y);

    Graphics_setBackgroundColor(g, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(g);

    for (dy = 0; dy < CASILLAS_LADO; dy++)
        for (dx = 0; dx < CASILLAS_LADO; dx++)
            pintar_casilla(g, matriz, ini_x + dx, ini_y + dy,
                           cursor_x, cursor_y);

    dibujar_marco(g, cursor_x, cursor_y);
}

void vista_mover_cursor(Graphics_Context *g,
                        uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                        uint8_t x_ant, uint8_t y_ant,
                        uint8_t x_nue, uint8_t y_nue)
{
    if (x_ant == x_nue && y_ant == y_nue) return;

    /* Cambia de cuadrante: redibujar todo */
    if (inicio_cuadrante(x_ant) != inicio_cuadrante(x_nue) ||
        inicio_cuadrante(y_ant) != inicio_cuadrante(y_nue))
    {
        vista_dibujar_tablero(g, matriz, x_nue, y_nue);
        return;
    }

    /* Mismo cuadrante: quitar el cursor de la anterior y ponerlo en la nueva */
    pintar_casilla(g, matriz, x_ant, y_ant, x_nue, y_nue);
    pintar_casilla(g, matriz, x_nue, y_nue, x_nue, y_nue);
}

void vista_actualizar_casilla(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t x, uint8_t y,
                              uint8_t cursor_x, uint8_t cursor_y)
{
    pintar_casilla(g, matriz, x, y, cursor_x, cursor_y);
}

void vista_mostrar_mi_tablero(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO])
{
    uint8_t x = 0, y = 0, nx, ny;
    float jx, jy;

    vista_dibujar_tablero(g, matriz, x, y);

    while (!vista_boton_pulsado())          /* P5.6 otra vez -> salir */
    {
        nx = x;
        ny = y;
        jx = JOYSTICK_get_x();
        jy = JOYSTICK_get_y();

        if (jx > UMBRAL_JOYSTICK && x < COLS_TABLERO - 1)        nx++;
        else if (jx < -UMBRAL_JOYSTICK && x > 0)                 nx--;
        if (jy > UMBRAL_JOYSTICK && y > 0)                       ny--;  /* arriba */
        else if (jy < -UMBRAL_JOYSTICK && y < FILAS_TABLERO - 1) ny++;  /* abajo */

        if (nx != x || ny != y)
        {
            vista_mover_cursor(g, matriz, x, y, nx, ny);
            x = nx;
            y = ny;
            BUZ_wait_ms(200);
        }
        BUZ_wait_ms(1);
    }
}
