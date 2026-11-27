#ifndef HAL_UART_CC_H_
#define HAL_UART_CC_H_

#include <stdbool.h>
#include <stdint.h>

/* UART dedicada al CC1310: UCA3, TX=P6.0 y RX=P6.1, 115200 8N1. */
void HAL_UART_CC_init_gpio(void);
void HAL_UART_CC_config(void);
bool HAL_UART_CC_data_available(void);
uint8_t HAL_UART_CC_rx_byte(void);
void HAL_UART_CC_tx_byte(uint8_t byte);

#endif
