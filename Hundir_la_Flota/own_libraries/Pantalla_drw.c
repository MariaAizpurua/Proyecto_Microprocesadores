/*
 * Pantalla_drw.c
 * Dibujo de las vistas del tablero.
 * Autor: arodriguezd.5.
 */

#include "Pantalla_drw.h"

/*  Screen enumeration:
 *
 *    ----   ----
 *  |  11  |  12  |
 *    ----   ----
 *  |  21  |  22  |
 *    ----   ----
 */

void pantalla11_drw(Graphics_Context *g_sContext)
{
    int32_t line_v[] = {27, 0, 127}, line_h[] = {0, 127, 27};
    char letra[] = "ABCDE", zenbaki[] = "12345";
    int i;

    Graphics_setForegroundColor(g_sContext, ClrBlack);
    Graphics_setBackgroundColor(g_sContext, ClrWhite);

    for (i = 0; i < 5; i++)
    {
        Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]); // Dibuja la cuadricula
        Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

        Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 14, 37 + i*20, true); // Pone los Radix
        Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 1, 37 + i*20, 14, true);

    }
}

void pantalla12_drw(Graphics_Context *g_sContext)
{
    int32_t line_v[] = {20, 0, 127}, line_h[] = {0, 127, 27};
    char letra[] = "ABCDE", zenbaki[] = "678910";
    int i;

    Graphics_setForegroundColor(g_sContext, ClrBlack);
    Graphics_setBackgroundColor(g_sContext, ClrWhite);

    for (i = 0; i < 5; i++)
    {
        if (i != 4)
        {
            Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
            Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

            Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 114, 37 + i*20, true);
            Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 1, 10 + i*20, 14, true);
        }
        else
        {
            Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
            Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

            Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 114, 37 + i*20, true);
            Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 2, 10 + i*20, 14, true);
        }
    }
}

void pantalla21_drw(Graphics_Context *g_sContext)
{
    int32_t line_v[] = {27, 0, 127}, line_h[] = {0, 127, 20};
    char letra[] = "FGHIJ", zenbaki[] = "12345";
    int i;

    Graphics_setForegroundColor(g_sContext, ClrBlack);
    Graphics_setBackgroundColor(g_sContext, ClrWhite);

    for (i = 0; i < 5; i++)
    {
        Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
        Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

        Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 14, 10 + i*20, true);
        Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 1, 37 + i*20, 114, true);

    }
}

void pantalla22_drw(Graphics_Context *g_sContext)
{
    int32_t line_v[] = {20, 0, 127}, line_h[] = {0, 127, 20};
    char letra[] = "FGHIJ", zenbaki[] = "678910";
    int i;

    Graphics_setForegroundColor(g_sContext, ClrBlack);
    Graphics_setBackgroundColor(g_sContext, ClrWhite);

    for (i = 0; i < 5; i++)
    {
        if (i != 4)
        {
            Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
            Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

            Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 114, 10 + i*20, true);
            Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 1, 10 + i*20, 114, true);
        }
        else
        {
            Graphics_drawLineV(g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
            Graphics_drawLineH(g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

            Graphics_drawStringCentered(g_sContext, (int8_t *)&letra[i], 1, 114, 10 + i*20, true);
            Graphics_drawStringCentered(g_sContext, (int8_t *)&zenbaki[i], 2, 10 + i*20, 114, true);
        }
    }
}
