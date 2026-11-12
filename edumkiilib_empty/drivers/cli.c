#include <hal/hal_uart.h>
#include <drivers/cli.h>
#include <stdbool.h>
#include <stdint.h>

bool CLI_data_available(void)
{
    return HAL_UART_data_available();
}

uint8_t CLI_rx_byte(void)
{
    return HAL_UART_rx_byte();
}

void CLI_rx_array(uint8_t *data, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        data[i] = HAL_UART_rx_byte();
    }
}

uint16_t CLI_rx_string(char *str, uint16_t maxlen)
{
    uint16_t n = 0;

    while (n < maxlen - 1)
    {
        uint8_t c = HAL_UART_rx_byte();
        if (c == '\r' || c == '\n')
        {
            break;
        }
        str[n++] = (char)c;
    }
    str[n] = '\0';
    return n;
}

void CLI_tx_byte(uint8_t txByte)
{
    HAL_UART_tx_byte(txByte);
}

void CLI_tx_array(const uint8_t *data, uint16_t len)
{
    uint16_t i;
    for (i = 0; i < len; i++)
    {
        HAL_UART_tx_byte(data[i]);
    }
}

void CLI_tx_string(const char *str)
{
    while (*str != '\0')
    {
        HAL_UART_tx_byte((uint8_t)*str++);
    }
}
