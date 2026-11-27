#include <msp430.h>
#include <driverlib.h>
#include "hal_uart_cc.h"

#define UART_CC_RX_BUFFER_SIZE 64

static volatile uint8_t rx_buffer[UART_CC_RX_BUFFER_SIZE];
static volatile uint8_t rx_head;
static volatile uint8_t rx_tail;

void HAL_UART_CC_init_gpio(void)
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P6, GPIO_PIN0,
                                                 GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1,
                                                GPIO_PRIMARY_MODULE_FUNCTION);
}

void HAL_UART_CC_config(void)
{
    EUSCI_A_UART_initParam config = {0};
    config.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    config.clockPrescalar = 8;
    config.firstModReg = 10;
    config.secondModReg = 247;
    config.parity = EUSCI_A_UART_NO_PARITY;
    config.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    config.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    config.uartMode = EUSCI_A_UART_MODE;
    config.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    EUSCI_A_UART_init(EUSCI_A3_BASE, &config);
    EUSCI_A_UART_enable(EUSCI_A3_BASE);
    EUSCI_A_UART_enableInterrupt(EUSCI_A3_BASE,
                                 EUSCI_A_UART_RECEIVE_INTERRUPT);
}

bool HAL_UART_CC_data_available(void)
{
    return rx_head != rx_tail;
}

uint8_t HAL_UART_CC_rx_byte(void)
{
    while (!HAL_UART_CC_data_available()) { }
    uint8_t byte = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) & (UART_CC_RX_BUFFER_SIZE - 1);
    return byte;
}

void HAL_UART_CC_tx_byte(uint8_t byte)
{
    while (!(UCA3IFG & UCTXIFG)) { }
    EUSCI_A_UART_transmitData(EUSCI_A3_BASE, byte);
}

#pragma vector=EUSCI_A3_VECTOR
__interrupt void USCI_A3_ISR(void)
{
    if (EUSCI_A_UART_getInterruptStatus(EUSCI_A3_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
    {
        uint8_t byte = EUSCI_A_UART_receiveData(EUSCI_A3_BASE);
        uint8_t next = (rx_head + 1) & (UART_CC_RX_BUFFER_SIZE - 1);
        if (next != rx_tail)
        {
            rx_buffer[rx_head] = byte;
            rx_head = next;
        }
        EUSCI_A_UART_clearInterrupt(EUSCI_A3_BASE,
                                    EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
}
