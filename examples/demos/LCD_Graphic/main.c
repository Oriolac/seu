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
#include "chvt.h"
#include <string.h>

#define N 10

#define TEMPERATURE_STATE 0
#define HUMIDITY_STATE 1
#define HEAT_INDEX 2
#define ACCEL_STATE_X 3
#define ACCEL_STATE_Y 4
#define ACCEL_STATE_Z 5

#define MAX_DATA_UNITS 20
#define START_DRAW_X 0x1D
#define MAX_X 0x79
#define MAX_Y_GRAPH 0x31

#define GET_VALUE(field, comparation)        \
  res = data_units[0].field;                 \
  for (int i = 1; i < length; i++)           \
  {                                          \
    if (data_units[i].field comparation res) \
      res = data_units[i].field;             \
  }

static const uint8_t arduino_address = 0x04;

struct data_unit_t
{
  float humidity;
  float temperature;
  float heat_index;
  float accelerometerX;
  float accelerometerY;
  float accelerometerZ;
};

typedef struct
{
  float humidity;
  float temperature;
  float heatIndex;
} dataproducer1;

typedef struct
{
  float x;
  float y;
  float z;
} dataproducer2;

struct data_unit_t data_units[MAX_DATA_UNITS];
int length;
int num_dtp;
struct data_t *data;
dataproducer2 dt2;
msg_t msgDt2;

dataproducer1 dt1;
msg_t msgDt1;

float get_attribute(int state, int i);
void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void add_struct_data_unit(int state);
void display_float(float f);
void display_int(int i);
void set_position(uint8_t x, uint8_t y);
void clear_screen(void);
void set_x(uint8_t x);
void set_y(uint8_t y);
char *get_title(int state);
void add_data(float humidity, float temperature, float heat_index, float accelX, float accelY, float accelZ);
void shift_data(void);
float get_min_value(int state);
float get_max_value(int state);
void printDataDt1(void);

static WORKING_AREA(waThread_LED1, 128);
static WORKING_AREA(waThread_LCD, 128);
static WORKING_AREA(waThread_I2C, 256);

static msg_t Thread_LCD(void *p)
{
  (void)p;
  chRegSetThreadName("SerialPrint");
  char *title;
  int state = TEMPERATURE_STATE;
  while (TRUE)
  {
    float min_val = get_min_value(state);
    float max_val = get_max_value(state);
    set_position(0x00, 0x3D);
    title = get_title(state);
    chprintf((BaseSequentialStream *)&SD1, "%s %d:", title, num_dtp);
    chThdSleepMilliseconds(10);
    drawLine(START_DRAW_X, 0x00, START_DRAW_X, 0x31);
    set_position(0x00, 0x31);
    display_float(max_val);
    set_position(0x00, 0x07);
    display_float(min_val);
    max_val++;
    min_val--;
    for (int i = length - 1; i > 0; i--)
    {
      float last = get_attribute(state, i);
      float start = get_attribute(state, i - 1);
      int x_offset = MAX_DATA_UNITS - length + i;
      int x_last = START_DRAW_X + x_offset * 5;
      int x_start = START_DRAW_X + (x_offset - 1) * 5;
      int y_last = (last - min_val) / (max_val - min_val) * MAX_Y_GRAPH;
      int y_start = (start - min_val) / (max_val - min_val) * MAX_Y_GRAPH;
      drawLine(x_start, y_start, x_last, y_last);
    }
    state = (state + 1) % ACCEL_STATE_Z;

    chThdSleepMilliseconds(3000);
    clear_screen();
  }
  return 0;
}

float get_attribute(int state, int i)
{
  switch (state)
  {
  case TEMPERATURE_STATE:
    return data_units[i].temperature;
  case HUMIDITY_STATE:
    return data_units[i].humidity;
  case HEAT_INDEX:
    return data_units[i].heat_index;
  case ACCEL_STATE_X:
    return data_units[i].accelerometerX;
  case ACCEL_STATE_Y:
    return data_units[i].accelerometerY;
  default:
    return data_units[i].accelerometerZ;
  }
}

void display_int(int i)
{
  chprintf((BaseSequentialStream *)&SD1, "%d", (int)i);
  chThdSleepMilliseconds(500);
}

void display_float(float f)
{
  chprintf((BaseSequentialStream *)&SD1, "%f", (float)f);
  chThdSleepMilliseconds(500);
}

float get_min_value(int state)
{
  float res = -1;
  switch (state)
  {
  case TEMPERATURE_STATE:
    GET_VALUE(temperature, <);
    break;
  case HUMIDITY_STATE:
    GET_VALUE(humidity, <);
    break;
  case HEAT_INDEX:
    GET_VALUE(heat_index, <);
    break;
  case ACCEL_STATE_X:
    GET_VALUE(accelerometerX, <);
    break;
  case ACCEL_STATE_Y:
    GET_VALUE(accelerometerY, <);
    break;
  default:
    GET_VALUE(accelerometerZ, <);
    break;
  }
  return res;
}

float get_max_value(int state)
{
  float res = -1;
  switch (state)
  {
  case TEMPERATURE_STATE:
    GET_VALUE(temperature, >);
    break;
  case HUMIDITY_STATE:
    GET_VALUE(humidity, >);
    break;
  case HEAT_INDEX:
    GET_VALUE(heat_index, >);
    break;
  case ACCEL_STATE_X:
    GET_VALUE(accelerometerX, >);
    break;
  case ACCEL_STATE_Y:
    GET_VALUE(accelerometerY, >);
    break;
  default:
    GET_VALUE(accelerometerZ, >);
    break;
  }
  return res;
}

char *get_title(int state)
{
  switch (state)
  {
  case TEMPERATURE_STATE:
    return "Temperature";
  case HUMIDITY_STATE:
    return "Humidity";
  case HEAT_INDEX:
    return "Heat index";
  case ACCEL_STATE_X:
    return "Accelerometer x";
  case ACCEL_STATE_Y:
    return "Accelerometer y";
  default:
    return "Accelerometer z";
  }
}

void receiveSilentDt2(void)
{
  const uint8_t t = 0x01;
  msgDt2 = i2cMasterTransmitTimeout(&I2C0, arduino_address, &t, 1, (uint8_t *)&dt2,
                                    sizeof(dataproducer2), MS2ST(1000));
  chThdSleepMilliseconds(3500);
}

void receiveSilentDt1(void)
{
  const uint8_t t = 0x00;
  msgDt1 = i2cMasterTransmitTimeout(&I2C0, arduino_address, &t, 1, (uint8_t *)&dt1,
                                    sizeof(dataproducer1), MS2ST(1000));
  chThdSleepMilliseconds(3500);
}

static msg_t Thread_LED1(void *p)
{
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE)
  {
    printDataDt1();
    chThdSleepMilliseconds(3500);
    // chThdYield();
  }
  return 0;
}

void printDataDt1(void)
{
  add_data(dt1.humidity, dt1.temperature, dt1.heatIndex, dt2.x, dt2.y, dt2.z);
  switch (msgDt1)
  {
  case Q_TIMEOUT:
    break;
  case Q_OK:
    break;
  case Q_RESET:
    chprintf((BaseSequentialStream *)&SD1, "Reset: %d", msgDt1);
    i2cflags_t i2cFlags = i2cGetErrors(&I2C0);
    chprintf((BaseSequentialStream *)&SD1, "Flags: %d", i2cFlags);
    I2CConfig i2cConfig;
    i2cStop(&I2C0);
    i2cStart(&I2C0, &i2cConfig);
    break;
  default:
    break;
  }
}

static msg_t Thread_I2C(void *p)
{
  (void)p;
  chRegSetThreadName("Read all the information in I2C");
  dt1.humidity = 0.33f;
  dt1.temperature = 2.33f;
  dt1.heatIndex = 0.55f;
  while (TRUE)
  {
    receiveSilentDt1();
    receiveSilentDt2();
    // receiveSilently12Chars();
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

  length = 0;

  sdStart(&SD1, NULL);

  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1,
                    NULL);

  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), HIGHPRIO, Thread_LCD, NULL);

  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), ABSPRIO, Thread_I2C,
                    NULL);

  chThdWait(chThdSelf());

  return 0;
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

void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  //draws a line from two given points. You can set and reset just as the pixel function.
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, (uint8_t)0x0C); //CTRL l
  sdPut(&SD1, (uint8_t)x1);
  sdPut(&SD1, (uint8_t)y1);
  sdPut(&SD1, (uint8_t)x2);
  sdPut(&SD1, (uint8_t)y2);
  sdPut(&SD1, (uint8_t)0x01);
  chThdSleepMilliseconds(10);
}

void clear_screen(void)
{
  sdPut(&SD1, (uint8_t)0x7C);
  sdPut(&SD1, 0x00);
}

void add_data(float humidity, float temperature, float heat_index, float accelX, float accelY, float accelZ)
{
  if (length < MAX_DATA_UNITS)
  {
    struct data_unit_t data_unit;
    memset(&data_unit, 0, sizeof(struct data_unit_t));
    data_unit.humidity = humidity;
    data_unit.temperature = temperature;
    data_unit.heat_index = heat_index;
    data_unit.accelerometerX = accelX;
    data_unit.accelerometerY = accelY;
    data_unit.accelerometerZ = accelZ;
    data_units[length] = data_unit;
    length++;
  }
  else
  {
    shift_data();
    data_units[MAX_DATA_UNITS - 1].humidity = humidity;
    data_units[MAX_DATA_UNITS - 1].temperature = temperature;
    data_units[MAX_DATA_UNITS - 1].heat_index = heat_index;
    data_units[MAX_DATA_UNITS - 1].accelerometerX = accelX;
    data_units[MAX_DATA_UNITS - 1].accelerometerY = accelY;
    data_units[MAX_DATA_UNITS - 1].accelerometerZ = accelZ;
  }
}

void shift_data(void)
{
  for (int i = 1; i < MAX_DATA_UNITS; i++)
  {
    data_units[i - 1] = data_units[i];
  }
}