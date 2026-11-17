/*
 * Place_ship.c
 *
 *  Created on: 14 Nov 2026
 *      Author: jonxa
*/

#include "Place_ship.h"
//, int info[][10]
void place_ship(Graphics_Context *g_sContext)
{
    int pointer[2] = {0, 0}; // Ubicación del barco
    bool rot = false; // Sentido del barco

    Graphics_Rectangle rect;

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

        Graphics_clearDisplay(g_sContext);

        if ((pointer[0] <= 4) && (pointer[1] <= 4))
        {
            pantalla11_drw(g_sContext);

            rect.xMin = 27 + 20 * pointer[0];
            rect.xMax = 47 + 20 * pointer[0];

            rect.yMax = 27 + 20 * pointer[0];
            rect.yMin = 47 + 20 * pointer[0];

        }
        else if ((pointer[0] > 4) && (pointer[1] <= 4))
        {
            pantalla12_drw(g_sContext);

            rect.xMin = 20 * pointer[0];
            rect.xMax = 20 + 20 * pointer[0];

            rect.yMax = 27 + 20 * pointer[0];
            rect.yMin = 47 + 20 * pointer[0];

        }
        else if ((pointer[0] <= 4) && (pointer[1] > 4))
        {
            pantalla21_drw(g_sContext);

            rect.xMin = 27 + 20 * pointer[0];
            rect.xMax = 47 + 20 * pointer[0];

            rect.yMax = 20 * pointer[0];
            rect.yMin = 20 + 20 * pointer[0];

        }
        else
        {
            pantalla22_drw(g_sContext);

            rect.xMin = 20 * pointer[0];
            rect.xMax = 20 + 20 * pointer[0];

            rect.yMax = 20 * pointer[0];
            rect.yMin = 20 + 20 * pointer[0];

        }

        Graphics_setForegroundColor(g_sContext, ClrCoral);
        Graphics_fillRectangle(g_sContext, &rect);

        __delay_cycles(8000000); //Espera de 500ms~
    }
}
