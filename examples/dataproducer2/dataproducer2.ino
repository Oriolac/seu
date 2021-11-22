/* Dataproducer 2
 * 
 * 
 */

#include <Adafruit_Sensor.h>

#include <Adafruit_ADXL345_U.h>

#include <Wire.h>      // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>    // libreria para LCD por I2C

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); // DIR, E, RW, RS, D4, D5, D6, D7

void setup()
{
  Serial.begin(9600);
  lcd.setBacklightPin(3, POSITIVE); // puerto P3 de PCF8574 como positivo
  lcd.setBacklight(HIGH);   // habilita iluminacion posterior de LCD
  lcd.begin(16, 2);     // 16 columnas por 2 lineas para LCD 1602A
  lcd.clear();      // limpia pantalla
  Serial.println("A");
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

}

void loop()
{
  Serial.println("B");
  sensors_event_t event;
  accel.getEvent(&event);
  Serial.println("Displaying information: ");
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");
  lcd.setCursor(0, 0);    // ubica cursor en columna 0 y linea 0
  lcd.print("X:");
  lcd.print(event.acceleration.x);
  lcd.print(" Y:");
  lcd.print(event.acceleration.y);// escribe el texto
  lcd.setCursor(0, 1);
  lcd.print("Z: ");
  lcd.print(event.acceleration.z);// ubica cursor en columna 0 y linea 1
  delay(500);
}
