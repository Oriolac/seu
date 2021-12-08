#include "Wire.h"

const int ARDUINO_ADDR = 0x04;


typedef struct {
  float humidity;
  float temperature;
  float theOtherOne;
} dataproducer1;


dataproducer1 data;

void wireSendData() {
  char c = 'a';
  Wire.write((byte *)&data, sizeof(dataproducer1));
  data.humidity = 0.2;
  data.temperature = 23.451;
  data.theOtherOne = 25.303;
  Serial.print("Dataproducer(humidity=");
  Serial.print(data.humidity);
  Serial.print(", temperature=");
  Serial.print(data.temperature);
  Serial.print(", theOtherOne=");
  Serial.print(data.theOtherOne);
  Serial.println(");");
  Serial.println(sizeof(dataproducer1));
  }

void sendCharacter() {
  char c = 'a';
  Wire.write((byte *)&c, sizeof(char));
  Serial.println("Sending data...");

}

void sendDataproducer() {
  sendCharacter();
}

void setup() {
  Serial.begin(9600);
  Wire.begin(ARDUINO_ADDR);
  Wire.onRequest(sendDataproducer);
}

void loop() {
  Serial.println("Arduino is working!");
  delay(10000);
}
