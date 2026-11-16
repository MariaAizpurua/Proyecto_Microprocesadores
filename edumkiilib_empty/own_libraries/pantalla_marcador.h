#ifndef PANTALLA_MARCADOR_H_
#define PANTALLA_MARCADOR_H_
#include <stdint.h>

static void dibujar_casilla_vacia(uint8_t jugador,uint8_t barco,uint8_t pos);
void marcador(void);
void dibujar_turno(uint8_t jugador);
void dibujar_contador(uint8_t jugador,uint8_t tipo,uint8_t valor);
void registrar_contador(uint8_t jugador,uint8_t tipo);
void dibujar_barco_tocado(uint8_t jugador,uint8_t barco,uint8_t pos);
void barco_tocado(uint8_t jugador,uint8_t barco);

#endif
