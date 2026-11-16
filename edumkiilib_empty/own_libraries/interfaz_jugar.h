/*
 * interfaz_jugar.h
 *
 *  Created on: 14 nov. 2026
 *      Author: maria
 */

#ifndef INTERFAZ_JUGAR_H_
#define INTERFAZ_JUGAR_H_

#include <stdint.h>

void marcar_casillas(uint8_t tablero[10][10], int cuadrante);
// tablero    = tablero del rival (al que se dispara)
// mi_tablero = el mio, se ve al pulsar P5.6 (vista_tablero)
void disparar(uint8_t tablero[10][10], uint8_t mi_tablero[10][10]);

#endif /* INTERFAZ_JUGAR_H_ */
