/*
 * tablero_juego.h
 * Operaciones sobre los tableros de juego.
 */

#ifndef TABLERO_JUEGO_H_
#define TABLERO_JUEGO_H_

#include <stdbool.h>
#include <stdint.h>
#include <own_libraries/tablero.h>

bool TABLERO_flota_hundida(uint8_t flota[FILAS_TABLERO][COLS_TABLERO]);
uint8_t TABLERO_resolver_disparo(uint8_t flota[FILAS_TABLERO][COLS_TABLERO], uint8_t casilla);
void TABLERO_actualizar_objetivo(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO], uint8_t casilla, uint8_t resultado);
bool TABLERO_flota_rival_hundida(uint8_t objetivo[FILAS_TABLERO][COLS_TABLERO]);

#endif /* TABLERO_JUEGO_H_ */
