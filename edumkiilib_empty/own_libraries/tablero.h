/*
 * tablero.h
 *
 * Definiciones COMUNES del tablero, compartidas por todos los modulos
 * (interfaz_jugar, vista_tablero, place_ship, main...).
 *
 * Convencion de ejes:  x = columna (0..9), y = fila (0..9)
 *                      la matriz se accede SIEMPRE como tablero[y][x]
 *                      fila  -> letra  A..J
 *                      columna -> numero 1..10
 *
 * Codificacion de cada casilla (es la que usa disparar() en
 * interfaz_jugar.c, por eso es la que manda):
 *
 *    0        agua sin disparar
 *    1..5     barco sin tocar (el valor es el id del barco)
 *    6..10    barco tocado     (id del barco + 5)
 *    11       barco hundido    (todas sus casillas pasan a 11)
 *    12       bomba fallida    (disparo al agua)
 */

#ifndef TABLERO_H_
#define TABLERO_H_

#include <stdint.h>

#define FILAS_TABLERO   10
#define COLS_TABLERO    10

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
