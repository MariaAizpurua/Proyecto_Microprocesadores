#include <hal/hal_adc.h>
#include <hal/hal_digital_input.h>
#include <driverlib.h>
#include <stdbool.h>

static bool select_pressed = false;

float JOYSTICK_get_x()
{
    float joystick_x_percentage = joystick_x_raw/4095.0 * 200 - 100;
    return joystick_x_percentage;
}

float JOYSTICK_get_y()
{
    float joystick_y_percentage = joystick_y_raw/4095.0 * 200 - 100;
    return joystick_y_percentage;
}

bool JOYSTICK_get_sel()
{
    if (!select_pressed)
    {
        if (flag_edumkii_SEL)
        {
            __delay_cycles(160000);
            flag_edumkii_SEL = false;
            select_pressed = true;
            return true;
        }
    }
    else if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN2) == GPIO_INPUT_PIN_HIGH)
    {
        __delay_cycles(16000);
        flag_edumkii_SEL = false;
        if (GPIO_getInputPinValue(GPIO_PORT_P6, GPIO_PIN2) == GPIO_INPUT_PIN_HIGH)
        {
            select_pressed = false;
        }
    }
    return false;
}
