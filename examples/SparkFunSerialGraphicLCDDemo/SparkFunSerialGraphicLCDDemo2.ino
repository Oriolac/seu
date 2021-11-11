/*********************************************************
Serial Graphic LCD Library Demo
Joel Bartlett
SparkFun Electronics
October 14, 2013

License: 
This code is beerware: feel free to use it, with or without attribution, 
in your own projects. If you find it helpful, buy me a beer next time you 
see me at the local pub.

This is an example sketch to accompany the Serial Graphic LCD Library. 
This is intended for use with SparkFun's Serial Graphic LCDs: the 128x64 pixel,
the 160x128 pixel, and the Graphic LCD Serial Backpack. There is no guarantee 
that this code will work on any other LCDs or backpacks. 

This code was written with Arduino 1.0.5 (available at arduino.cc) It should 
work with any Arduino IDE version from 1.0 and on. If you are using 
Arduino 0023 or an older version, this will not compile.

Hardware:
You'll need one of the LCDs mentioned above as well as an Arduino compatible 
board running at 5V, and some jumper wires to connect the two. 

Connections are as follows:
__________________
Arduino | Backpack|
________|_________|
   5V   |  Vin    |
  GND   |  GND    |
   TX   |  RX     |
__________________

***********************************************************/

#include <SparkFunSerialGraphicLCD.h>//inculde the Serial Graphic LCD library
#include <SoftwareSerial.h>
//Despite this being inculdeing in the library file, 
//it needs to be added here as well to get the sketch to compile


//This demo code was created for both the 128x64 and the 160x128 pixel LCD. 
//Thus, these maximum limits are set to allow this code to work on both sizes of LCD.
//The default size is the 128x64, but you can delete those values and uncomment
//the other values if you have a 160x128 pixel LCD. If you are writing code for just 
//one size LCD, you can omit this information entirely and just use numerical values
//in place of these variable names. 

#define maxX 127//159 
#define maxY 63 //127
//Each maximum value is one less than the stated value to account for position 0,0
//Thus, position 127 is actually the 128th pixel. 

//Create an instance of the LCD class named LCD. We will use this instance to call all the 
//subsequent LCD functions
LCD LCD;

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup");
  //we're just going to run through a bunch of demos to show the functionality of the LCD.
  LCD.setBacklight(10);
  delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.
  
  LCD.setHome();//set the cursor back to 0,0.
  LCD.clearScreen();//clear anything that may have been previously printed ot the screen.
  delay(10);
    
  LCD.printStr("Commence Arduino Demo Mode");
  delay(1500);
  
  //Each of these functions is explained in great detail below. 
  
  tempAndHumidity();
  

  //This function should only be used if you accidentally changed the baud rate or if you forgot to what rate it was changed. 
  //LCD.restoreDefaultBaud();
  Serial.println("Finish Setup");
}
//-------------------------------------------------------------------------------------------
void loop()
{
//nothing in loop since we are just running through each demo once
  tempAndHumidity();
  delay(10000);
  Serial.println("FInish Loop");

}

//-------------------------------------------------------------------------------------------
void tempAndHumidity()
{
//This function shows how you could read the data from a temerature and humidity 
//sensor and then print that data to the Graphic LCD.
  
  LCD.clearScreen();
  LCD.printStr("Or, data from sensors");
  delay(2000);

  //these could be varaibles instead of static numbers 
  float tempF = 77.0; 
  float tempC = 25.0;
  float humidity = 67.0;
  
  LCD.clearScreen();
  
  LCD.printStr("Temp = ");
  LCD.printNum(int(tempF));
  LCD.printStr("F ");
  LCD.printNum(int(tempC));
  LCD.printStr("C");
  LCD.nextLine();
  LCD.printStr("Humidity = ");
  LCD.printNum(int(humidity)); 
  LCD.printStr("%");
  delay(2500);
}
