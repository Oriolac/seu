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

MUTEX_DECL(mtx1);

//static mutex_t mtx1;

static WORKING_AREA(waThread_LED1, 128);
static msg_t Thread_LED1(void *p) {
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE) {
    chMtxLock(&mtx1);
      for (int cont=0;cont<5;cont++){    
        palSetPad(GPIO25_PORT, GPIO25_PAD);
        chThdSleepMilliseconds(500);
        palClearPad(GPIO25_PORT, GPIO25_PAD);
        chThdSleepMilliseconds(500);        
      }  
    chMtxUnlock();
  }
  return 0;
}

static WORKING_AREA(waThread_LED2, 128);
static msg_t Thread_LED2(void *p) {
  (void)p;
  chRegSetThreadName("blinker-2");
  while (TRUE) {
    chMtxLock(&mtx1);
    palSetPad(GPIO18_PORT, GPIO18_PAD);
    for (int cont=0;cont<5;cont++){    
      palSetPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(100);
      palClearPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(300);
    }  
    palClearPad(GPIO18_PORT, GPIO18_PAD);
    chMtxUnlock();
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);

  chMtxInit(&mtx1);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), 
                                          NORMALPRIO, Thread_LED1, NULL);
  chThdCreateStatic(waThread_LED2, sizeof(waThread_LED2), 
                                          HIGHPRIO, Thread_LED2, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
