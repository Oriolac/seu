/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
//#include "chtime.h"
#include "hal.h"


static virtual_timer_t led_vt;
static toggle = 0;

static void led_cb(void *arg){

  if (toggle){
    palSetPad(GPIO25_PORT, GPIO25_PAD);
  } else {
      palClearPad(GPIO25_PORT, GPIO25_PAD);
    }
  
  toggle=toggle^1;
 
  chSysLockFromISR();
  chVTSetI(&led_vt,MS2ST(500),led_cb,NULL);
  chSysUnlockFromISR();
}

/*
 * Application entry point.
 */
void main(void) {
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);

  chVTObjectInit(&led_vt);
  
  chVTSet(&led_vt,MS2ST(500),led_cb,NULL);
  
  // Blocks until finish
  chThdWait(chThdSelf());

}
