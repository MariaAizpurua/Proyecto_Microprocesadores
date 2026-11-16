#ifndef VISTA_TABLERO_H_
#define VISTA_TABLERO_H_

#include <stdint.h>
#include "grlib.h"

/* ===============================================================
 * vista_tablero  -  Pantalla "MI TABLERO" (situación de mis barcos)
 *
 * Los puntos marcados con  TODO(equipo)  están pendientes de acordar
 * con los compañeros y pueden tener que cambiarse. Búscalos con
 * Ctrl+F "TODO(equipo)".
 * =============================================================== */

/* ---------------------------------------------------------------
 * CONVENCIÓN DE EJES (válida para TODAS las funciones de este módulo)
 *
 *   x = columna = eje horizontal   (0..9)
 *   y = fila    = eje vertical     (0..9)
 *
 *   La matriz se accede SIEMPRE como  matriz[y][x]
 *   (nunca matriz[x][y]).  cursor_x es la columna y cursor_y la fila.
 *
 * Cabeceras en pantalla (como en la interfaz de la compañera):
 *   columnas (x) -> números 1..10   (arriba)
 *   filas    (y) -> letras A..J     (a la izquierda)
 *   "A2" = fila A (y=0), columna 2 (x=1)
 *
 * TODO(equipo): confirmar que todos los módulos (poner barcos,
 * disparar, estadísticas) usan esta misma convención de ejes.
 * --------------------------------------------------------------- */

/* ---------------------------------------------------------------
 * Estados de casilla.
 * TODO(equipo): estos valores deben ser EXACTAMENTE los que use el
 * módulo del juego. Lo ideal es moverlos a un .h común (p. ej.
 * tablero.h) incluido por todos; entonces se borra este bloque.
 * Si el equipo cambia de modelo (p. ej. guardar el id del barco y los
 * toques por separado, con vidas por barco), habrá que adaptar
 * color_de_estado() en el .c.
 * --------------------------------------------------------------- */
#ifndef AGUA
#define AGUA            0
#define BARCO_2         1
#define BARCO_3A        2
#define BARCO_3B        3
#define BARCO_4         4
#define BARCO_5         5
#define TOCADO          6
#define HUNDIDO         7
#define BOMBA_FALLIDA   8
#endif

#define FILAS_TABLERO   10
#define COLS_TABLERO    10

/* Parámetro mostrar_barcos.
 * TODO(equipo): mi pantalla usa siempre VER_BARCOS. OCULTAR_BARCOS
 * solo hace falta si la compañera reutiliza esta función para la
 * pantalla de disparar; si dibuja la suya propia, se puede quitar. */
#define VER_BARCOS      1   /* pantalla "mi tablero"          */
#define OCULTAR_BARCOS  0   /* pantalla "tablero del rival"   */

/* Dibuja la pantalla completa: indicador, cabeceras y el cuadrante
 * (5x5) donde está el cursor. Úsala al cambiar de pantalla
 * (botón blanco) o de cuadrante.
 * TODO(equipo): decidir quién llama a esta función al pulsar el botón
 * blanco (el módulo que gestione el cambio de pantalla). */
void vista_dibujar_tablero(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t cursor_x, uint8_t cursor_y,
                           uint8_t mostrar_barcos);

/* Mueve el cursor de (x_ant,y_ant) a (x_nue,y_nue).
 * Si cambia de cuadrante redibuja todo; si no, solo repinta las dos
 * casillas afectadas (más rápido y sin parpadeo).
 * TODO(equipo): quién lee el joystick y guarda cursor_x / cursor_y
 * (esta función solo dibuja, no los modifica). Acordar también si el
 * cursor salta al otro cuadrante al llegar al borde (0..9 sin tope). */
void vista_mover_cursor(Graphics_Context *g,
                        uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                        uint8_t x_ant, uint8_t y_ant,
                        uint8_t x_nue, uint8_t y_nue,
                        uint8_t mostrar_barcos);

/* Repinta la casilla (x, y) (por ejemplo, tras un disparo del rival).
 * Lee el estado de matriz[y][x].
 * No hace nada si la casilla no está en el cuadrante visible.
 * TODO(equipo): quién la llama cuando el rival dispara sobre mí
 * (probablemente el módulo del juego, tras actualizar la matriz). */
void vista_actualizar_casilla(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t x, uint8_t y,
                              uint8_t cursor_x, uint8_t cursor_y,
                              uint8_t mostrar_barcos);

#endif /* VISTA_TABLERO_H_ */
