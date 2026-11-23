/*
 * enlace_marcador.h
 *
 * Protocolo UART usado entre MSP430. Conserva los avisos antiguos para el
 * marcador y añade solicitud/respuesta de disparos para una partida remota.
 *
 * De momento SOLO se define el protocolo y se llama desde main en los
 * puntos correctos. El envio real esta desactivado
 * (MARCADOR_ENVIO_ACTIVO = 0) hasta que el marcador este listo.
 *
 * ---------------------------------------------------------------------
 * TRAMA (siempre 5 bytes):
 *
 *   [0] MARCADOR_INICIO_TRAMA (0xAA)   para sincronizarse
 *   [1] comando                        'I', 'T', 'D' o 'F'
 *   [2] jugador                        0 = PLAYER 1, 1 = PLAYER 2
 *   [3] resultado                      solo en 'D' (si no, 0)
 *   [4] barco                          solo en 'D' (si no, 0xFF)
 *
 * COMANDOS (jugador -> que debe hacer el marcador al recibirlo):
 *
 *   'I'  Inicio de partida     -> marcador()   (ya pone a 0 los
 *                                 contadores)
 *   'T'  Cambio de turno       -> dibujar_turno(jugador)
 *   'D'  Resultado de disparo  -> jugador = el que HA DISPARADO
 *          resultado RES_AGUA     -> registrar_contador(jugador, 2)
 *          resultado RES_TOCADO   -> barco_tocado(jugador, barco)
 *          resultado RES_HUNDIDO  -> barco_tocado(jugador, barco)
 *          (barco_tocado ya decide el solo si es tocado o hundido;
 *           el resultado sirve para comprobar que van sincronizados)
 *   'F'  Fin de partida        -> jugador = el GANADOR
 *                                 (pantalla_marcador aun no tiene
 *                                 funcion para esto)
 *
 * barco = indice 0..4 con el MISMO orden que tamano_barco[] de
 * pantalla_marcador.c = {2,3,3,4,5}, es decir:
 *     barco = (id del barco en el tablero) - 1
 *     BARCO_2 -> 0, BARCO_3A -> 1, BARCO_3B -> 2, BARCO_4 -> 3, BARCO_5 -> 4
 *
 * PARTIDA ENTRE DOS PLACAS (tambien usa tramas de 5 bytes):
 *   'H'  jugador 0 anuncia que esta listo; jugador 1 responde con 'A'.
 *   'S'  jugador, secuencia, casilla (fila*10+columna).
 *   'R'  jugador que disparo, secuencia, resultado.
 *        0=agua, 1..5=tocado (id de barco), 6..10=hundido (id+5).
 *   'K'  confirma que se recibio el resultado.
 * La radio `trx` reenvia estas tramas sin interpretarlas.
 * ---------------------------------------------------------------------
 */

#ifndef ENLACE_MARCADOR_H_
#define ENLACE_MARCADOR_H_

#include <stdbool.h>
#include <stdint.h>

/* Envia por UCA3 hacia el CC1310 (P6.0/P6.1). */
#define MARCADOR_ENVIO_ACTIVO   1

#define MARCADOR_INICIO_TRAMA   0xAA

/* Comandos */
#define CMD_INICIO      'I'
#define CMD_TURNO       'T'
#define CMD_DISPARO     'D'
#define CMD_FIN         'F'

/* Protocolo de partida entre dos MSP430, reenviado sin cambios por la radio. */
#define CMD_LISTO       'H'   /* jugador 0 anuncia que ya coloco su flota */
#define CMD_ACK_LISTO   'A'   /* jugador 1 confirma y espera el primer disparo */
#define CMD_DISPARO_RX  'S'   /* jugador, secuencia, casilla (fila*10+col) */
#define CMD_RESULTADO   'R'   /* jugador, secuencia, resultado codificado */
#define CMD_ACK_RESULTADO 'K' /* confirma que el resultado llego */

/* Resultado de un disparo */
#define RES_AGUA        0
#define RES_TOCADO      1
#define RES_HUNDIDO     2

#define BARCO_NINGUNO   0xFF

/* Resultado remoto: 0=agua, 1..5=tocado barco, 6..10=hundido barco. */
#define RESULTADO_INVALIDO  0xFF

/* Lee una trama completa sin bloquear; devuelve true cuando hay una. */
bool ENLACE_recibir_trama(uint8_t trama[5]);
void ENLACE_enviar_listo(uint8_t jugador);
void ENLACE_enviar_ack_listo(uint8_t jugador);
void ENLACE_enviar_disparo(uint8_t jugador, uint8_t secuencia, uint8_t casilla);
void ENLACE_enviar_resultado(uint8_t jugador, uint8_t secuencia, uint8_t resultado);
void ENLACE_enviar_ack_resultado(uint8_t jugador, uint8_t secuencia);

void MARCADOR_enviar_inicio(void);
void MARCADOR_enviar_turno(uint8_t jugador);
void MARCADOR_enviar_disparo(uint8_t jugador, uint8_t resultado, uint8_t barco);
void MARCADOR_enviar_fin(uint8_t ganador);

/* Llamar repetidamente desde el bucle principal de la placa marcador. */
void MARCADOR_receptor_procesar(void);

#endif /* ENLACE_MARCADOR_H_ */
