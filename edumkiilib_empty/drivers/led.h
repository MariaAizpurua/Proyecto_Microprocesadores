#ifndef DRIVERS_LED_H_
#define DRIVERS_LED_H_

#include <stdint.h>
#include <stdbool.h>

#define CYCLE_OFF   0
#define CYCLE_RED   1
#define CYCLE_GREEN 2
#define CYCLE_BLUE  3

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Sets the EDUMKII RGB LED color with per-channel intensity.
//!
//! PWM variant: each channel accepts an intensity from 0 (off) to 255
//! (maximum), generated in hardware with Timer B0 (PWM HAL).
//!
//! \param r red channel intensity (0-255).
//! \param g green channel intensity (0-255).
//! \param b blue channel intensity (0-255).
//!
//! \return None.
//
//*****************************************************************************
void LED_set_rgb(uint8_t r,uint8_t g,uint8_t b);

//*****************************************************************************
//
//! \brief Advances the RGB LED through the red -> green -> blue sequence.
//!
//! Each call moves to the next color of the cycle. Useful as a visual
//! heartbeat.
//!
//! \return None.
//
//*****************************************************************************
void LED_cycle_rgb();

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_LED_H_ */
