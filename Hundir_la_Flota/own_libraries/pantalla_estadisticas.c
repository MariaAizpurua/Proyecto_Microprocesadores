/*
 * pantalla_estadisticas.c
 * Registro y dibujo de estadisticas locales.
 * Autora: Irene.
 */

#include "pantalla_estadisticas.h"
#include <msp430.h>
#include <driverlib.h>
#include <stdbool.h>
#include <grlib.h>
#include <stdint.h>
#include <string.h>
#include <own_libraries/enlace_juego.h>
#include <own_libraries/tablero.h>
extern Graphics_Context g_sContext;

#define Y_Separacion_turno 11
#define Y_Separacion_evento 115
#define x_division 63
#define titulos 17
#define col_x1 31
#define col_x2 95
#define fila1_y 30
#define separacion_fila 10

static uint8_t contador[2][3];
static const uint8_t tamano_barco[5]={2,3,3,4,5};
static uint8_t barcos[2][5];

static void dibujar_casilla_tocada(uint8_t jugador, uint8_t barco, uint8_t pos);
static void dibujar_contador_estadisticas(uint8_t jugador, uint8_t tipo, uint8_t valor);

static void dibujar_casilla_vacia(uint8_t jugador,uint8_t barco,uint8_t pos){
    int16_t x,y;

    if(jugador==0){
        x=2+pos*8;
          }
    else{
        x=64+pos*8;
           }
    y=62+barco*10;

    Graphics_Rectangle r={x,y,x+5,y+5 };
    Graphics_drawRectangle(&g_sContext, &r);

}
void ESTADISTICAS_mostrar(void){
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_setBackgroundColor(&g_sContext, ClrWhite);
    Graphics_clearDisplay(&g_sContext);
    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);

    Graphics_drawLineH(&g_sContext, 0, 127, Y_Separacion_turno);
    Graphics_drawLineH(&g_sContext, 0, 127, Y_Separacion_evento);

    Graphics_drawLineV(&g_sContext,x_division,Y_Separacion_turno,Y_Separacion_evento);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"PLAYER 1",AUTO_STRING_LENGTH,col_x1,titulos,TRANSPARENT_TEXT);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"PLAYER 2",AUTO_STRING_LENGTH,col_x2,titulos,TRANSPARENT_TEXT);

    Graphics_drawString(&g_sContext, (int8_t *)"Tocado:",AUTO_STRING_LENGTH,2,fila1_y,TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, (int8_t *)"Hundido:",AUTO_STRING_LENGTH,2,fila1_y+separacion_fila,TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, (int8_t *)"Agua:",AUTO_STRING_LENGTH,2,fila1_y+2*separacion_fila,TRANSPARENT_TEXT);

    Graphics_drawString(&g_sContext, (int8_t *)"Tocado:",AUTO_STRING_LENGTH,66,fila1_y,TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, (int8_t *)"Hundido:",AUTO_STRING_LENGTH,66,fila1_y+separacion_fila,TRANSPARENT_TEXT);
    Graphics_drawString(&g_sContext, (int8_t *)"Agua:",AUTO_STRING_LENGTH,66,fila1_y+2*separacion_fila,TRANSPARENT_TEXT);

    uint8_t j, b, p;
    for (j = 0; j < 2; j++){
        for (b = 0; b < 5; b++){
            for (p = 0; p < tamano_barco[b]; p++){
                dibujar_casilla_vacia(j, b, p);
                if (p < barcos[j][b])
                    dibujar_casilla_tocada(j, b, p);
            }
        }
        for (b = 0; b < 3; b++)
            dibujar_contador_estadisticas(j, b, contador[j][b]);
    }

    Graphics_setFont(&g_sContext, &g_sFontFixed6x8);
    Graphics_setForegroundColor(&g_sContext, ClrBlack);
    Graphics_drawStringCentered(&g_sContext, (int8_t *)"S1: volver al menu",
                                AUTO_STRING_LENGTH, 64, 118, TRANSPARENT_TEXT);
}

void ESTADISTICAS_reiniciar(void){
    memset(contador, 0, sizeof(contador));
    memset(barcos, 0, sizeof(barcos));
}

static void dibujar_contador_estadisticas(uint8_t jugador,uint8_t tipo,uint8_t valor){
    int16_t x,y;
    char texto[3];

    if (jugador==0){
        x=50;
}
    else{
            x=114;
    }
    y=fila1_y + tipo * separacion_fila;

    texto[0]='0'+valor/10;
    texto[1]='0'+valor%10;
    texto[2]='\0';

    Graphics_drawString(&g_sContext,(int8_t*)texto,AUTO_STRING_LENGTH,x,y,OPAQUE_TEXT);


}

static void ESTADISTICAS_registrar_agua(uint8_t jugador){
    contador[jugador][2]++;
}

static void dibujar_casilla_tocada(uint8_t jugador,uint8_t barco,uint8_t pos){
    int16_t x,y;

    if(jugador==0){
        x=2+pos*8;
          }
    else{
        x=64+pos*8;
           }
    y=62+barco*10;

    Graphics_Rectangle r={x,y,x+5,y+5 };
    Graphics_fillRectangle(&g_sContext, &r);

}

static void ESTADISTICAS_registrar_tocado(uint8_t jugador,uint8_t barco){
    uint8_t tamano = tamano_barco[barco];
    uint8_t progreso = barcos[jugador][barco];
    barcos[jugador][barco]++;
    contador[jugador][progreso + 1 == tamano ? 1 : 0]++;
}

void ESTADISTICAS_registrar_resultado(uint8_t jugador, uint8_t resultado)
{
    if (resultado == RES_AGUA)
        ESTADISTICAS_registrar_agua(jugador);
    else if (resultado >= 1 && resultado <= 2 * NUM_BARCOS)
        ESTADISTICAS_registrar_tocado(jugador,
                (resultado > NUM_BARCOS ? resultado - NUM_BARCOS : resultado) - 1);
}
