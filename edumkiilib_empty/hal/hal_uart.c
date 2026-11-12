#include <msp430.h>
#include <driverlib.h>
#include <hal/hal_uart.h>

// RX ring buffer: the ISR pushes, the application pops. Without it, any
// burst faster than the main loop polling would silently lose bytes
// (the previous implementation latched only the LAST received byte).
#define UART_RX_BUFFER_SIZE 64      // power of two (mask-based wrap)
static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint8_t rx_head = 0;    // ISR writes here
static volatile uint8_t rx_tail = 0;    // application reads here

void HAL_UART_init_gpio()
{
    // Select UART function for TX (P2.0 = UCA0TXD)
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P2,
            GPIO_PIN0,
            GPIO_SECONDARY_MODULE_FUNCTION);

    // Select UART function for RX (P2.1 = UCA0RXD)
    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P2,
            GPIO_PIN1,
            GPIO_SECONDARY_MODULE_FUNCTION);
}


void HAL_UART_config(void)
{
    EUSCI_A_UART_initParam uartConfig = {0};
    uartConfig.selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
    uartConfig.clockPrescalar = 8;      // 115200 baud with SMCLK = 16 MHz
    uartConfig.firstModReg = 10;
    uartConfig.secondModReg = 247;
    uartConfig.parity = EUSCI_A_UART_NO_PARITY;
    uartConfig.msborLsbFirst = EUSCI_A_UART_LSB_FIRST;
    uartConfig.numberofStopBits = EUSCI_A_UART_ONE_STOP_BIT;
    uartConfig.uartMode = EUSCI_A_UART_MODE;
    uartConfig.overSampling = EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION;

    // Initialize USCI_A0 as UART
    EUSCI_A_UART_init(EUSCI_A0_BASE, &uartConfig);

    // Enable UART module
    EUSCI_A_UART_enable(EUSCI_A0_BASE);

    // Enable RX interrupt
    EUSCI_A_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
}

bool HAL_UART_data_available(void)
{
    return rx_head != rx_tail;
}

uint8_t HAL_UART_rx_byte(void)
{
    // Blocks until a byte is available (check HAL_UART_data_available()
    // first for non-blocking use)
    while (rx_head == rx_tail);

    uint8_t data = rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) & (UART_RX_BUFFER_SIZE - 1);
    return data;
}

void HAL_UART_tx_byte(uint8_t txByte)
{
    EUSCI_A_UART_transmitData(EUSCI_A0_BASE, txByte);
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
    uint8_t status = EUSCI_A_UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);

    if (status == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        uint8_t next = (rx_head + 1) & (UART_RX_BUFFER_SIZE - 1);
        uint8_t data = EUSCI_A_UART_receiveData(EUSCI_A0_BASE);
        if (next != rx_tail)            // drop the byte if the buffer is full
        {
            rx_buffer[rx_head] = data;
            rx_head = next;
        }
        EUSCI_A_UART_clearInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
    }
}
