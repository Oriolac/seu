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
#include "chvt.h"
#include "hal.h"

static WORKING_AREA(waThread_LED1, 128);
static msg_t Thread_LED1(void *p) {
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE) {
    palClearPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(100);
    palSetPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(900);
    //chThdYield();
  }
  return 0;
}


static WORKING_AREA(waThread_LED2, 128);
static msg_t Thread_LED2(void *p) {
  (void)p;
  chRegSetThreadName("blinker-2");
  systime_t start;
  while (TRUE) {
    palClearPad(GPIO18_PORT, GPIO18_PAD);
    start = chTimeNow();
    while (chTimeNow()-start<500);
    start = chTimeNow();
    palSetPad(GPIO18_PORT, GPIO18_PAD);
    while (chTimeNow()-start<500);
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1, NULL);
  chThdCreateStatic(waThread_LED2, sizeof(waThread_LED2), NORMALPRIO, Thread_LED2, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
