/*
 * rfEasyLinkRx_nortos.c
 * Prueba de recepcion radio a UART para CC1310.
 */

#include <stdint.h>
#include <unistd.h>

#include <ti/drivers/UART.h>
#include <ti/drivers/pin/PINCC26XX.h>

#include "Board.h"
#include "easylink/EasyLink.h"

#define RADIO_FREQUENCY_HZ  864500000UL
#define FRAME_START        0xAA
#define FRAME_SIZE         5

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

/*
 * Bridge RX: 864.5 MHz -> CC1310 UART TX (DIO3) -> MSP430 UCA3 RX (P6.1).
 * This replaces the SDK rfEasyLinkRx_nortos.c in the copied RX example.
 */
void *mainThread(void *arg0)
{
    UART_Handle uart;
    UART_Params uartParams;
    EasyLink_Params radioParams;
    EasyLink_RxPacket packet = {0};

    (void)arg0;

    pinHandle = PIN_open(&pinState, pinTable);
    if (pinHandle == NULL)
        stop_on_error();

    /* Green LED: RX firmware entered mainThread and opened the LED GPIOs. */
    PIN_setOutputValue(pinHandle, Board_PIN_LED1, 1);

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
        packet.absTime = 0;
        if (EasyLink_receive(&packet) == EasyLink_Status_Success &&
            packet.len == FRAME_SIZE && packet.payload[0] == FRAME_START)
        {
            /* Reenvia la trama de cinco bytes del protocolo del juego. */
            if (UART_write(uart, packet.payload, FRAME_SIZE) != FRAME_SIZE)
                stop_on_error();

            /* Red LED latches on after a valid RF frame reaches UART. */
            PIN_setOutputValue(pinHandle, Board_PIN_LED2, 1);
        }
    }
}
