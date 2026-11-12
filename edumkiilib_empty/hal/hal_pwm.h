#ifndef HAL_HAL_PWM_H_
#define HAL_HAL_PWM_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//*****************************************************************************
//
//! \brief Configures the RGB LED and buzzer pins as PWM outputs.
//!
//! Unified variant of hal/pwm/rgb and hal/pwm/buzzer on Timer B0:
//! R=P3.6/TB0.5, G=P3.5/TB0.4, B=P3.4/TB0.3 and buzzer=P3.7/TB0.6
//! (primary function). Call BEFORE PMM_unlockLPM5().
//!
//! \return None.
//
//*****************************************************************************
void HAL_PWM_init_gpio();

//*****************************************************************************
//
//! \brief Initializes Timer B0 with the LED off and the buzzer silent.
//!
//! Call AFTER PMM_unlockLPM5().
//!
//! \return None.
//
//*****************************************************************************
void HAL_PWM_config();

//*****************************************************************************
//
//! \brief Makes the buzzer sound at the given frequency.
//!
//! TB0 has only ONE period register (CCR0): while the buzzer is active the
//! period becomes 16 MHz / freq and the RGB duties are RESCALED so the color
//! does not change. With freq = 0 the buzzer goes silent and the period
//! returns to 255.
//!
//! \param freq frequency in Hz (0 = silence; minimum ~250 Hz due to the
//!        16 bits of CCR0).
//!
//! \return None.
//
//*****************************************************************************
void HAL_PWM_update_buzzer_freq(int freq);

//*****************************************************************************
//
//! \brief Sets the intensity of each RGB LED channel.
//!
//! The 0-255 values are interpreted as a FRACTION of the current TB0 period,
//! so the color does not change even when the buzzer modifies the period.
//!
//! \param r red channel intensity (0-255).
//! \param g green channel intensity (0-255).
//! \param b blue channel intensity (0-255).
//!
//! \return None.
//
//*****************************************************************************
void HAL_PWM_update_led_rgb(uint8_t r,uint8_t g,uint8_t b);

//*****************************************************************************
//
//! \brief Rewrites the complete Timer B0 configuration.
//!
//! Single writer of TB0 (period + 4 coherent duties); the RGB and buzzer
//! functions only update their state and delegate here. Internal HAL
//! function.
//!
//! \return None.
//
//*****************************************************************************
void HAL_PWM_update_tb0();

#ifdef __cplusplus
}
#endif

#endif /* HAL_HAL_PWM_H_ */
