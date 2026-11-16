/*
 * Pantalla_drw.h
 *
 *  Created on: 9 nov. 2026
 *      Author: arodriguezd.5
 */

#ifndef PANTALLA_DRW_H_
#define PANTALLA_DRW_H_

#include <stdint.h>
#include <stdbool.h>

#include <grlib.h>

void pantalla11_drw(Graphics_Context *g_sContext); // Dibujan el tablero en 4 pantallas
void pantalla12_drw(Graphics_Context *g_sContext);
void pantalla21_drw(Graphics_Context *g_sContext);
void pantalla22_drw(Graphics_Context *g_sContext);

#endif /* PANTALLA_DRW_H_ */
