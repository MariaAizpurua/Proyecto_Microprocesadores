/*
 * interfaz_jugar.c
 *
 *  Created on: 14 nov. 2026
 *      Author: maria
 *
 *
 */

#include "interfaz_jugar.h"
#include <drivers/joystick.h>
#include <grlib.h>
#include "Pantalla_drw.h"
#include <drivers/switch.h>
#include <drivers/buzzer.h>
#include <stdio.h>
#include <msp430.h>
#include "vista_tablero.h"
#include "estilo_militar.h"

extern Graphics_Context g_sContext;


// Vuelve a dibujar la pantalla de disparo tal como estaba (cuadrante,
// casillas marcadas y cursor). Se usa al volver de ver mi tablero (P5.6).
static void redibujar_disparo(uint8_t tablero[10][10], int posX, int posY,
                              Graphics_Rectangle *rect){
    int cuadrante = (posY<5) ? ((posX<5) ? 1 : 2) : ((posX<5) ? 3 : 4);

    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    Graphics_clearDisplay(&g_sContext);
    marcar_casillas(tablero, cuadrante);
    switch (cuadrante){
    case 1: pantalla11_drw(&g_sContext); break;
    case 2: pantalla12_drw(&g_sContext); break;
    case 3: pantalla21_drw(&g_sContext); break;
    default: pantalla22_drw(&g_sContext); break;
    }
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_fillRectangle(&g_sContext, rect);
}

bool seleccionar_disparo(uint8_t tablero[10][10], uint8_t mi_tablero[10][10],
                         uint8_t *fila_elegida, uint8_t *columna_elegida){

    int fin=0;
    while(!fin){
        int posX=0;
        int posY=0;
        Graphics_Rectangle rect={28,28,46,46};
        Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
        Graphics_clearDisplay(&g_sContext);
        marcar_casillas(tablero,1);
        pantalla11_drw(&g_sContext);
        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
        Graphics_fillRectangle(&g_sContext, &rect);



        while (!SWITCH_get_edumkii_S1()){
            if (MENU_boton_pulsado()){
                MENU_abrir();
                redibujar_disparo(tablero, posX, posY, &rect);
            }
            // P5.6 (LaunchPad): ver mi tablero y, al volver a pulsar, regresar
            if (vista_boton_pulsado()){
                vista_mostrar_mi_tablero(&g_sContext, mi_tablero);
                redibujar_disparo(tablero, posX, posY, &rect);
            }
            if(JOYSTICK_get_x()>30){ //////No 0 por ruido
                if (posX<9){
                    posX++;
                }

                if(posX<5 && posY<5){
                    rect.xMin=28+posX*20;
                    rect.xMax=rect.xMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,1);
                    pantalla11_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);

                }else if(posX<5 && posY>=5){
                    rect.xMin=28+posX*20;
                    rect.xMax=rect.xMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,3);
                    pantalla21_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                } else if(posX>=5 && posY<5){
                    rect.xMin=(posX-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,2);
                    pantalla12_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }else{
                    rect.xMin=(posX-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,4);
                    pantalla22_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }

            }else if(JOYSTICK_get_x()<-30){
                    if (posX>0){
                        posX--;
                    }

                    if(posX<5 && posY<5){
                        rect.xMin=28+posX*20;
                        rect.xMax=rect.xMin+18;
                        Graphics_clearDisplay(&g_sContext);
                        marcar_casillas(tablero,1);
                        pantalla11_drw(&g_sContext);
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(&g_sContext, &rect);

                    }else if(posX<5 && posY>=5){
                        rect.xMin=28+posX*20;
                        rect.xMax=rect.xMin+18;
                        Graphics_clearDisplay(&g_sContext);
                        marcar_casillas(tablero,3);
                        pantalla21_drw(&g_sContext);
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(&g_sContext, &rect);
                    } else if(posX>=5 && posY<5){
                        rect.xMin=(posX-5)*20+1;
                        rect.xMax=rect.xMin+18;
                        Graphics_clearDisplay(&g_sContext);
                        marcar_casillas(tablero,2);
                        pantalla12_drw(&g_sContext);
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(&g_sContext, &rect);
                    }else{
                        rect.xMin=(posX-5)*20+1;
                        rect.xMax=rect.xMin+18;
                        Graphics_clearDisplay(&g_sContext);
                        marcar_casillas(tablero,4);
                        pantalla22_drw(&g_sContext);
                        Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                        Graphics_fillRectangle(&g_sContext, &rect);
                    }
            }
            BUZ_wait_ms(200);

            if(JOYSTICK_get_y()>30){
                if (posY>0){
                    posY--;
                }

                if(posX<5 && posY<5){
                    rect.yMin=28+posY*20;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,1);
                    pantalla11_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);

                }else if(posX>=5 && posY<5){
                    rect.yMin=28+posY*20;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,2);
                    pantalla12_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                } else if(posX<5 && posY>=5){
                    rect.yMin=(posY-5)*20+1;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,3);
                    pantalla21_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }else{
                    rect.yMin=(posY-5)*20+1;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,4);
                    pantalla22_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }
            }else if(JOYSTICK_get_y()<-30){
                if (posY<9){
                    posY++;
                }

                if(posX<5 && posY<5){
                    rect.yMin=28+posY*20;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,1);
                    pantalla11_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);

                }else if(posX>=5 && posY<5){
                    rect.yMin=28+posY*20;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,2);
                    pantalla12_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                } else if(posX<5 && posY>=5){
                    rect.yMin=(posY-5)*20+1;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,3);
                    pantalla21_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }else{
                    rect.yMin=(posY-5)*20+1;
                    rect.yMax=rect.yMin+18;
                    Graphics_clearDisplay(&g_sContext);
                    marcar_casillas(tablero,4);
                    pantalla22_drw(&g_sContext);
                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }

            }
            BUZ_wait_ms(200);


        }
        // Una vez elegido el cuadrado y pulsado el botón
        while (SWITCH_get_edumkii_S1());   // se queda aquí mientras S1 siga pulsado


        char texto[32];
        sprintf(texto,"Disparar a %c%d ?", 'A'+posY, posX+1);
        pantalla_militar_confirmar(texto, "CONFIRMA LA ORDEN");

        int s1, s2;
                do {
                    s1 = SWITCH_get_edumkii_S1();
                    s2 = SWITCH_get_edumkii_S2();
                    BUZ_wait_ms(1);
                } while (!s1 && !s2);

                if (s2) {                                 // cancelar
                    continue;
                }

            while (SWITCH_get_edumkii_S1());
            if (tablero[posY][posX] != 0)
            {
                pantalla_militar("Ya disparaste ahi", "Elige otra casilla",
                                 "", ESTILO_AVISO);
                BUZ_wait_ms(1500);
                continue;
            }

            *fila_elegida = (uint8_t)posY;
            *columna_elegida = (uint8_t)posX;
            return true;


        }





    return false;
}



void marcar_casillas(uint8_t tablero[10][10],int cuadrante){
    Graphics_Rectangle rect;
    int i,j;
    switch (cuadrante){
    case 1:
        for (i=0;i<5;i++){
            for (j=0;j<5;j++){
                if (tablero[j][i]>=6 && tablero[j][i]<=10){// Barcos tocados
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_CORAL);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }
                else if(tablero[j][i]==11 ){// Barco Hundido
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);


                }else if(tablero[j][i]==12){// Agua
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_SKY_BLUE);
                    Graphics_fillRectangle(&g_sContext, &rect);

                }


            }
        }
        break;
    case 2:
        for(i=5;i<10;i++){
            for (j=0;j<5;j++){
                if (tablero[j][i]>=6 && tablero[j][i]<=10){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_CORAL);
                    Graphics_fillRectangle(&g_sContext, &rect);
               }
                else if (tablero[j][i]==11){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                   rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }
                else if (tablero[j][i]==12){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                   rect.yMin=28+j*20;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_SKY_BLUE);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }


            }
        }
        break;

    case 3:
        for(i=0;i<5;i++){
            for(j=5;j<10;j++){
                if (tablero[j][i]>=6 && tablero[j][i]<=10){
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_CORAL);
                    Graphics_fillRectangle(&g_sContext, &rect);
               }
                else if (tablero[j][i]==11){
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                   rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }
                else if (tablero[j][i]==12){
                    rect.xMin=28+i*20;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_SKY_BLUE);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }


            }
        }
        break;








    case 4:
        for(i=5;i<10;i++){
            for(j=5;j<10;j++){
                if (tablero[j][i]>=6 && tablero[j][i]<=10){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_CORAL);
                    Graphics_fillRectangle(&g_sContext, &rect);
               }
                else if (tablero[j][i]==11){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_DARK_RED);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }
                else if (tablero[j][i]==12){
                    rect.xMin=(i-5)*20+1;
                    rect.xMax=rect.xMin+18;
                    rect.yMin=(j-5)*20+1;
                    rect.yMax=rect.yMin+18;

                    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_LIGHT_SKY_BLUE);
                    Graphics_fillRectangle(&g_sContext, &rect);
                }


            }
        }
        break;



    }
}





