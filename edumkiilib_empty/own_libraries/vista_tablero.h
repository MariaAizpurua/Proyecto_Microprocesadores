/*
 * vista_tablero.h
 * Interfaz de visualizacion del tablero.
 */

#ifndef VISTA_TABLERO_H_
#define VISTA_TABLERO_H_

#include <stdint.h>
#include <stdbool.h>
#include "grlib.h"
#include "tablero.h"

/* ===============================================================
 * vista_tablero  -  Pantalla "MI TABLERO" (situacion de mis barcos y
 * donde me ha disparado el rival).
 *
 * Mismo formato que la pantalla de disparar (interfaz_jugar.c):
 * fondo blanco, cuadricula de Pantalla_drw y un cuadrante 5x5 cada vez.
 *
 * Se entra y se sale con el boton S1 de la LaunchPad (P5.6).
 *
 * Convencion de ejes: x = columna (0..9), y = fila (0..9),
 * la matriz se accede SIEMPRE como matriz[y][x].
 * Columnas -> numeros 1..10, filas -> letras A..J.
 *
 * Colores:  barco propio = gris, tocado = coral claro,
 *           hundido = rojo oscuro, bomba fallida = azul cielo claro,
 *           cursor = borde rojo. En la esquina libre pone "YO".
 * =============================================================== */

/* Configura el boton P5.6 (entrada con pull-up + interrupcion).
 * Llamar una vez en main, despues de PMM_unlockLPM5() y antes de
 * activar las interrupciones (GIE). */
void vista_init(void);

/* true una sola vez por cada pulsacion de P5.6 (con antirrebote). */
bool vista_boton_pulsado(void);

/* Pantalla completa "mi tablero": el joystick mueve el cursor por los
 * 4 cuadrantes y se queda aqui hasta que se vuelve a pulsar P5.6.
 * Al volver, quien la llamo tiene que redibujar su pantalla. */
void vista_mostrar_mi_tablero(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO]);

/* Dibuja el cuadrante (5x5) donde esta el cursor. */
void vista_dibujar_tablero(Graphics_Context *g,
                           uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                           uint8_t cursor_x, uint8_t cursor_y);

/* Mueve el cursor. Si cambia de cuadrante redibuja todo; si no, solo
 * repinta las dos casillas afectadas. */
void vista_mover_cursor(Graphics_Context *g,
                        uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                        uint8_t x_ant, uint8_t y_ant,
                        uint8_t x_nue, uint8_t y_nue);

/* Repinta la casilla (x, y) (p. ej. tras un disparo del rival).
 * No hace nada si no esta en el cuadrante visible. */
void vista_actualizar_casilla(Graphics_Context *g,
                              uint8_t matriz[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t x, uint8_t y,
                              uint8_t cursor_x, uint8_t cursor_y);

#endif /* VISTA_TABLERO_H_ */
