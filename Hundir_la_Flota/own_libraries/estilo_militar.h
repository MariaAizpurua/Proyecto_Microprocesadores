/*
 * estilo_militar.h
 * Interfaz de las pantallas y estilos del juego.
 */

#ifndef ESTILO_MILITAR_H_
#define ESTILO_MILITAR_H_

#include <stdint.h>
#include <stdbool.h>

#define MENU_NUM_OPCIONES  3   /* 0 estadisticas, 1 musica, 2 volver */

/* Colores e icono de una pantalla de mensaje */
typedef enum
{
    ESTILO_NORMAL,     /* placa oliva, estrellas            */
    ESTILO_AGUA,       /* placa azul, olas                  */
    ESTILO_TOCADO,     /* placa roja, diana                 */
    ESTILO_HUNDIDO,    /* placa dorada, estrellas           */
    ESTILO_GANADO,     /* placa dorada, estrellas           */
    ESTILO_PERDIDO,    /* placa roja, diana                 */
    ESTILO_AVISO       /* placa roja, triangulo de aviso    */
} estilo_pantalla_t;

/* Menu */
void menu_militar_dibujar(uint8_t opcion, bool musica_activa);
void menu_militar_placa(uint8_t indice, bool seleccionada, bool musica_activa);

/* Pantalla intermedia: titulo en la placa, detalle debajo del icono y
 * la instruccion en el pie. Cualquier texto puede ser "". */
void pantalla_militar(const char *titulo, const char *detalle,
                      const char *instruccion, estilo_pantalla_t estilo);

/* Pregunta de si/no con dos botones: S1 = SI, S2 = NO. */
void pantalla_militar_confirmar(const char *pregunta, const char *instruccion);

#endif /* ESTILO_MILITAR_H_ */
