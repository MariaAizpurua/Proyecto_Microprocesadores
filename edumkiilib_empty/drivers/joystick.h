#ifndef DRIVERS_JOYSTICK_H_
#define DRIVERS_JOYSTICK_H_

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Returns the horizontal joystick position as a percentage.
//!
//! Converts the latest ADC conversion (channel A2, refreshed by calling
//! HAL_ADC_trigger_single_conversion()) into a position: 0 at rest (VDD/2),
//! -100 at one end (0 V) and +100 at the other (VDD).
//!
//! \return X position in the range -100.0 .. +100.0.
//
//*****************************************************************************
float JOYSTICK_get_x();

//*****************************************************************************
//
//! \brief Returns the vertical joystick position as a percentage.
//!
//! Same as JOYSTICK_get_x() but for the vertical axis (channel A15).
//!
//! \return Y position in the range -100.0 .. +100.0.
//
//*****************************************************************************
float JOYSTICK_get_y();

//*****************************************************************************
//
//! \brief Reads the joystick center (SELECT) push button.
//!
//! \b Warning: declared in this header but NOT implemented in joystick.c;
//! using it produces a link error.
//!
//! \return true if pressed.
//
//*****************************************************************************
bool JOYSTICK_get_sel();

#ifdef __cplusplus
}
#endif

#endif /* DRIVERS_JOYSTICK_H_ */
