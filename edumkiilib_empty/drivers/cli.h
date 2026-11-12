#ifndef DRIVERS_CLI_H_
#define DRIVERS_CLI_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Checks whether a received byte is pending to be read from the CLI.
//!
//! The command line interface (CLI) uses the UART HAL (eUSCI_A0, USB
//! backchannel of the LaunchPad), which buffers reception in a ring buffer.
//!
//! \return true if a byte is available; false otherwise.
//
//*****************************************************************************
bool CLI_data_available(void);

//*****************************************************************************
//
//! \brief Reads the next byte received by the CLI.
//!
//! Blocks until a byte is available; call CLI_data_available() first for
//! non-blocking use.
//!
//! \return The received byte.
//
//*****************************************************************************
uint8_t CLI_rx_byte(void);

//*****************************************************************************
//
//! \brief Receives a given number of bytes into an array (blocking).
//!
//! Returns once exactly len bytes have been read.
//!
//! \param data destination buffer.
//! \param len number of bytes to receive.
//!
//! \return None.
//
//*****************************************************************************
void CLI_rx_array(uint8_t *data, uint16_t len);

//*****************************************************************************
//
//! \brief Receives a text line (until CR or LF) into a string (blocking).
//!
//! Reads characters until Enter is detected or maxlen-1 characters have
//! been stored. The terminator is not stored; the result is always
//! null-terminated.
//!
//! \param str destination buffer.
//! \param maxlen buffer size (including the null terminator).
//!
//! \return Number of characters stored (not counting the null terminator).
//
//*****************************************************************************
uint16_t CLI_rx_string(char *str, uint16_t maxlen);

//*****************************************************************************
//
//! \brief Transmits one byte over the CLI (UART).
//!
//! \param txByte is the byte to transmit.
//!
//! \return None.
//
//*****************************************************************************
void CLI_tx_byte(uint8_t txByte);

//*****************************************************************************
//
//! \brief Transmits a given number of bytes from an array.
//!
//! \param data source buffer.
//! \param len number of bytes to transmit.
//!
//! \return None.
//
//*****************************************************************************
void CLI_tx_array(const uint8_t *data, uint16_t len);

//*****************************************************************************
//
//! \brief Transmits a null-terminated string (the 0x00 is not sent).
//!
//! \param str the string to transmit.
//!
//! \return None.
//
//*****************************************************************************
void CLI_tx_string(const char *str);

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_CLI_H_ */
