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
  float heatIndex;
} dataproducer1;

typedef struct {
  float x;
  float y;
  float z;
} dataproducer2;


dataproducer2 dt2;
msg_t msgDt2;

dataproducer1 dt1;
msg_t msgDt1;

static WORKING_AREA(waThread_LED1, 128);
static WORKING_AREA(waThread_I2C, 256);

void printDataDt1(void);
void printDataDt2(void);
void display_float(float f);

void display_float(float f)
{
  chprintf((BaseSequentialStream *)&SD1, "%f", (float) f);
  chThdSleepMilliseconds(500);
}

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
    printDataDt1();
    printDataDt2();
    // chThdYield();
  }
  return 0;
}

void receiveSilentDt2(void) {
  const uint8_t  t = 0x01;
  msgDt2 = i2cMasterTransmitTimeout(&I2C0, arduino_address, &t, 1, (uint8_t *)&dt2,
                                      sizeof(dataproducer2), MS2ST(1000));
  chThdSleepMilliseconds(3500);
}


void receiveSilentDt1(void) {
  const uint8_t  t = 0x00;
  msgDt1 = i2cMasterTransmitTimeout(&I2C0, arduino_address, &t, 1, (uint8_t *)&dt1,
                                   sizeof(dataproducer1), MS2ST(1000));
  chThdSleepMilliseconds(3500);
}

void printDataDt2(void) {
  chprintf((BaseSequentialStream *)&SD1, "Dt2(");
  chprintf((BaseSequentialStream *)&SD1, "x=%f", dt2.x);
  display_float(dt2.x);
  chprintf((BaseSequentialStream *)&SD1, ", y=%f", dt2.y);
  chprintf((BaseSequentialStream *)&SD1, ", z=%f);", dt2.z);
  chprintf((BaseSequentialStream *)&SD1, "size=%d\n", sizeof(dataproducer2));
  switch (msgDt2) {
  case Q_TIMEOUT:
    chprintf((BaseSequentialStream *)&SD1, "Timeout\n");
    break;
  case Q_OK:
    chprintf((BaseSequentialStream *)&SD1, "Received Dt2\n");
    break;
  case Q_RESET:
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d\n", msgDt1);
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

}
void printDataDt1(void) {
  chprintf((BaseSequentialStream *)&SD1, "Dt1(");
  chprintf((BaseSequentialStream *)&SD1, "humidity=");
  chprintf((BaseSequentialStream *)&SD1, "%f", dt1.humidity);
  chprintf((BaseSequentialStream *)&SD1, ", temp=%f", dt1.temperature);
  chprintf((BaseSequentialStream *)&SD1, ", heatI=%f);", dt1.heatIndex);
  chprintf((BaseSequentialStream *)&SD1, "size=%d\n", sizeof(dataproducer1));
  switch (msgDt1) {
  case Q_TIMEOUT:
    chprintf((BaseSequentialStream *)&SD1, "Timeout\n");
    break;
  case Q_OK:
    chprintf((BaseSequentialStream *)&SD1, "Received Dt1\n");
    break;
  case Q_RESET:
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d\n", msgDt1);
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

}

void receiveSilently12Chars(void) {
  char data[] = "asdfqwerzxcv";
  msgDt1 = i2cMasterReceiveTimeout(&I2C0, arduino_address, (uint8_t *)&data,
                                   12, MS2ST(1000));
  chThdSleepMilliseconds(500);
  chprintf((BaseSequentialStream *)&SD1, "deb=%s", data);
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
  dt1.heatIndex = 0.55f;
  while (TRUE) {
    chprintf((BaseSequentialStream *)&SD1, "Receiving data...\n");
    receiveSilentDt1();
    receiveSilentDt2();
    // receiveSilently12Chars();
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

  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), ABSPRIO, Thread_I2C,
                    NULL);
  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
