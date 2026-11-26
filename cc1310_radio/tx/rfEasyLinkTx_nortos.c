/*
 * rfEasyLinkTx_nortos.c
 * Prueba de transmision UART a radio para CC1310.
 */

#include <stdbool.h>
#include <stdint.h>

#include <ti/drivers/UART.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "Board.h"
#include "easylink/EasyLink.h"

#define RADIO_FREQUENCY_HZ  864500000UL
#define FRAME_START        0xAA
#define FRAME_SIZE         5
#define RADIO_DESTINATION  0xAA

static PIN_Handle pinHandle;
static PIN_State pinState;

static PIN_Config pinTable[] = {
    Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_PIN_LED2 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

static void stop_on_error(void)
{
    while (1) { }
}

static bool frame_is_valid(const uint8_t frame[FRAME_SIZE])
{
    if (frame[0] != FRAME_START || frame[2] > 1)
        return false;

    switch (frame[1])
    {
    case 'I':
    case 'T':
    case 'F':
        return true;

    case 'D':
        if (frame[3] == 0)
            return frame[4] == 0xFF;
        return (frame[3] == 1 || frame[3] == 2) && frame[4] < 5;

    default:
        return false;
    }
}

static void radio_send_frame(const uint8_t frame[FRAME_SIZE])
{
    EasyLink_TxPacket packet = {0};

    packet.len = FRAME_SIZE;
    packet.dstAddr[0] = RADIO_DESTINATION;
    packet.absTime = 0;

    packet.payload[0] = frame[0];
    packet.payload[1] = frame[1];
    packet.payload[2] = frame[2];
    packet.payload[3] = frame[3];
    packet.payload[4] = frame[4];

    if (EasyLink_transmit(&packet) != EasyLink_Status_Success)
        stop_on_error();

    /* Green LED latches on after the first successful radio transmission. */
    PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);
}

/*
 * Bridge TX: MSP430 UCA3 (P6.0 TX) -> CC1310 UART RX (DIO2) -> 864.5 MHz.
 * This replaces the SDK rfEasyLinkTx_nortos.c in the copied TX example.
 */
void *mainThread(void *arg0)
{
    UART_Handle uart;
    UART_Params uartParams;
    EasyLink_Params radioParams;
    uint8_t frame[FRAME_SIZE];
    uint8_t position = 0;

    (void)arg0;

    pinHandle = PIN_open(&pinState, pinTable);
    if (pinHandle == NULL)
        stop_on_error();

    /* Red LED: TX firmware entered mainThread and opened the LED GPIOs. */
    PIN_setOutputValue(pinHandle, Board_PIN_LED2, 1);

    UART_init();
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;

    uart = UART_open(Board_UART0, &uartParams);
    if (uart == NULL)
        stop_on_error();

    EasyLink_Params_init(&radioParams);
    if (EasyLink_init(&radioParams) != EasyLink_Status_Success)
        stop_on_error();
    if (EasyLink_setFrequency(RADIO_FREQUENCY_HZ) != EasyLink_Status_Success)
        stop_on_error();

    while (1)
    {
        uint8_t byte;

        if (UART_read(uart, &byte, 1) != 1)
            continue;

        /* Resynchronize on AA if a byte was lost or the MSP reset. */
        if (byte == FRAME_START)
        {
            frame[0] = byte;
            position = 1;
            continue;
        }

        if (position == 0)
            continue;

        frame[position++] = byte;
        if (position == FRAME_SIZE)
        {
            if (frame_is_valid(frame))
                radio_send_frame(frame);
            position = 0;
        }
    }
}
