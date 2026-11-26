/*
 * colocacion_flota.h
 * Interfaz para inicializar y colocar la flota local.
 */

#ifndef COLOCACION_FLOTA_H_
#define COLOCACION_FLOTA_H_

#include <own_libraries/tablero.h>

void COLOCACION_colocar_flota(uint8_t tablero[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t jugador);

#endif /* COLOCACION_FLOTA_H_ */
