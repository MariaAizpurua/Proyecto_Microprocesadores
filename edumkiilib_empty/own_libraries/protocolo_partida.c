/*
 * protocolo_partida.c
 * Handshake, disparos, confirmaciones y reintentos.
 */

#include <msp430.h>
#include <stdbool.h>
#include <stdint.h>
#include <drivers/buzzer.h>
#include <own_libraries/enlace_juego.h>
#include <own_libraries/protocolo_partida.h>
#include <own_libraries/tablero_juego.h>

static bool recibir_trama_timeout(uint8_t trama[5], uint16_t timeout_ms)
{
    while (timeout_ms--)
    {
        if (ENLACE_recibir_trama(trama))
            return true;
        BUZ_wait_ms(1);
    }
    return false;
}

void PROTOCOLO_iniciar(uint8_t jugador)
{
    uint8_t trama[5];

    if (jugador == 0)
    {
        while (1)
        {
            if (recibir_trama_timeout(trama, 1000) &&
                trama[1] == CMD_ACK_LISTO && trama[2] == 1)
                return;

            ENLACE_enviar_listo(0);
        }
    }

    while (1)
    {
        if (recibir_trama_timeout(trama, 1000) &&
            trama[1] == CMD_LISTO && trama[2] == 0)
        {
            ENLACE_enviar_ack_listo(1);
            return;
        }
    }
}

/* Convierte el disparo recibido contra la unica flota almacenada en esta placa. */

static uint8_t responder_disparo(uint8_t trama[5],
                                 uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                 protocolo_partida_estado_t *estado)
{
    uint8_t seq = trama[3];
    uint8_t casilla = trama[4];
    uint8_t resultado;

    if (estado->hay_anterior && seq == estado->secuencia_anterior &&
        casilla == estado->casilla_anterior)
        resultado = estado->resultado_anterior; /* reenvio de una peticion repetida */
    else if (casilla < FILAS_TABLERO * COLS_TABLERO)
    {
        resultado = TABLERO_resolver_disparo(flota, casilla);
        if (resultado != RESULTADO_INVALIDO)
        {
            estado->hay_anterior = true;
            estado->secuencia_anterior = seq;
            estado->casilla_anterior = casilla;
            estado->resultado_anterior = resultado;
        }
    }
    else
        resultado = RESULTADO_INVALIDO;

    return resultado;
}

static void esperar_ack_resultado(uint8_t jugador, uint8_t seq,
                                  uint8_t casilla, uint8_t resultado,
                                  uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                  protocolo_partida_estado_t *estado)
{
    uint8_t trama[5];

    while (1)
    {
        ENLACE_enviar_resultado(jugador, seq, resultado);
        if (!recibir_trama_timeout(trama, 1000))
            continue;

        if (trama[1] == CMD_ACK_RESULTADO && trama[2] == jugador && trama[3] == seq)
            return;

        if (trama[1] == CMD_DISPARO_RX && trama[2] == jugador &&
            trama[3] == seq && trama[4] == casilla)
        {
            /* La peticion original se repitio: el resultado se reenviara arriba. */
            responder_disparo(trama, flota, estado);
        }
    }
}

void PROTOCOLO_esperar_disparo_rival(uint8_t jugador,
                                  uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                  protocolo_partida_estado_t *estado,
                                  uint8_t *casilla_recibida,
                                  uint8_t *resultado_recibido)
{
    uint8_t trama[5];
    while (1)
    {
        if (!ENLACE_recibir_trama(trama))
        {
            BUZ_wait_ms(1);
            continue;
        }

        if (trama[1] == CMD_LISTO && trama[2] == 0 && jugador == 1)
        {
            ENLACE_enviar_ack_listo(1); /* ACK perdido: repetir */
            continue;
        }

        if (trama[1] == CMD_RESULTADO && trama[2] == (uint8_t)(1 - jugador) &&
            estado->hay_anterior && trama[3] == estado->secuencia_anterior)
        {
            ENLACE_enviar_ack_resultado(trama[2], trama[3]);
            continue;
        }

        if (trama[1] == CMD_DISPARO_RX && trama[2] == (uint8_t)(1 - jugador) &&
            trama[4] < FILAS_TABLERO * COLS_TABLERO)
        {
            uint8_t resultado;
            bool nuevo = !estado->hay_anterior ||
                         trama[3] != estado->secuencia_anterior ||
                         trama[4] != estado->casilla_anterior;
            resultado = responder_disparo(trama, flota, estado);
            esperar_ack_resultado(trama[2], trama[3], trama[4], resultado, flota,
                                  estado);
            if (!nuevo || resultado == RESULTADO_INVALIDO)
                continue;

            *casilla_recibida = trama[4];
            *resultado_recibido = resultado;
            return;
        }
    }
}

/* Al ganar, sigue confirmando el resultado por si se perdio el primer ACK. */
void PROTOCOLO_confirmar_resultado_ganador(uint8_t jugador)
{
    uint8_t trama[5];
    if (ENLACE_recibir_trama(trama) && trama[1] == CMD_RESULTADO &&
        trama[2] == jugador)
        ENLACE_enviar_ack_resultado(jugador, trama[3]);
}

uint8_t PROTOCOLO_enviar_disparo_y_esperar(uint8_t jugador, uint8_t secuencia,
                                        uint8_t casilla,
                                        uint8_t flota[FILAS_TABLERO][COLS_TABLERO],
                                        protocolo_partida_estado_t *estado)
{
    uint8_t trama[5];

    while (1)
    {
        ENLACE_enviar_disparo(jugador, secuencia, casilla);
        if (!recibir_trama_timeout(trama, 1000))
            continue; /* no hubo respuesta: repetir el mismo disparo */

        if (trama[1] == CMD_RESULTADO && trama[2] == jugador &&
            trama[3] == secuencia && trama[4] <= 2 * NUM_BARCOS)
        {
            ENLACE_enviar_ack_resultado(jugador, secuencia);
            return trama[4];
        }

        /* Una peticion anterior repetida se contesta aunque estemos jugando. */
        if (trama[1] == CMD_DISPARO_RX && trama[2] == (uint8_t)(1 - jugador) &&
            trama[4] < FILAS_TABLERO * COLS_TABLERO)
        {
            uint8_t resultado_rival = responder_disparo(trama, flota, estado);
            esperar_ack_resultado(trama[2], trama[3], trama[4], resultado_rival,
                                  flota, estado);
        }
    }
}
