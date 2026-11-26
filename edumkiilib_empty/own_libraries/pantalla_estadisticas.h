/*
 * pantalla_estadisticas.h
 * Interfaz de las estadisticas de la partida.
 */

#ifndef PANTALLA_ESTADISTICAS_H_
#define PANTALLA_ESTADISTICAS_H_

#include <stdint.h>

void ESTADISTICAS_reiniciar(void);
void ESTADISTICAS_mostrar(void);
void ESTADISTICAS_registrar_resultado(uint8_t jugador, uint8_t resultado);

#endif /* PANTALLA_ESTADISTICAS_H_ */
