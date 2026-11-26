/*
 * partida_interfaz.c
 * Colocacion de flotas, mensajes y esperas.
 */

#include <msp430.h>
#include <driverlib.h>
#include <grlib.h>
#include <drivers/switch.h>
#include <drivers/buzzer.h>
#include <own_libraries/estilo_militar.h>
#include <own_libraries/menu_juego.h>
#include <own_libraries/partida_interfaz.h>

extern Graphics_Context g_sContext;
void PARTIDA_pantalla_mensaje(const char *l1, const char *l2, const char *l3)
{
    pantalla_militar(l1, l2, l3, ESTILO_NORMAL);
}

void PARTIDA_esperar_S1(void)
{
    while (!SWITCH_get_edumkii_S1())
        BUZ_wait_ms(1);
}

void PARTIDA_esperar_inicio_turno(void)
{
    PARTIDA_pantalla_mensaje("Tu turno", "S1: elegir disparo", "P5.5: menu");
    while (!SWITCH_get_edumkii_S1())
    {
        if (MENU_boton_pulsado())
        {
            MENU_abrir();
            PARTIDA_pantalla_mensaje("Tu turno", "S1: elegir disparo", "P5.5: menu");
        }
        BUZ_wait_ms(1);
    }
    while (SWITCH_get_edumkii_S1())
        BUZ_wait_ms(1);
}
