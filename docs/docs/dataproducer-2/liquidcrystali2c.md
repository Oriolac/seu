# Liquid Crystal Display connected with i2c bus

In this document, it will be described the LCD used in the dataproducer 2. First, it will be discussed the libraries used in the application and how to install them.

Secondly it will be detailed an example about how to use it.

## Instalation
You should download a zip containing [this repository](https://github.com/fmalpartida/New-LiquidCrystal), as it is not on the library manager. Then you can install a zip using the IDE.

## Example 

```c++
#include <Wire.h>      // libreria de comunicacion por I2C
#include <LCD.h>      // libreria para funciones de LCD
#include <LiquidCrystal_I2C.h>    // libreria para LCD por I2C


LiquidCrystal_I2C lcd (0x27, 2, 1, 0, 4, 5, 6, 7); 
// Direction acces of iic, E, RW, RS, D4, D5, D6, D7 
// pins respectively

void setup()
{
  Serial.begin(9600);
  lcd.setBacklightPin(3, POSITIVE); 
  // pin P3 at PCF8574 as positive
  lcd.setBacklight(HIGH);  
  // sets so the display shows information
  lcd.begin(16, 2);
  lcd.clear(); // cleans the display
}

void loop()
{
  lcd.setCursor(0, 0);    // ubica cursor en columna 0 y linea 0
  lcd.print("Hello world!"); 
  lcd.setCursor(0, 1);
  lcd.print(milis() / 1000);
  lcd.print(" seg");// ubica cursor en columna 0 y linea 1
  delay(500);
}


```
