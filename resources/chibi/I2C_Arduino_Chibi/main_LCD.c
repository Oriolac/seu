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
#include "chprintf.h"

static WORKING_AREA(waThread_LCD, 128);
static msg_t Thread_LCD(void *p) {
  (void)p;
  chRegSetThreadName("SerialPrint");
  uint16_t iteration=0;
  while (TRUE) {
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x20);
    chThdSleepMilliseconds(10);
    
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x20);
    chThdSleepMilliseconds(10);  
    
    chprintf((BaseSequentialStream *)&SD1, "Iter.: %u", iteration);
    iteration++;
    chThdSleepMilliseconds(2000);
  }
  return 0;
}


/*
 * Application entry point.
 */
int main(void) {
  halInit();
  chSysInit();

  char txbuf[]= "Hello Chibi-World";
  
  // Initialize Serial Port
  sdStart(&SD1, NULL); 

  // First Message
  chprintf((BaseSequentialStream *)&SD1, "Main (SD1 started)"); 
   
  // Coordinates
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x18);
  sdPut(&SD1, (uint8_t)0x00);
  chThdSleepMilliseconds(10);
   
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x19);
  sdPut(&SD1, (uint8_t)0x0A);
  chThdSleepMilliseconds(10); 

  // Second message
  chprintf((BaseSequentialStream *)&SD1, txbuf);
  
  // Start thread
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), HIGHPRIO, Thread_LCD, NULL);

  /*
   * Events servicing loop.
   */
  chThdWait(chThdSelf());

  return 0;
}
