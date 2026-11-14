#include "pantalla_marcador.h"
#include <msp430.h>
#include <driverlib.h>
#include <stdbool.h>
#include <grlib.h>
#include <stdint.h>
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


void marcador(void){
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

    //Jugador 1 barcos
    Graphics_Rectangle c0 = {2, 62, 7, 67};
    Graphics_drawRectangle(&g_sContext, &c0);

    Graphics_Rectangle c1 = {10, 62, 15, 67};
    Graphics_drawRectangle(&g_sContext, &c1);

    Graphics_Rectangle c2 = {2, 72, 7, 77};
    Graphics_drawRectangle(&g_sContext, &c2);

    Graphics_Rectangle c3 = {10, 72, 15, 77};
    Graphics_drawRectangle(&g_sContext, &c3);

    Graphics_Rectangle c4 = {18, 72, 23, 77};
    Graphics_drawRectangle(&g_sContext, &c4);

    Graphics_Rectangle c5 = {2, 82, 7, 87};
    Graphics_drawRectangle(&g_sContext, &c5);

    Graphics_Rectangle c6 = {10, 82, 15, 87};
    Graphics_drawRectangle(&g_sContext, &c6);

    Graphics_Rectangle c7 = {18, 82, 23, 87};
    Graphics_drawRectangle(&g_sContext, &c7);

    Graphics_Rectangle c8 = {2, 92, 7, 97};
    Graphics_drawRectangle(&g_sContext, &c8);

    Graphics_Rectangle c9 = {10, 92, 15, 97};
    Graphics_drawRectangle(&g_sContext, &c9);

    Graphics_Rectangle c10 = {18, 92, 23, 97};
    Graphics_drawRectangle(&g_sContext, &c10);

    Graphics_Rectangle c11 = {26, 92, 31, 97};
    Graphics_drawRectangle(&g_sContext, &c11);

    Graphics_Rectangle c12 = {2, 102, 7, 107};
    Graphics_drawRectangle(&g_sContext, &c12);

    Graphics_Rectangle c13 = {10, 102, 15, 107};
    Graphics_drawRectangle(&g_sContext, &c13);

    Graphics_Rectangle c14 = {18, 102, 23, 107};
    Graphics_drawRectangle(&g_sContext, &c14);

    Graphics_Rectangle c15 = {26, 102, 31, 107};
    Graphics_drawRectangle(&g_sContext, &c15);

    Graphics_Rectangle c16 = {34, 102, 39, 107};
    Graphics_drawRectangle(&g_sContext, &c16);

    //Jugador 2 barcos
    Graphics_Rectangle c17 = {64, 62, 69, 67};
    Graphics_drawRectangle(&g_sContext, &c17);

    Graphics_Rectangle c18 = {72, 62, 77, 67};
    Graphics_drawRectangle(&g_sContext, &c18);

    Graphics_Rectangle c19 = {64, 72, 69, 77};
    Graphics_drawRectangle(&g_sContext, &c19);

    Graphics_Rectangle c20 = {72, 72, 77, 77};
    Graphics_drawRectangle(&g_sContext, &c20);

    Graphics_Rectangle c21 = {80, 72, 85, 77};
    Graphics_drawRectangle(&g_sContext, &c21);

    Graphics_Rectangle c22 = {64, 82, 69, 87};
    Graphics_drawRectangle(&g_sContext, &c22);

    Graphics_Rectangle c23 = {72, 82, 77, 87};
    Graphics_drawRectangle(&g_sContext, &c23);

    Graphics_Rectangle c24 = {80, 82, 85, 87};
    Graphics_drawRectangle(&g_sContext, &c24);

    Graphics_Rectangle c25 = {64, 92, 69, 97};
    Graphics_drawRectangle(&g_sContext, &c25);

    Graphics_Rectangle c26 = {72, 92, 77, 97};
    Graphics_drawRectangle(&g_sContext, &c26);

    Graphics_Rectangle c27 = {80, 92, 85, 97};
    Graphics_drawRectangle(&g_sContext, &c27);

    Graphics_Rectangle c28 = {88, 92, 93, 97};
    Graphics_drawRectangle(&g_sContext, &c28);

    Graphics_Rectangle c29 = {64, 102, 69, 107};
    Graphics_drawRectangle(&g_sContext, &c29);

    Graphics_Rectangle c30 = {72, 102, 77, 107};
    Graphics_drawRectangle(&g_sContext, &c30);

    Graphics_Rectangle c31 = {80, 102, 85, 107};
    Graphics_drawRectangle(&g_sContext, &c31);

    Graphics_Rectangle c32 = {88, 102, 93, 107};
    Graphics_drawRectangle(&g_sContext, &c32);

    Graphics_Rectangle c33 = {96, 102, 101, 107};
    Graphics_drawRectangle(&g_sContext, &c33);




}

void dibujar_turno(uint8_t jugador){
    if (jugador == 0){
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"TURNO: PLAYER 1",
                                    AUTO_STRING_LENGTH,64, 5, OPAQUE_TEXT);
    }
    else{
        Graphics_drawStringCentered(&g_sContext, (int8_t *)"TURNO: PLAYER 2",
                                    AUTO_STRING_LENGTH,64,5, OPAQUE_TEXT);
    }
}

void dibujar_contador(uint8_t jugador,uint8_t tipo,uint8_t valor){
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

void registrar_contador(uint8_t jugador,uint8_t tipo){
    contador[jugador][tipo]++;
  dibujar_contador(jugador,tipo,contador[jugador][tipo]);

}

void dibujar_barco_tocado(uint8_t jugador,uint8_t barco,uint8_t pos){
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

void barco_tocado(uint8_t jugador,uint8_t barco){
    dibujar_barco_tocado(jugador,barco,barcos[jugador][barco]);
    barcos[jugador][barco]++;
    if(barcos[jugador][barco]==tamano_barco[barco]){
        registrar_contador(jugador,1); //hundido
    }
    else{
        registrar_contador(jugador,0);//tocado

    }

}
