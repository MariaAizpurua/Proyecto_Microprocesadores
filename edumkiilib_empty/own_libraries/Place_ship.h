/*
 * Place_ship.h
 *
 *  Created on: 14 Nov 2026
 *      Author: jonxa
 */

#ifndef HAL_PLACE_SHIP_H_
#define HAL_PLACE_SHIP_H_

#include <msp430.h>
#include <stdint.h>
#include <grlib.h>
#include <../drivers/joystick.h>
#include <../hal/hal_digital_input.h>

#include "Pantalla_drw.h"

void place_ship(Graphics_Context *g_sContext);

#endif /* HAL_PLACE_SHIP_H_ */
