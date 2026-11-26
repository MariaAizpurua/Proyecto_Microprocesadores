/*
 * colocacion_flota.c
 * Inicializacion del tablero y colocacion interactiva de la flota.
 */

#include <string.h>
#include <stdio.h>
#include <grlib.h>
#include <own_libraries/Place_ship.h>
#include <own_libraries/partida_interfaz.h>
#include <own_libraries/colocacion_flota.h>

extern Graphics_Context g_sContext;

void COLOCACION_colocar_flota(uint8_t tablero[FILAS_TABLERO][COLS_TABLERO],
                              uint8_t jugador)
{
    char linea[20];

    memset(tablero, AGUA, FILAS_TABLERO * COLS_TABLERO);
    sprintf(linea, "Jugador %d", jugador + 1);
    PARTIDA_pantalla_mensaje(linea, "Coloca tus barcos", "S1: empezar");
    PARTIDA_esperar_S1();

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(&g_sContext);
    place_ship_2 (&g_sContext, tablero);
    place_ship_31(&g_sContext, tablero);
    place_ship_32(&g_sContext, tablero);
    place_ship_4 (&g_sContext, tablero);
    place_ship_5 (&g_sContext, tablero);
}
