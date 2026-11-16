/*
 * Place_ship.c
 *
 *  Created on: 14 Nov 2026
 *      Author: jonxa
*/

#include "Place_ship.h"

void place_shipd(int info[][10])
{
    int pointer[2] = {0, 0}; // Ubicación del barco
    bool rot = false; // Sentido del barco

    while (1)
    {
        if (JOYSTICK_get_x() > 50) // movimiento joystick eje X
        {
            if (pointer[0] == 9)
            {
                continue;
            }
            else
            {
                pointer[0]++;
            }
        }
        else if (JOYSTICK_get_x() < -50)
        {
            if ((pointer[0] == 1 && rot == false) || ((pointer[0] == 9) && rot == true))
            {
                continue;
            }
            else
            {
                pointer[0]--;
            }
        }

        if (JOYSTICK_get_y() > 50) // movimiento joystick eje Y
        {
            if (pointer[1] == 0)
            {
                continue;
            }
            else
            {
                pointer[1]--;
            }
        }
        else if (JOYSTICK_get_y() < -50)
        {
            if ((pointer[1] == 8 && rot == false) || ((pointer[0] == 9) && rot == true))
            {
                continue;
            }
            else
            {
                pointer[1]++;
            }
        }

        if (flag_edumkii_SEL)
        {
            flag_edumkii_SEL = false;
            rot = !rot;
        }

        // TODO: Rotar pieza, generar gráficos, poner barco con A, Retardo para no saturar

        __delay_cycles(8000000); //Espera de 500ms~
    }
}

void ship_2
