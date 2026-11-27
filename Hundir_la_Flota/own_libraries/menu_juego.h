/*
 * menu_juego.h
 * Interfaz publica del menu del juego.
 */

#ifndef MENU_JUEGO_H_
#define MENU_JUEGO_H_

#include <stdbool.h>

/* Configura P5.5. Llamar despues de PMM_unlockLPM5(). */
void MENU_init(void);

/* true una vez por pulsacion completa de P5.5. */
bool MENU_boton_pulsado(void);

/* Muestra y controla el menu del juego. */
void MENU_abrir(void);

#endif /* MENU_JUEGO_H_ */
