# Liquid Crystal Display connected with i2c bus

In this document, it will be described the LCD used in the dataproducer 2. First, it will be discussed the libraries used in the application and how to install them.

Secondly it will be detailed an example about how to use it.

## Instalation for arduino
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

## Problems with arduino and ESP01
The library seemed to hardcode the pins that  would be used
for communication. For this reason, ESP01, as it had different pins,
didn't work at all when using the library. 

Luckly for us, there was a library that already solved [this](https://github.com/agnunez/ESP8266-I2C-LCD1602). But the library itself had a weird bug for arduinos. As it seems, it's been a while since some library had a bug when printing strings: they ignored the code of the error returned by the write operation. In this case, it should return the number of bytes written, but instead, it returned always 0. For this reason, we needed to modify the library. The library with the change can be found and downloaded from [quimpm/LiquidCrystal_I2C](https://github.com/quimpm/LiquidCrystal_I2C).

## Wiring with fritzing

