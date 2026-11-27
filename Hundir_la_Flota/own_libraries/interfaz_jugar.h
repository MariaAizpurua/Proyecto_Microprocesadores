/*
 * interfaz_jugar.h
 * Interfaz para seleccionar y mostrar disparos.
 * Autor: maria.
 */

#ifndef INTERFAZ_JUGAR_H_
#define INTERFAZ_JUGAR_H_

#include <stdbool.h>
#include <stdint.h>

void marcar_casillas(uint8_t tablero[10][10], int cuadrante);
// objetivo = mapa local de disparos; mi_flota = flota propia (P5.6).
/* Selecciona destino sin resolverlo: el rival responde por radio/UART. */
bool seleccionar_disparo(uint8_t objetivo[10][10], uint8_t mi_flota[10][10],
                         uint8_t *fila, uint8_t *columna);

#endif /* INTERFAZ_JUGAR_H_ */
