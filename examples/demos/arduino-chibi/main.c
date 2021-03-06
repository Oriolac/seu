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
static const uint8_t arduino_address = 0x33;
/*
** Data structures that will be used on the threads and global
** instantiation.
**
*/

typedef struct
{
  float humidity;
  float temperature;
  float theOtherOne;
} dataproducer1;

typedef struct
{
  float x;
  float y;
  float z;
} dataproducer2;

dataproducer1 dt1;
dataproducer2 dt2;

//static WORKING_AREA(waThread_LED1, 128);
static WORKING_AREA(waThread_I2C, 128);

void set_position(uint8_t x, uint8_t y);
void clear_screen(void);
void set_x(uint8_t x);
void set_y(uint8_t y);

/**
 * Thread for LED1. It is mostly used so that I know the raspberry
 * PI is doing something.
 * */
/*
static msg_t Thread_LED1(void *p)
{
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE)
  {
    palClearPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(1000);
    palSetPad(GPIO25_PORT, GPIO25_PAD);
    chThdSleepMilliseconds(1000);
  }
  return 0;
}
*/
/** Reads accelerometers information.
 *
 *
 * */
static msg_t Thread_I2C(void *p)
{
  (void)p;
  chRegSetThreadName("Read all the information in I2C");
  dt1.humidity = 0.33;
  dt1.temperature = 2.33;
  dt1.theOtherOne = 0.55;
  char c;
  while (TRUE)
  {
    // i2cMasterTransmitTimeout(&I2C0, arduino_address, NULL, 0, (uint8_t
    // *)&dt1,
    //                          sizeof(dataproducer1), MS2ST(1000));
    msg_t msg = i2cMasterTransmitTimeout(&I2C0, arduino_address, NULL, 0, (uint8_t *)&c,
                                         sizeof(char), MS2ST(5000));
    chThdSleepMilliseconds(800);
    set_position(0x00, 0x3D);
    switch (msg)
    {
    case Q_TIMEOUT:
      chprintf((BaseSequentialStream *)&SD1, "Timeout");
      break;
    case Q_OK:
      chprintf((BaseSequentialStream *)&SD1, "Received %d", c);
      break;
    case Q_RESET:
      chprintf((BaseSequentialStream *)&SD1, "Reset: %d", msg);
      i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
      set_position(0x00, 0x2D);
      chprintf((BaseSequentialStream *)&SD1, "Flags: %d", i2cFlags);
      break;
    default:
      chprintf((BaseSequentialStream *)&SD1, "Default, should not happen");
      break;
    }
    set_position(0x00, 0x07);
    chprintf((BaseSequentialStream *)&SD1, "char", c);
    chThdSleepMilliseconds(10000);
    clear_screen();
  }
  return 0;
}

/*
 * Application entry point.
 */
int main(void)
{
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);

  sdStart(&SD1, NULL);

  /** I2C initialization.
   * */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);

/*
  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1,
                    NULL);
*/
  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), HIGHPRIO, Thread_I2C,
                    NULL);
  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}

void clear_screen(void)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, 0x00);
}

void set_x(uint8_t x)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x18);
  sdPut(&SD1, (uint8_t)x);
  chThdSleepMilliseconds(10);
}

void set_y(uint8_t y)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x19);
  sdPut(&SD1, (uint8_t)y);
  chThdSleepMilliseconds(10);
}

void set_position(uint8_t x, uint8_t y)
{
  set_x(x);
  set_y(y);
}
