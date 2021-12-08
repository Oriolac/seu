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
#include "chprintf.h"
#include "chvt.h"
#include "hal.h"

/*
** Constants.
*/
static const uint8_t arduino_address = 0x04;
/*
** Data structures that will be used on the threads and global
** instantiation.
**
*/

typedef struct {
  float humidity;
  float temperature;
  float theOtherOne;
} dataproducer1;

typedef struct {
  float x;
  float y;
  float z;
} dataproducer2;

dataproducer1 dt1;
dataproducer2 dt2;

static WORKING_AREA(waThread_LED1, 128);
static WORKING_AREA(waThread_I2C, 128);

/**
 * Thread for LED1. It is mostly used so that I know the raspberry
 * PI is doing something.
 * */
static msg_t Thread_LED1(void *p) {
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE) {
    palClearPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(1000);
    palSetPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(1000);
    // chThdYield();
  }
  return 0;
}
/**
 *Used for debug */
void i2cSendCharacter(void) {
  char c;
  msg_t msg = i2cMasterReceiveTimeout(&I2C0, arduino_address,
                                      (uint8_t *)&c,
                                      sizeof(char), MS2ST(1000));
  chThdSleepMilliseconds(500);

  switch (msg) {
  case Q_TIMEOUT:
    chprintf((BaseSequentialStream *)&SD1, "Timeout");
    break;
  case Q_OK:
    chprintf((BaseSequentialStream *)&SD1, "Received %d", c);
    break;
  case Q_RESET:
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d\n", msg);
    i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
    chprintf((BaseSequentialStream *)&SD1, "Flags: %d\n", i2cFlags);
    I2CConfig i2cConfig;
    i2cStop(&I2C0);
    i2cStart(&I2C0, &i2cConfig);
    break;
  default:
    chprintf((BaseSequentialStream *)&SD1, "Default, should not happen");
    break;
  }

  chprintf((BaseSequentialStream *)&SD1, "char=%d", c);
}

void receiveDt1(void) {
  dataproducer1 data;
  msg_t msg = i2cMasterReceiveTimeout(&I2C0, arduino_address,
                                      (uint8_t *)&data,
                                      sizeof(dataproducer1),
                                      MS2ST(1000));
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&SD1,
           "Dt1(");
  chprintf((BaseSequentialStream *)&SD1,
           "humidity=%f", dt1.humidity);
  chprintf((BaseSequentialStream *)&SD1,
           ", temp=%f", dt1.temperature);
  chprintf((BaseSequentialStream *)&SD1,
           ", theO=%f)\n", dt1.theOtherOne);
  chprintf((BaseSequentialStream *)&SD1, "size=%d\n", sizeof(dataproducer1));
  switch (msg) {
  case Q_TIMEOUT:
    chprintf((BaseSequentialStream *)&SD1, "Timeout\n");
    break;
  case Q_OK:
    chprintf((BaseSequentialStream *)&SD1, "Received something\n");
    break;
  case Q_RESET:
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d\n", msg);
    i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
    chprintf((BaseSequentialStream *)&SD1, "Flags: %d\n", i2cFlags);
    I2CConfig i2cConfig;
    i2cStop(&I2C0);
    i2cStart(&I2C0, &i2cConfig);
    break;
  default:
    chprintf((BaseSequentialStream *)&SD1, "Default, should not happen");
    break;
  }
  dt1 = data;
  chThdSleepMilliseconds(3000);
}

/** Reads accelerometers information.
 *
 *
 * */
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("Read all the information in I2C");
  dt1.humidity = 0.33f;
  dt1.temperature = 2.33f;
  dt1.theOtherOne = 0.55f;
  while (TRUE) {
    chprintf((BaseSequentialStream *)&SD1, "Receiving data...\n");
    i2cSendCharacter();
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

  sdStart(&SD1, NULL);

  /** I2C initialization.
   * */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1,
                    NULL);

  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), HIGHPRIO, Thread_I2C,
                    NULL);
  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
