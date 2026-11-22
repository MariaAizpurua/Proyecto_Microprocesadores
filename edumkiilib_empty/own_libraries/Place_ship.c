/*
 * Place_ship.c
 *
 *  Created on: 14 Nov 2026
 *      Author: jonxa
*/

#include "Place_ship.h"

/* MERECE LA PENA VERLO
static void dibujar_corazon(Graphics_Context *g_sContext, int16_t cx, int16_t cy)
{
    int16_t i;

    Graphics_fillCircle(g_sContext, cx - 2, cy, 2);
    Graphics_fillCircle(g_sContext, cx + 2, cy, 2);

    for (i = 0; i < 5; i++)
    {
        Graphics_drawLine(g_sContext, cx - (4 - i), cy + 1 + i,
                                      cx + (4 - i), cy + 1 + i);
    }
}

void pantalla_casilla_ocupada(Graphics_Context *g_sContext)
{
    Graphics_Rectangle marco_ext = {3, 3, 124, 124};
    Graphics_Rectangle marco_int = {6, 6, 121, 121};

    // Fondo rosa pastel clarito
    Graphics_setBackgroundColor(g_sContext, 0xFFF0F5);
    Graphics_clearDisplay(g_sContext);

    // Marco doble rosa
    Graphics_setForegroundColor(g_sContext, 0xFF69B4); // Rosa fuerte
    Graphics_drawRectangle(g_sContext, &marco_ext);

    Graphics_setForegroundColor(g_sContext, 0xFFB6C1); // Rosa bebe
    Graphics_drawRectangle(g_sContext, &marco_int);

    // Corazoncitos arriba y abajo
    Graphics_setForegroundColor(g_sContext, 0xFF1493); // Fucsia
    dibujar_corazon(g_sContext, 22, 18);
    dibujar_corazon(g_sContext, 105, 18);
    dibujar_corazon(g_sContext, 64, 108);

    // Texto en morado dulce con buen contraste
    Graphics_setFont(g_sContext, &g_sFontCm12);
    Graphics_setForegroundColor(g_sContext, 0x8B1C62);

    Graphics_drawStringCentered(g_sContext, (int8_t *)"~ Ups! ~",           AUTO_STRING_LENGTH, 64, 21, false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"Alguna casilla ya",  AUTO_STRING_LENGTH, 64, 44, false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"ha sido ocupada,",   AUTO_STRING_LENGTH, 64, 59, false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"por favor, intente", AUTO_STRING_LENGTH, 64, 74, false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"otra posicion",      AUTO_STRING_LENGTH, 64, 89, false);

    // Restaura el fondo blanco para que el tablero no se quede rosa al volver
    Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
}^
*/

static void dibujar_alerta(Graphics_Context *g_sContext, int16_t cx, int16_t top_y)
{
    int16_t i, w;
    Graphics_Rectangle barra, punto;

    // Triangulo exterior negro (borde de 2 px)
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);
    for (i = 0; i <= 24; i++)
    {
        w = (i * 14) / 24;
        Graphics_drawLine(g_sContext, cx - w, top_y + i, cx + w, top_y + i);
    }

    // Triangulo interior amarillo intenso
    Graphics_setForegroundColor(g_sContext, 0xFFD000);
    for (i = 0; i <= 19; i++)
    {
        w = (i * 11) / 19;
        Graphics_drawLine(g_sContext, cx - w, top_y + 3 + i, cx + w, top_y + 3 + i);
    }

    // Signo de exclamacion (!) negro dentro del triangulo
    Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BLACK);

    barra.xMin = cx - 1;
    barra.xMax = cx + 1;
    barra.yMin = top_y + 9;
    barra.yMax = top_y + 15;
    Graphics_fillRectangle(g_sContext, &barra);

    punto.xMin = cx - 1;
    punto.xMax = cx + 1;
    punto.yMin = top_y + 18;
    punto.yMax = top_y + 20;
    Graphics_fillRectangle(g_sContext, &punto);
}

void pantalla_casilla_ocupada(Graphics_Context *g_sContext)
{
    Graphics_Rectangle marco_ext = {3, 3, 124, 124};
    Graphics_Rectangle marco_int = {6, 6, 121, 121};

    // Fondo amarillo crema suave
    Graphics_setBackgroundColor(g_sContext, 0xFFFDE0);
    Graphics_clearDisplay(g_sContext);

    // Marco doble en tonos ambar y dorado
    Graphics_setForegroundColor(g_sContext, 0xD48800); // Ambar oscuro
    Graphics_drawRectangle(g_sContext, &marco_ext);

    Graphics_setForegroundColor(g_sContext, 0xFFC107); // Amarillo señal
    Graphics_drawRectangle(g_sContext, &marco_int);

    // Cartel triangular de cuidado centrado arriba
    dibujar_alerta(g_sContext, 64, 13);

    // Texto en marron muy oscuro (mas legible sobre amarillo que el negro puro)
    Graphics_setFont(g_sContext, &g_sFontCm12);
    Graphics_setForegroundColor(g_sContext, 0x3E2700);

    Graphics_drawStringCentered(g_sContext, (int8_t *)"Alguna casilla ya",  AUTO_STRING_LENGTH, 64, 54,  false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"ha sido ocupada,",   AUTO_STRING_LENGTH, 64, 70,  false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"por favor, intente", AUTO_STRING_LENGTH, 64, 86,  false);
    Graphics_drawStringCentered(g_sContext, (int8_t *)"otra posicion",      AUTO_STRING_LENGTH, 64, 102, false);

    // Restaura el fondo blanco para el tablero
    Graphics_setBackgroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
}

void place_ship_2(Graphics_Context *g_sContext, uint8_t info[10][10])
{
    int pointer[2] = {1, 0}; // Ubicación del barco
    bool rot = false; // Sentido del barco
    bool flg = true; // Flag para evitar que flashee todo el rato una limpieza del display

    Graphics_Rectangle rect;

    while (1)
    {
        if ((JOYSTICK_get_x() > 50) && (pointer[0] < 9)) // movimiento joystick eje X
        {
            pointer[0]++;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }
        else if ((JOYSTICK_get_x() < -50) && !((pointer[0] == 1 && rot == false) || ((pointer[0] == 0) && rot == true)))
        {
            pointer[0]--;

            Graphics_clearDisplay(g_sContext);

            flg = true;

        }

        if ((JOYSTICK_get_y() > 50) && (pointer[1] > 0)) // movimiento joystick eje Y
        {
            pointer[1]--;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }
        else if ((JOYSTICK_get_y() < -50) && !((pointer[1] == 9 && rot == false) || ((pointer[1] == 8) && rot == true)))
        {
            pointer[1]++;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }

        if (JOYSTICK_get_sel()) // rotación del barco
        {

            if (rot == false && pointer[1] == 9)
            {
                pointer[1] = 8;
            }
            else if (rot == true && pointer[0] == 0)
            {
                pointer[0] = 1;
            }

            rot = !rot;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }

        if (rot == false)
        {
            if ((pointer[0] <= 4) && (pointer[1] <= 4)) // Dibujar la pantalla y un rectángulo rojo donde estamos seleccionando
            {
                if (flg == true) // Dibuja el grid si se ha cambiado
                {
                pantalla11_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] - 19; // Rectángulo rojo
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 47 + 20 * pointer[1] - 1;

                if (pointer[0] == 4) // Corrección en los bordes de la pantalla
                {
                    rect.xMax++;
                }

                if (pointer[1] == 4)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] > 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla12_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) - 19;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 47 + 20 * pointer[1] - 1;

                if (pointer[0] == 6)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 4)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] <= 4) && (pointer[1] > 4))
            {
                if (flg == true)
                {
                pantalla21_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] - 19;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 20 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
            else
            {
                if (flg == true)
                {
                pantalla22_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) - 19;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 20 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] ==6)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
        }
        else //Hay que repetir para cuando el barco está vertical
        {
            if ((pointer[0] <= 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla11_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] + 1;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 67 + 20 * pointer[1] - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 3)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] > 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla12_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) + 1;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 67 + 20 * pointer[1] - 1;

                if (pointer[0] == 5)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 3)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] <= 4) && (pointer[1] > 4))
            {
                if (flg == true)
                {
                pantalla21_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] + 1;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 40 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
            else
            {
                if (flg == true)
                {
                pantalla22_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) + 1;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 40 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 5)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
        }

        Graphics_setForegroundColor(g_sContext, ClrCoral);
        Graphics_fillRectangle(g_sContext, &rect);

        if (SWITCH_get_edumkii_S1()) //exraemos la posición seleccionada y terminamos la función
        {
            info[pointer[1]][pointer[0]] = 1;

            if (rot == false)
            {
                info[pointer[1]][pointer[0] - 1] = 1;
            }
            else
            {
                info[pointer[1] + 1][pointer[0]] = 1;
            }

            return;
        }

        __delay_cycles(8000000); //Espera de 500ms~
    }
}

void place_ship_31(Graphics_Context *g_sContext, uint8_t info[10][10])
{
    int pointer[2] = {2, 0}; // Ubicación del barco
    bool rot = false; // Sentido del barco
    bool flg = true; // Flag para evitar que flashee todo el rato una limpieza del display
    int i;

    Graphics_Rectangle rect;
    Graphics_clearDisplay(g_sContext);

    while (1)
    {


        if ((JOYSTICK_get_x() > 50) && (pointer[0] < 9)) // movimiento joystick eje X
        {
            pointer[0]++;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }
        else if ((JOYSTICK_get_x() < -50) && !((pointer[0] == 2 && rot == false) || ((pointer[0] == 0) && rot == true)))
        {
            pointer[0]--;

            Graphics_clearDisplay(g_sContext);

            flg = true;

        }

        if ((JOYSTICK_get_y() > 50) && (pointer[1] > 0)) // movimiento joystick eje Y
        {
            pointer[1]--;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }
        else if ((JOYSTICK_get_y() < -50) && !((pointer[1] == 9 && rot == false) || ((pointer[1] == 7) && rot == true)))
        {
            pointer[1]++;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }

        if (JOYSTICK_get_sel()) // rotación del barco
        {

            if (rot == false && pointer[1] >= 8)
            {
                pointer[1] = 7;
            }
            else if (rot == true && pointer[0] <= 1)
            {
                pointer[0] = 2;
            }

            rot = !rot;

            Graphics_clearDisplay(g_sContext);

            flg = true;
        }

        if (rot == false)
        {
            if ((pointer[0] <= 4) && (pointer[1] <= 4)) // Dibujar la pantalla y un rectángulo rojo donde estamos seleccionando
            {
                if (flg == true) // Dibuja el grid si se ha cambiado algo
                {
                pantalla11_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] - 39; // Rectángulo rojo
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 47 + 20 * pointer[1] - 1;

                if (pointer[0] == 4) // Corrección en los bordes de la pantalla
                {
                    rect.xMax++;
                }

                if (pointer[1] == 4)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] > 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla12_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) - 39;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 47 + 20 * pointer[1] - 1;

                if (pointer[0] == 7)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 4)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] <= 4) && (pointer[1] > 4))
            {
                if (flg == true)
                {
                pantalla21_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] - 39;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 20 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
            else
            {
                if (flg == true)
                {
                pantalla22_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) - 39;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 20 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] ==7)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
        }
        else //Hay que repetir para cuando el barco está vertical
        {
            if ((pointer[0] <= 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla11_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] + 1;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 87 + 20 * pointer[1] - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 2)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] > 4) && (pointer[1] <= 4))
            {
                if (flg == true)
                {
                pantalla12_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) + 1;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 27 + 20 * pointer[1] + 1;
                rect.yMin = 87 + 20 * pointer[1] - 1;

                if (pointer[0] == 5)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 2)
                {
                    rect.yMin++;
                }
            }
            else if ((pointer[0] <= 4) && (pointer[1] > 4))
            {
                if (flg == true)
                {
                pantalla21_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 27 + 20 * pointer[0] + 1;
                rect.xMax = 47 + 20 * pointer[0] - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 60 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 4)
                {
                    rect.xMax++;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
            else
            {
                if (flg == true)
                {
                pantalla22_drw(g_sContext);

                flg = false;
                }

                rect.xMin = 20 * (pointer[0] - 5) + 1;
                rect.xMax = 20 + 20 * (pointer[0] - 5) - 1;

                rect.yMax = 20 * (pointer[1] - 5) + 1;
                rect.yMin = 60 + 20 * (pointer[1] - 5) - 1;

                if (pointer[0] == 5)
                {
                    rect.xMin--;
                }

                if (pointer[1] == 5)
                {
                    rect.yMax--;
                }
            }
        }

        Graphics_setForegroundColor(g_sContext, ClrCoral);
        Graphics_fillRectangle(g_sContext, &rect);

        if (SWITCH_get_edumkii_S1()) // extraemos la posición seleccionada y terminamos la función
        {
            for (i = 0; i < 3; i++)
            {
                if ((rot == true  && info[pointer[1] + i][pointer[0]] != 0) || (rot == false && info[pointer[1]][pointer[0] - i] != 0))
                {
                    break;
                }
            }

            if (i == 3)
            {
                info[pointer[1]][pointer[0]] = 2;

                if (rot == false)
                {
                    info[pointer[1]][pointer[0] - 1] = 2;
                    info[pointer[1]][pointer[0] - 2] = 2;
                }
                else
                {
                    info[pointer[1] + 1][pointer[0]] = 2;
                    info[pointer[1] + 2][pointer[0]] = 2;
                }

                return;
            }
            else
            {
                pantalla_casilla_ocupada(g_sContext);
                __delay_cycles(32000000);
                Graphics_clearDisplay(g_sContext);
                flg = true;
            }
        }

        __delay_cycles(8000000); //Espera de 500ms~
    }
}
