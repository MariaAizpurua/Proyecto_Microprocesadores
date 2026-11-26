/*
 * rfEasyLinkTrx_nortos.c
 * Puente bidireccional UART y radio para CC1310.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include <ti/drivers/UART.h>
#include <ti/drivers/pin/PINCC26XX.h>
#include <ti/drivers/dpl/ClockP.h>

#include "Board.h"
#include "easylink/EasyLink.h"

#define RADIO_FREQUENCY_HZ  864500000UL
#define RADIO_ADDRESS       0xAA        /* igual en las dos placas */
#define FRAME_START         0xAA
#define FRAME_SIZE          5

#define UART_ESPERA_US      2000        /* cada cuanto se revisa la radio */
#define HUECOS_RESYNC       5           /* 5 x 2 ms sin bytes = trama rota */
#define COLA_RX             8           /* tramas de radio pendientes */

#define LED_VERDE  Board_PIN_LED1
#define LED_ROJO   Board_PIN_LED2

static PIN_Handle pinHandle;
static PIN_State pinState;

static PIN_Config pinTable[] = {
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_PIN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

/* Cola de tramas recibidas por radio (la rellena el callback) */
static uint8_t cola_rx[COLA_RX][FRAME_SIZE];
static volatile uint8_t cola_cabeza;
static volatile uint8_t cola_cola;
static volatile bool escuchando;

static void toggle(PIN_Id led)
{
    PIN_setOutputValue(pinHandle, led, !PIN_getOutputValue(led));
}

static void error_config(void)
{
    volatile uint32_t i;
    PIN_setOutputValue(pinHandle, LED_VERDE, 0);
    while (1)
    {
        toggle(LED_ROJO);
        for (i = 0; i < 300000; i++) { }
    }
}

/* Lo llama EasyLink cuando termina una escucha (llegue algo o no) */
static void radio_rx_hecho(EasyLink_RxPacket *paquete, EasyLink_Status estado)
{
    escuchando = false;

    if (estado == EasyLink_Status_Success &&
        paquete->len == FRAME_SIZE && paquete->payload[0] == FRAME_START)
    {
        uint8_t siguiente = (cola_cabeza + 1) % COLA_RX;
        if (siguiente != cola_cola)             /* si la cola esta llena se pierde */
        {
            memcpy(cola_rx[cola_cabeza], paquete->payload, FRAME_SIZE);
            cola_cabeza = siguiente;
        }
    }
}

static void radio_escuchar(void)
{
    if (escuchando)
        return;

    escuchando = true;
    if (EasyLink_receiveAsync(radio_rx_hecho, 0) != EasyLink_Status_Success)
        escuchando = false;                     /* se reintenta en la siguiente vuelta */
}

static void radio_enviar(const uint8_t trama[FRAME_SIZE])
{
    EasyLink_TxPacket paquete = {0};

    /* La radio no puede escuchar y transmitir a la vez: se corta la escucha */
    EasyLink_abort();
    escuchando = false;

    paquete.len = FRAME_SIZE;
    paquete.dstAddr[0] = RADIO_ADDRESS;
    paquete.absTime = 0;
    memcpy(paquete.payload, trama, FRAME_SIZE);

    if (EasyLink_transmit(&paquete) == EasyLink_Status_Success)
        toggle(LED_ROJO);

    radio_escuchar();
}

void *mainThread(void *arg0)
{
    UART_Handle uart;
    UART_Params uartParams;
    EasyLink_Params radioParams;
    uint8_t trama[FRAME_SIZE];
    uint8_t posicion = 0;
    uint8_t huecos = 0;

    (void)arg0;

    pinHandle = PIN_open(&pinState, pinTable);
    if (pinHandle == NULL)
        while (1) { }

    UART_init();
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
    uartParams.readTimeout = UART_ESPERA_US / ClockP_tickPeriod;

    uart = UART_open(Board_UART0, &uartParams);
    if (uart == NULL)
        error_config();

    EasyLink_Params_init(&radioParams);
    if (EasyLink_init(&radioParams) != EasyLink_Status_Success)
        error_config();
    if (EasyLink_setFrequency(RADIO_FREQUENCY_HZ) != EasyLink_Status_Success)
        error_config();

    /* Radio lista */
    PIN_setOutputValue(pinHandle, LED_VERDE, 1);

    while (1)
    {
        uint8_t byte;

        radio_escuchar();

        /* 1) Radio -> MSP430 */
        while (cola_cola != cola_cabeza)
        {
            UART_write(uart, cola_rx[cola_cola], FRAME_SIZE);
            cola_cola = (cola_cola + 1) % COLA_RX;
            toggle(LED_VERDE);
        }

        /* 2) MSP430 -> radio (espera como mucho UART_ESPERA_US) */
        if (UART_read(uart, &byte, 1) != 1)
        {
            /* Si una trama se queda a medias, se descarta */
            if (posicion > 0 && ++huecos >= HUECOS_RESYNC)
                posicion = 0;
            continue;
        }
        huecos = 0;

        if (posicion == 0)
        {
            if (byte == FRAME_START)
                trama[posicion++] = byte;
            continue;
        }

        trama[posicion++] = byte;
        if (posicion == FRAME_SIZE)
        {
            posicion = 0;
            radio_enviar(trama);
        }
    }
}
