#include "vista_tablero.h"

/* Convención de ejes: x = columna, y = fila, matriz[y][x].
 * Ver vista_tablero.h.
 * Los puntos pendientes de acordar con el equipo están marcados con
 * TODO(equipo). */

/* ---------------------------------------------------------------
 * Geometría (pantalla 128x128)
 * Dejamos 23 px arriba e izquierda para el indicador y las cabeceras.
 * 5 casillas * 20 px = 100 px -> el tablero ocupa de 23 a 123.
 * TODO(equipo): si la compañera usa otras medidas en su pantalla,
 * igualarlas aquí para que las dos se vean consistentes.
 * --------------------------------------------------------------- */
#define MARGEN_X        23
#define MARGEN_Y        23
#define TAM_CASILLA     20
#define CASILLAS_LADO   5

/* Cabeceras: columnas (x) = números 1..10 arriba,
 *            filas    (y) = letras A..J a la izquierda.
 * El cuadrado de arriba a la izquierda (MARGEN_X x MARGEN_Y) es el
 * indicador de en qué pantalla estás. */
#define Y_CAB_COLS      ((MARGEN_Y - 8) / 2)   /* texto 6x8 centrado */
#define X_CAB_FILAS     ((MARGEN_X - 6) / 2)

/* Colores de fondo y texto.
 * TODO(equipo): en el vídeo de la compañera el tablero tiene casillas
 * claras y el rival se ve sobre fondo claro; si se quiere el mismo
 * aspecto en las dos pantallas, cambiar aquí. */
#define COLOR_FONDO     GRAPHICS_COLOR_BLACK
#define COLOR_TEXTO     GRAPHICS_COLOR_WHITE

/* Cursor.
 * TODO(equipo): en "mi tablero" el cursor solo sirve para moverse entre
 * cuadrantes y ver el tablero entero. Poner a 0 si no se quiere ver. */
#define CURSOR_VISIBLE  1
#define COLOR_CURSOR    GRAPHICS_COLOR_YELLOW

/* ---------------------------------------------------------------
 * Funciones auxiliares (privadas)
 * --------------------------------------------------------------- */

/* Devuelve la primera casilla (0 ó 5) del cuadrante que contiene 'valor'. */
static uint8_t inicio_cuadrante(uint8_t valor)
{
    return (valor >= CASILLAS_LADO) ? CASILLAS_LADO : 0;
}

/* Color de cada estado.
 * TODO(equipo): acordar la paleta definitiva con la compañera para que
 * "tocado", "hundido", "agua" y "fallo" se vean igual en las dos
 * pantallas. En su vídeo: tocado = rojo, hundido = rojo oscuro,
 * agua = azul claro. Aquí: tocado = naranja, hundido = rojo,
 * fallo = blanco.
 * TODO(equipo): ahora todos los barcos sin tocar son del mismo gris;
 * se podría usar un color por tipo de barco si se quiere. */
static uint32_t color_de_estado(uint8_t estado, uint8_t mostrar_barcos)
{
    switch (estado) {
    case BARCO_2:
    case BARCO_3A:
    case BARCO_3B:
    case BARCO_4:
    case BARCO_5:
        /* En el tablero del rival no se ven los barcos sin tocar */
        return mostrar_barcos ? GRAPHICS_COLOR_GRAY : GRAPHICS_COLOR_BLUE;
    case TOCADO:
        return GRAPHICS_COLOR_ORANGE;
    case HUNDIDO:
        return GRAPHICS_COLOR_RED;
    case BOMBA_FALLIDA:
        return GRAPHICS_COLOR_WHITE;
    case AGUA:
    default:
        return GRAPHICS_COLOR_BLUE;
    }
}

/* Pinta la casilla (x, y) si está en el cuadrante visible; si es la
 * del cursor, le añade el borde. */
static void pintar_casilla(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t x, uint8_t y,
                           uint8_t cursor_x, uint8_t cursor_y,
                           uint8_t mostrar_barcos)
{
    uint8_t ini_x = inicio_cuadrante(cursor_x);
    uint8_t ini_y = inicio_cuadrante(cursor_y);

    if (x < ini_x || x >= ini_x + CASILLAS_LADO) return;
    if (y < ini_y || y >= ini_y + CASILLAS_LADO) return;

    int16_t px_min = MARGEN_X + (x - ini_x) * TAM_CASILLA;
    int16_t py_min = MARGEN_Y + (y - ini_y) * TAM_CASILLA;

    Graphics_Rectangle rect;
    rect.xMin = px_min;
    rect.yMin = py_min;
    rect.xMax = px_min + TAM_CASILLA - 2;   /* -2 deja 1 px de separación */
    rect.yMax = py_min + TAM_CASILLA - 2;

    Graphics_setForegroundColor(g, color_de_estado(matriz[y][x],
                                                   mostrar_barcos));
    Graphics_fillRectangle(g, &rect);

#if CURSOR_VISIBLE
    if (x == cursor_x && y == cursor_y) {
        Graphics_setForegroundColor(g, COLOR_CURSOR);
        Graphics_drawRectangle(g, &rect);
    }
#endif
}

/* TODO(equipo): según la versión de grlib, el parámetro de texto puede
 * ser int8_t* o char*, y la constante de texto transparente puede
 * llamarse distinto (GRAPHICS_TRANSPARENT_TEXT / TRANSPARENT_TEXT).
 * Ajustar aquí si el compilador da error. */
static void dibujar_texto(Graphics_Context *g, const char *txt,
                          int32_t x, int32_t y)
{
    Graphics_drawString(g, (int8_t *)txt, GRAPHICS_AUTO_STRING_LENGTH,
                        x, y, GRAPHICS_TRANSPARENT_TEXT);
}

/* Cabeceras del cuadrante visible: números arriba, letras a la izquierda. */
static void dibujar_cabeceras(Graphics_Context *g,
                              uint8_t cursor_x, uint8_t cursor_y)
{
    uint8_t ini_x = inicio_cuadrante(cursor_x);
    uint8_t ini_y = inicio_cuadrante(cursor_y);
    uint8_t i;
    char buf[3];

    Graphics_setForegroundColor(g, COLOR_TEXTO);

    /* Columnas (x): números 1..5 o 6..10, centrados sobre cada casilla */
    for (i = 0; i < CASILLAS_LADO; i++) {
        uint8_t n = ini_x + i + 1;
        if (n < 10) {
            buf[0] = '0' + n;
            buf[1] = '\0';
            dibujar_texto(g, buf, MARGEN_X + i * TAM_CASILLA + 7, Y_CAB_COLS);
        } else {
            buf[0] = '1';
            buf[1] = '0';
            buf[2] = '\0';
            dibujar_texto(g, buf, MARGEN_X + i * TAM_CASILLA + 4, Y_CAB_COLS);
        }
    }

    /* Filas (y): letras A..E o F..J, a la altura de cada casilla */
    for (i = 0; i < CASILLAS_LADO; i++) {
        buf[0] = 'A' + ini_y + i;
        buf[1] = '\0';
        dibujar_texto(g, buf, X_CAB_FILAS, MARGEN_Y + i * TAM_CASILLA + 5);
    }
}

/* Cuadrado de arriba a la izquierda: indica en qué pantalla estás.
 *   "YO"  sobre fondo gris -> mi tablero (los barcos propios son grises)
 *   "RIV" solo con marco   -> tablero del rival
 * TODO(equipo): acordar con la compañera cómo se indica la pantalla en
 * su lado (texto, icono, color...) para que las dos sean coherentes.
 * Las posiciones del texto (5,7) y (2,7) están ajustadas a ojo para la
 * fuente 6x8; revisarlas en la placa real. */
static void dibujar_indicador(Graphics_Context *g, uint8_t mostrar_barcos)
{
    Graphics_Rectangle r;
    r.xMin = 0;
    r.yMin = 0;
    r.xMax = MARGEN_X - 2;
    r.yMax = MARGEN_Y - 2;

    if (mostrar_barcos) {
        Graphics_setForegroundColor(g, GRAPHICS_COLOR_GRAY);
        Graphics_fillRectangle(g, &r);
        Graphics_setForegroundColor(g, COLOR_TEXTO);
        dibujar_texto(g, "YO", 5, 7);
    } else {
        Graphics_setForegroundColor(g, COLOR_TEXTO);
        Graphics_drawRectangle(g, &r);
        dibujar_texto(g, "RIV", 2, 7);
    }
}

/* ---------------------------------------------------------------
 * Funciones públicas
 * --------------------------------------------------------------- */

void vista_dibujar_tablero(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t cursor_x, uint8_t cursor_y,
                           uint8_t mostrar_barcos)
{
    uint8_t dx, dy;
    uint8_t ini_x = inicio_cuadrante(cursor_x);
    uint8_t ini_y = inicio_cuadrante(cursor_y);

    /* TODO(equipo): si otro módulo ya configura la fuente y el color de
     * fondo del contexto gráfico, quitar estas dos líneas. */
    Graphics_setFont(g, &g_sFontFixed6x8);
    Graphics_setBackgroundColor(g, COLOR_FONDO);
    Graphics_clearDisplay(g);

    dibujar_indicador(g, mostrar_barcos);
    dibujar_cabeceras(g, cursor_x, cursor_y);

    for (dy = 0; dy < CASILLAS_LADO; dy++) {
        for (dx = 0; dx < CASILLAS_LADO; dx++) {
            pintar_casilla(g, matriz, ini_x + dx, ini_y + dy,
                           cursor_x, cursor_y, mostrar_barcos);
        }
    }
}

void vista_mover_cursor(Graphics_Context *g,
                        uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                        uint8_t x_ant, uint8_t y_ant,
                        uint8_t x_nue, uint8_t y_nue,
                        uint8_t mostrar_barcos)
{
    if (x_ant == x_nue && y_ant == y_nue) return;

    /* ¿Cambia de cuadrante? Entonces hay que redibujar todo */
    if (inicio_cuadrante(x_ant) != inicio_cuadrante(x_nue) ||
        inicio_cuadrante(y_ant) != inicio_cuadrante(y_nue)) {
        vista_dibujar_tablero(g, matriz, x_nue, y_nue, mostrar_barcos);
        return;
    }

    /* Mismo cuadrante: quitar el borde de la casilla anterior y
     * poner el de la nueva. */
    pintar_casilla(g, matriz, x_ant, y_ant, x_nue, y_nue, mostrar_barcos);
    pintar_casilla(g, matriz, x_nue, y_nue, x_nue, y_nue, mostrar_barcos);
}

void vista_actualizar_casilla(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t x, uint8_t y,
                              uint8_t cursor_x, uint8_t cursor_y,
                              uint8_t mostrar_barcos)
{
    pintar_casilla(g, matriz, x, y, cursor_x, cursor_y, mostrar_barcos);
}
