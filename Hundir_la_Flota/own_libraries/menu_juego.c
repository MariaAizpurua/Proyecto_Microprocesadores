/*
 * menu_juego.c
 * Control del menu, estadisticas y musica.
 */

#include <msp430.h>
#include <driverlib.h>
#include <grlib.h>
#include <stdint.h>

#include <drivers/buzzer.h>
#include <drivers/joystick.h>
#include <drivers/switch.h>
#include <own_libraries/estilo_militar.h>
#include <own_libraries/menu_juego.h>
#include <own_libraries/pantalla_estadisticas.h>

extern Graphics_Context g_sContext;

static bool musica_activa;

void MENU_init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P5, GPIO_PIN5);
    musica_activa = false;
}

static void mostrar_estadisticas(void)
{
    ESTADISTICAS_mostrar();

    while (!SWITCH_get_edumkii_S1())
        BUZ_wait_ms(1);
    while (SWITCH_get_edumkii_S1())
        BUZ_wait_ms(1);
}

bool MENU_boton_pulsado(void)
{
    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) != GPIO_INPUT_PIN_LOW)
        return false;

    BUZ_wait_ms(20);
    if (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) != GPIO_INPUT_PIN_LOW)
        return false;

    while (GPIO_getInputPinValue(GPIO_PORT_P5, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)
        BUZ_wait_ms(1);
    BUZ_wait_ms(20);
    return true;
}

void MENU_abrir(void)
{
    uint8_t opcion = 0;
    uint8_t anterior;
    float y;

    menu_militar_dibujar(opcion, musica_activa);
    while (1)
    {
        if (MENU_boton_pulsado())
            return;

        y = JOYSTICK_get_y();
        if (y > 30 || y < -30)
        {
            anterior = opcion;
            if (y > 30)
                opcion = (opcion + MENU_NUM_OPCIONES - 1) % MENU_NUM_OPCIONES;
            else
                opcion = (opcion + 1) % MENU_NUM_OPCIONES;

            menu_militar_placa(anterior, false, musica_activa);
            menu_militar_placa(opcion, true, musica_activa);
            BUZ_wait_ms(200);
        }

        if (SWITCH_get_edumkii_S1())
        {
            while (SWITCH_get_edumkii_S1())
                BUZ_wait_ms(1);

            if (opcion == 0)
            {
                mostrar_estadisticas();
                menu_militar_dibujar(opcion, musica_activa);
            }
            else if (opcion == 1)
            {
                musica_activa = !musica_activa;
                BUZ_music_set(musica_activa);
                menu_militar_placa(opcion, true, musica_activa);
            }
            else
                return;
        }
        BUZ_wait_ms(1);
    }
}
