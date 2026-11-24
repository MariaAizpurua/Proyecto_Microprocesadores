/*
 * interfaz_jugar.h
 *
 *  Created on: 14 nov. 2026
 *      Author: maria
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

/* Menu del juego, invocable tambien desde la pantalla de seleccion. */
bool MENU_boton_pulsado(void);
void MENU_abrir(void);

#endif /* INTERFAZ_JUGAR_H_ */
