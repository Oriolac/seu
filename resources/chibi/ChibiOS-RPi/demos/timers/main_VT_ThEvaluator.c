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
#include "hal.h"

static void led_clear(void *arg);
static void led_set(void *arg);

static VirtualTimer led_vt1, led_vt2;

static void led_clear(void *arg){
  palClearPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt1,MS2ST(500),led_set,NULL);
}

static void led_set(void *arg){
  palSetPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt2,MS2ST(500),led_clear,NULL);
} 

static WORKING_AREA(waThread1, 128);
static msg_t evaluator(void *p) {
  (void)p;
  chRegSetThreadName("Evaluator");
  while (TRUE) {
    if(chVTIsArmedI(&led_vt1)){
      palSetPad(GPIO18_PORT, GPIO18_PAD);
    } else {
        palClearPad(GPIO18_PORT, GPIO18_PAD);
      }
  }
  return 0;
}


void main(void) {
  halInit();
  chSysInit();
  
  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);
 
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, evaluator, NULL);
 
  chVTSetI(&led_vt1,MS2ST(500),led_set,NULL);
  
  chThdWait(chThdSelf());

}
