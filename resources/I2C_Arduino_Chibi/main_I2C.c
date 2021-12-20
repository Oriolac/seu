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

static const uint8_t slave_address = 0x04;


static WORKING_AREA(waThread_I2C, 128);
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("SerialPrintI2C");
  uint8_t request[]={0,0};
  uint8_t result=0;
  msg_t status;
  
  // Some time to allow slaves initialization
  chThdSleepMilliseconds(2000);
  
  while (TRUE) {

    // Request values
    i2cMasterTransmitTimeout(
                        &I2C0, slave_address, request, 2, 
                        &result, 1, MS2ST(1000));
    chThdSleepMilliseconds(10);
     
    sdPut(&SD1, (int8_t)0x7C);
    sdPut(&SD1, (int8_t)0x18);
    sdPut(&SD1, (int8_t)0x00);
    chThdSleepMilliseconds(10);
    
    sdPut(&SD1, (int8_t)0x7C);
    sdPut(&SD1, (int8_t)0x19);
    sdPut(&SD1, (int8_t)0x20);
    chThdSleepMilliseconds(10);
      
    chprintf((BaseSequentialStream *)&SD1, "Aval. %ux%u: %u  ", 
                                     request[0],request[1], result);
    request[1]++;
    if (request[1]>10) {
      request[1] = 0;
      request[0]++;
    }
      
    chThdSleepMilliseconds(2000);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

  // Initialize Serial Port
  sdStart(&SD1, NULL); 
  
  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);
   
  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), 
                                          HIGHPRIO, Thread_I2C, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
