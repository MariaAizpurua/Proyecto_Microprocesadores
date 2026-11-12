#ifndef HAL_ADC_H_
#define HAL_ADC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//! Latest conversion of the joystick X axis (channel A2, 0-4095).
extern uint16_t joystick_x_raw;
//! Latest conversion of the joystick Y axis (channel A15, 0-4095).
extern uint16_t joystick_y_raw;
//! Latest conversion of the accelerometer X axis (channel A12, 0-4095).
extern uint16_t acc_x_raw;
//! Latest conversion of the accelerometer Y axis (channel A13, 0-4095).
extern uint16_t acc_y_raw;
//! Latest conversion of the accelerometer Z axis (channel A14, 0-4095).
extern uint16_t acc_z_raw;

//*****************************************************************************
//
//! \brief Configures the joystick AND accelerometer pins as analog inputs.
//!
//! Unified variant of hal/adc/joystick and hal/adc/kxtc9_2050: joystick on
//! P1.2/A2 and P3.3/A15; accelerometer on P3.0/A12, P3.1/A13 and P3.2/A14
//! (ternary function). Call BEFORE PMM_unlockLPM5().
//!
//! \return None.
//
//*****************************************************************************
void HAL_ADC_init_gpio();

//*****************************************************************************
//
//! \brief Configures the ADC12_B in 5-channel sequence mode.
//!
//! MEM0=A2, MEM1=A15, MEM2=A12, MEM3=A13, MEM4=A14 (ENDOFSEQUENCE on MEM4,
//! interrupt IE4). The ISR stores the five results in the *_raw variables.
//! This lets the joystick and the accelerometer coexist in the same project
//! (the original HAL variants are mutually exclusive).
//! Call AFTER PMM_unlockLPM5() and enable GIE afterwards.
//!
//! \return None.
//
//*****************************************************************************
void HAL_ADC_config();

//*****************************************************************************
//
//! \brief Triggers one conversion of the full sequence and waits for the end.
//!
//! On return, the five *_raw variables hold fresh values.
//!
//! \return None.
//
//*****************************************************************************
void HAL_ADC_trigger_single_conversion();

//*****************************************************************************
//
//! \brief Starts automatically repeated conversions of the sequence.
//!
//! The *_raw variables are refreshed continuously via ISR.
//!
//! \return None.
//
//*****************************************************************************
void HAL_ADC_trigger_continuous_conversion();

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_H_ */
