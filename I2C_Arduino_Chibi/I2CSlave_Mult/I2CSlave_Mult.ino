/*
 * SLAVE CONFIG
 */

#include "Wire.h"

#define SLAVE_ADDR 0x04

uint8_t value1, value2;

void receiveFunc(){
  while(Wire.available()!=2);
    value1=(uint8_t)Wire.read();
    value2=(uint8_t)Wire.read();
    Serial.print(value1);
    Serial.print("x");
    Serial.print(value2);
}

void sendFunc(){
   Serial.print("=");
   Serial.println((uint8_t)(value1*value2));
   Wire.write((uint8_t)(value1*value2));
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveFunc);
  Wire.onRequest(sendFunc);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
