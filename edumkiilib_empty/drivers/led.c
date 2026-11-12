#include <hal/hal_pwm.h>
#include <drivers/led.h>

void LED_set_rgb(uint8_t r,uint8_t g,uint8_t b)
{
    HAL_PWM_update_led_rgb(r, g, b);
}

uint8_t cycle_status = CYCLE_OFF;
void LED_cycle_rgb()
{
    switch(cycle_status)
    {
    case CYCLE_OFF:
        cycle_status = CYCLE_RED;
        LED_set_rgb(255,0,0);
        break;
    case CYCLE_RED:
        cycle_status = CYCLE_GREEN;
        LED_set_rgb(0,255,0);
        break;
    case CYCLE_GREEN:
        cycle_status = CYCLE_BLUE;
        LED_set_rgb(0,0,255);
        break;
    case CYCLE_BLUE:
        cycle_status = CYCLE_RED;
        LED_set_rgb(255,0,0);
        break;
    }
}
