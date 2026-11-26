/*
 * partida_interfaz.h
 * Interfaz de usuario durante la partida.
 */

#ifndef PARTIDA_INTERFAZ_H_
#define PARTIDA_INTERFAZ_H_

#include <own_libraries/tablero.h>

#define PARTIDA_MS_PANTALLA_RESULTADO 2500

void PARTIDA_pantalla_mensaje(const char *l1, const char *l2, const char *l3);
void PARTIDA_esperar_S1(void);
void PARTIDA_esperar_inicio_turno(void);

#endif /* PARTIDA_INTERFAZ_H_ */
