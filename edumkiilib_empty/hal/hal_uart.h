#ifndef HAL_HAL_UART_H_
#define HAL_HAL_UART_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Configures the USB backchannel UART pins.
//!
//! P2.0 = UCA0TXD and P2.1 = UCA0RXD (secondary function). Call BEFORE
//! PMM_unlockLPM5().
//!
//! \return None.
//
//*****************************************************************************
void HAL_UART_init_gpio(void);

//*****************************************************************************
//
//! \brief Configures eUSCI_A0 as a 115200 baud UART (8N1).
//!
//! With SMCLK at 16 MHz (UCBR=8, UCBRF=10, UCBRS=247, oversampling) and the
//! receive interrupt enabled: the ISR stores the received byte and sets an
//! internal flag. Call AFTER PMM_unlockLPM5() and enable the global
//! interrupts (GIE) afterwards.
//!
//! \return None.
//
//*****************************************************************************
void HAL_UART_config(void);

//*****************************************************************************
//
//! \brief Checks whether a received byte is pending to be read.
//!
//! \return true if data is available.
//
//*****************************************************************************
bool HAL_UART_data_available(void);

//*****************************************************************************
//
//! \brief Returns the last received byte and clears the new-data flag.
//!
//! \return The received byte.
//
//*****************************************************************************
uint8_t HAL_UART_rx_byte(void);

//*****************************************************************************
//
//! \brief Transmits one byte over the UART (blocking).
//!
//! Waits until the transmit buffer is free and writes the byte.
//!
//! \param txByte is the byte to transmit.
//!
//! \return None.
//
//*****************************************************************************
void HAL_UART_tx_byte(uint8_t txByte);

#ifdef __cplusplus
}
#endif

#endif /* HAL_HAL_UART_H_ */
