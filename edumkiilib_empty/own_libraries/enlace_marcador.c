/*
 * enlace_marcador.c
 *
 * Envio de datos al dispositivo del marcador. Ver enlace_marcador.h.
 */

#include "enlace_marcador.h"
#include <hal/hal_uart.h>

static void enviar_trama(uint8_t comando, uint8_t jugador,
                         uint8_t resultado, uint8_t barco)
{
#if MARCADOR_ENVIO_ACTIVO
    // TODO: cambiar HAL_UART_tx_byte por la UART elegida (o la antena)
    HAL_UART_tx_byte(MARCADOR_INICIO_TRAMA);
    HAL_UART_tx_byte(comando);
    HAL_UART_tx_byte(jugador);
    HAL_UART_tx_byte(resultado);
    HAL_UART_tx_byte(barco);
#else
    /* Envio desactivado: el marcador aun no esta conectado */
    (void)comando;
    (void)jugador;
    (void)resultado;
    (void)barco;
#endif
}

void MARCADOR_enviar_inicio(void)
{
    enviar_trama(CMD_INICIO, 0, 0, BARCO_NINGUNO);
}

void MARCADOR_enviar_turno(uint8_t jugador)
{
    enviar_trama(CMD_TURNO, jugador, 0, BARCO_NINGUNO);
}

void MARCADOR_enviar_disparo(uint8_t jugador, uint8_t resultado, uint8_t barco)
{
    enviar_trama(CMD_DISPARO, jugador, resultado, barco);
}

void MARCADOR_enviar_fin(uint8_t ganador)
{
    enviar_trama(CMD_FIN, ganador, 0, BARCO_NINGUNO);
}
