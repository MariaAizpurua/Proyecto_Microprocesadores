#include <driverlib.h>
#include <msp430.h>
#include <hal/hal_pwm.h>

uint16_t current_red;
uint16_t current_green;
uint16_t current_blue;
uint16_t current_freq;
uint16_t current_tb0_period;

void HAL_PWM_init_gpio()
{
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); // EDUMKII-LED-R (TB0.5)
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION); // EDUMKII-LED-G (TB0.4)
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN4, GPIO_PRIMARY_MODULE_FUNCTION); // EDUMKII-LED-B (TB0.3)
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION); // EDUMKII-BUZZER (TB0.6)
}

void HAL_PWM_config()
{
    current_red = 0;
    current_green = 0;
    current_blue = 0;
    current_freq = 0;
    HAL_PWM_update_tb0();
}

void HAL_PWM_update_led_rgb(uint8_t r,uint8_t g,uint8_t b)
{
    current_red = r;
    current_green = g;
    current_blue = b;

    HAL_PWM_update_tb0();
}

void HAL_PWM_update_buzzer_freq(int freq)
{
    current_freq = freq;

    HAL_PWM_update_tb0();
}

void HAL_PWM_update_tb0()
{
    // TB0 period: 255 with the buzzer off; 16 MHz / freq while it sounds
    current_tb0_period = 255;
    if (current_freq != 0)
    {
        current_tb0_period = 16000000UL / current_freq;
    }

    // Use driverlib for RED LED PWM configuration
    // (duty rescaled from the 0-255 range to the current period)
    Timer_B_outputPWMParam PwmRedConfig =
    {
        TIMER_A_CLOCKSOURCE_SMCLK,              // clockSource
        TIMER_A_CLOCKSOURCE_DIVIDER_1,          // clockSourceDivider
        current_tb0_period,                     // timerPeriod
        TIMER_B_CAPTURECOMPARE_REGISTER_5,      // timer compareRegister
        TIMER_B_OUTPUTMODE_RESET_SET,           // compareOutputMode
        (uint16_t)((uint32_t)current_red * current_tb0_period / 255)   // dutyCycle
    };
    Timer_B_outputPWM(TIMER_B0_BASE, &PwmRedConfig);

    // Add extra configuration for GREEN and BLUE at register level
    TB0CCTL4 = TIMER_B_OUTPUTMODE_RESET_SET;
    TB0CCR4 = (uint16_t)((uint32_t)current_green * current_tb0_period / 255);
    TB0CCTL3 = TIMER_B_OUTPUTMODE_RESET_SET;
    TB0CCR3 = (uint16_t)((uint32_t)current_blue * current_tb0_period / 255);

    // Buzzer on CCR6: 50% duty while sounding, 0 when silent
    TB0CCTL6 = TIMER_B_OUTPUTMODE_RESET_SET;
    TB0CCR6 = 0;
    if (current_freq != 0)
    {
        TB0CCR6 = current_tb0_period / 2;
    }
}
