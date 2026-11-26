/*
 * tablero.h
 * Constantes comunes y formato de la matriz del tablero.
 */

#ifndef TABLERO_H_
#define TABLERO_H_

#include <stdint.h>

#define FILAS_TABLERO   10
#define COLS_TABLERO    10

/*
 * Convencion: tablero[fila][columna], con fila 0..9 (A..J) y columna 0..9
 * (1..10). Los identificadores de barco son BARCO_2..BARCO_5.
 *
 * Codificacion de una casilla:
 *   0      agua sin disparar
 *   1..5   barco intacto (el valor identifica el barco)
 *   6..10  barco tocado (id del barco + OFFSET_TOCADO)
 *   11     barco hundido (todas sus casillas pasan a este valor)
 *   12     disparo al agua (BOMBA_FALLIDA)
 */

/* Estados de casilla */
#define AGUA            0
#define BARCO_2         1   /* longitud 2 */
#define BARCO_3A        2   /* longitud 3 */
#define BARCO_3B        3   /* longitud 3 */
#define BARCO_4         4   /* longitud 4 */
#define BARCO_5         5   /* longitud 5 */
#define OFFSET_TOCADO   5   /* barco tocado = id + OFFSET_TOCADO (6..10) */
#define HUNDIDO         11
#define BOMBA_FALLIDA   12

#define NUM_BARCOS      5

/* Ayudas para interpretar una casilla */
#define ES_BARCO_INTACTO(e)  ((e) >= BARCO_2 && (e) <= BARCO_5)
#define ES_BARCO_TOCADO(e)   ((e) >= BARCO_2 + OFFSET_TOCADO && \
                              (e) <= BARCO_5 + OFFSET_TOCADO)

#endif /* TABLERO_H_ */
