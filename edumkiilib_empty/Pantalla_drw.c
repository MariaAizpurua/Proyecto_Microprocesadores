


void pantalla11_drw()
{
    int32_t line_v[] = {27, 0, 127}, line_h[] = {0, 127, 27};
    char letra[] = "ABCDE", zenbaki[] = "12345";
    int i;

    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);

    for (i = 0; i < 5; i++)
    {
        Graphics_drawLineV(&g_sContext, line_v[0] + i*20, line_v[1], line_v[2]);
        Graphics_drawLineH(&g_sContext, line_h[0], line_h[1], line_h[2] + i*20);

        Graphics_drawString(&g_sContext, (int8_t *)&letra[i], 1, 9, 33 + i*20, true);   // Pendiente cambiar por Graphics_drawStringCentered para hacerlo bonito
        Graphics_drawString(&g_sContext, (int8_t *)&zenbaki[i], 1, 33 + i*20, 9, true);

    }
}
