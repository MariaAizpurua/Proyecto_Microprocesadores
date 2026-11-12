/*
 * Empty C template with edumkiilib - MSP430FR5994 + EDUMKII BoosterPack
 *
 * Complete, working initialization of every HAL and BoosterPack device
 * (with the unified hal/adc/all and hal/pwm/all variants, so joystick +
 * accelerometer and RGB + buzzer can all be used together), plus an empty
 * main loop for your application.
 *
 * Note: the digital output HAL (LaunchPad LEDs + RGB) shares pins
 * P3.4-P3.6 with the PWM HAL. Its init runs BEFORE the PWM one, so the
 * PWM muxing wins on those pins: use LED_set_rgb() (PWM) for the RGB LED
 * and HAL_DIGOUT_set_LED1/2() for the LaunchPad LEDs only.
 */

// General libraries
#include <msp430.h>
#include <driverlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// GRLIB
#include <grlib.h>

// HAL headers
#include <hal/hal_lcd.h>
#include <hal/hal_i2c.h>
#include <hal/hal_digital_input.h>
#include <hal/hal_digital_output.h>
#include <hal/hal_uart.h>
#include <hal/hal_adc.h>
#include <hal/hal_pwm.h>

// Driver headers
#include <drivers/crystalfontz.h>
#include <drivers/joystick.h>
#include <drivers/kxtc9_2050.h>
#include <drivers/switch.h>
#include <drivers/tmp006.h>
#include <drivers/opt3001.h>
#include <drivers/led.h>
#include <drivers/buzzer.h>
#include <drivers/cli.h>

// Global variables
Graphics_Context g_sContext;

static void Clocks_init()
{
    // Increase FRAM access wait time. Needed to work properly at 16MHz
    FRAMCtl_A_configureWaitStateControl(FRAMCTL_A_ACCESS_TIME_CYCLES_1);

    // Set DCO at 16 MHz
    CS_setDCOFreq(CS_DCORSEL_1, CS_DCOFSEL_4);
    CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    CS_initClockSignal(CS_ACLK, CS_VLOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

void main(void)
{
    // Stop WDT
    WDT_A_hold(WDT_A_BASE);

    // Initialize clocks
    Clocks_init();

    // Initialize HAL GPIOs
    HAL_LCD_init_gpio();
    HAL_I2C_init_gpio();
    HAL_DIGIN_init_gpio();
    HAL_DIGOUT_init_gpio();
    HAL_UART_init_gpio();
    HAL_ADC_init_gpio();
    HAL_PWM_init_gpio();        // last on purpose: wins P3.4-P3.7 muxing

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PMM_unlockLPM5();

    // Configure HAL modules
    HAL_LCD_config();
    HAL_I2C_config();
    HAL_DIGIN_config();
    HAL_DIGOUT_config();
    HAL_UART_config();
    HAL_ADC_config();
    HAL_PWM_config();

    // Enable global interrupts
    __bis_SR_register(GIE);

    // Initialize and configure external devices
    CRYSTALFONTZ_init();
    CRYSTALFONTZ_set_orientation(LCD_ORIENTATION_UP);
    Graphics_initContext(&g_sContext, &g_sCRYSTALFONTZ);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLUE);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontCm12);
    Graphics_clearDisplay(&g_sContext);

    TMP006_config();
    OPT3001_config();

    while (1)
    {

    }
}
