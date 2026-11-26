/*
 * enlace_juego.h
 * Interfaz y comandos del enlace UART de la partida.
 */

#ifndef ENLACE_JUEGO_H_
#define ENLACE_JUEGO_H_

#include <stdbool.h>
#include <stdint.h>

/*
 * Formato de cada trama UART (5 bytes):
 * [0] 0xAA; [1] comando; [2] jugador (0 = jugador 1, 1 = jugador 2);
 * [3] y [4] son datos cuyo significado depende del comando.
 * El CC1310 reenvia la trama completa por radio sin interpretarla.
 *
 * H: jugador 1 anuncia que esta listo; A: jugador 2 confirma.
 * S: disparo; [3] secuencia, [4] casilla (fila * 10 + columna).
 * R: resultado del disparo; [3] secuencia, [4] resultado:
 *    0 = agua, 1..5 = tocado (id del barco), 6..10 = hundido (id + 5).
 * K: confirmacion de recepcion del resultado; [3] secuencia.
 */

/* Habilita la transmision UART hacia el CC1310. */
#define ENLACE_ENVIO_ACTIVO  1
#define ENLACE_INICIO_TRAMA  0xAA

/* Protocolo de la partida; todas las tramas tienen cinco bytes. */
#define CMD_LISTO             'H'
#define CMD_ACK_LISTO         'A'
#define CMD_DISPARO_RX        'S'
#define CMD_RESULTADO         'R'
#define CMD_ACK_RESULTADO     'K'

#define RES_AGUA              0
#define RES_TOCADO            1
#define RES_HUNDIDO           2
#define RESULTADO_INVALIDO    0xFF

/* Lee una trama completa sin bloquear; devuelve true cuando hay una. */
bool ENLACE_recibir_trama(uint8_t trama[5]);
void ENLACE_enviar_listo(uint8_t jugador);
void ENLACE_enviar_ack_listo(uint8_t jugador);
void ENLACE_enviar_disparo(uint8_t jugador, uint8_t secuencia, uint8_t casilla);
void ENLACE_enviar_resultado(uint8_t jugador, uint8_t secuencia, uint8_t resultado);
void ENLACE_enviar_ack_resultado(uint8_t jugador, uint8_t secuencia);

#endif /* ENLACE_JUEGO_H_ */
