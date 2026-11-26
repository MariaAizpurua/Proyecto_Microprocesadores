/*
 * protocolo_partida.h
 * Interfaz del protocolo entre los dos jugadores.
 */

#ifndef PROTOCOLO_PARTIDA_H_
#define PROTOCOLO_PARTIDA_H_

#include <stdbool.h>
#include <stdint.h>
#include <own_libraries/tablero.h>

typedef struct {
    bool hay_anterior;
    uint8_t secuencia_anterior;
    uint8_t casilla_anterior;
    uint8_t resultado_anterior;
} protocolo_partida_estado_t;

void PROTOCOLO_iniciar(uint8_t jugador);
void PROTOCOLO_esperar_disparo_rival(uint8_t jugador,
                                     uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                     protocolo_partida_estado_t *estado,
                                     uint8_t *casilla,
                                     uint8_t *resultado);
void PROTOCOLO_confirmar_resultado_ganador(uint8_t jugador);
uint8_t PROTOCOLO_enviar_disparo_y_esperar(uint8_t jugador, uint8_t secuencia,
                                           uint8_t casilla,
                                           uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                           protocolo_partida_estado_t *estado);

#endif /* PROTOCOLO_PARTIDA_H_ */
