/*
 * enlace_juego.c
 * Envio y recepcion UART de tramas de la partida.
 */

#include "enlace_juego.h"
#include <hal/hal_uart_cc.h>
#include <string.h>

bool ENLACE_recibir_trama(uint8_t trama_salida[5])
{
    static uint8_t trama[5];
    static uint8_t posicion;

    while (HAL_UART_CC_data_available())
    {
        uint8_t byte = HAL_UART_CC_rx_byte();
        if (posicion == 0)
        {
            if (byte == ENLACE_INICIO_TRAMA)
                trama[posicion++] = byte;
            continue;
        }

        /* En este protocolo los bytes de datos no pueden valer 0xAA. */
        if (byte == ENLACE_INICIO_TRAMA)
        {
            trama[0] = byte;
            posicion = 1;
            continue;
        }

        trama[posicion++] = byte;
        if (posicion == sizeof(trama))
        {
            memcpy(trama_salida, trama, sizeof(trama));
            posicion = 0;
            return true;
        }
    }

    return false;
}

static void enviar_trama_protocolo(uint8_t comando, uint8_t jugador,
                                   uint8_t dato1, uint8_t dato2)
{
#if ENLACE_ENVIO_ACTIVO
    HAL_UART_CC_tx_byte(ENLACE_INICIO_TRAMA);
    HAL_UART_CC_tx_byte(comando);
    HAL_UART_CC_tx_byte(jugador);
    HAL_UART_CC_tx_byte(dato1);
    HAL_UART_CC_tx_byte(dato2);
#else
    (void)comando;
    (void)jugador;
    (void)dato1;
    (void)dato2;
#endif
}

void ENLACE_enviar_listo(uint8_t jugador)
{
    enviar_trama_protocolo(CMD_LISTO, jugador, 0, 0);
}

void ENLACE_enviar_ack_listo(uint8_t jugador)
{
    enviar_trama_protocolo(CMD_ACK_LISTO, jugador, 0, 0);
}

void ENLACE_enviar_disparo(uint8_t jugador, uint8_t secuencia, uint8_t casilla)
{
    enviar_trama_protocolo(CMD_DISPARO_RX, jugador, secuencia, casilla);
}

void ENLACE_enviar_resultado(uint8_t jugador, uint8_t secuencia, uint8_t resultado)
{
    enviar_trama_protocolo(CMD_RESULTADO, jugador, secuencia, resultado);
}

void ENLACE_enviar_ack_resultado(uint8_t jugador, uint8_t secuencia)
{
    enviar_trama_protocolo(CMD_ACK_RESULTADO, jugador, secuencia, 0);
}
