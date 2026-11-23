/*
 * enlace_marcador.c
 *
 * Envio de datos al dispositivo del marcador. Ver enlace_marcador.h.
 */

#include "enlace_marcador.h"
#include <hal/hal_uart_cc.h>
#include "pantalla_marcador.h"

static void enviar_trama(uint8_t comando, uint8_t jugador,
                         uint8_t resultado, uint8_t barco)
{
#if MARCADOR_ENVIO_ACTIVO
    HAL_UART_CC_tx_byte(MARCADOR_INICIO_TRAMA);
    HAL_UART_CC_tx_byte(comando);
    HAL_UART_CC_tx_byte(jugador);
    HAL_UART_CC_tx_byte(resultado);
    HAL_UART_CC_tx_byte(barco);
#else
    /* Envio desactivado: el marcador aun no esta conectado */
    (void)comando;
    (void)jugador;
    (void)resultado;
    (void)barco;
#endif
}

void MARCADOR_receptor_procesar(void)
{
    static uint8_t trama[5];
    static uint8_t posicion;

    while (HAL_UART_CC_data_available())
    {
        uint8_t byte = HAL_UART_CC_rx_byte();
        if (posicion == 0)
        {
            if (byte == MARCADOR_INICIO_TRAMA)
                trama[posicion++] = byte;
            continue;
        }

        trama[posicion++] = byte;
        if (posicion == sizeof(trama))
        {
            uint8_t comando = trama[1];
            uint8_t jugador = trama[2];
            uint8_t resultado = trama[3];
            uint8_t barco = trama[4];

            if (jugador < 2)
            {
                switch (comando)
                {
                case CMD_INICIO:
                    marcador();
                    break;
                case CMD_TURNO:
                    dibujar_turno(jugador);
                    break;
                case CMD_DISPARO:
                    if (resultado == RES_AGUA)
                        registrar_contador(jugador, 2);
                    else if ((resultado == RES_TOCADO || resultado == RES_HUNDIDO)
                             && barco < 5)
                        barco_tocado(jugador, barco);
                    break;
                case CMD_FIN:
                    /* La pantalla actual no tiene una vista de fin de partida. */
                    break;
                default:
                    break;
                }
            }
            posicion = 0;
        }
    }
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
