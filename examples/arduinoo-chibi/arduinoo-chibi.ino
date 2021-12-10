#include "Wire.h"

const int ARDUINO_ADDR = 0x04;

typedef struct {
  float humidity;
  float temperature;
  float heatIndex;
} dataproducer1;


typedef struct{
  float x;
  float y;
  float z;
} dataproducer2;

dataproducer1 dt1;
dataproducer2 dt2;
bool sendDt1ElseDt2 = true;


void sendRealdt1() {
  Wire.write((byte *) &dt1, 12);
  Serial.println("Sending dt1...");
  }
  
void sendRealdt2() {
  Wire.write((byte *) &dt2, 12);
  Serial.println("Sending dt2...");
  }

void sendCharacter() {
  char c = 'a';
  Wire.write((byte *)&c, sizeof(char));
  Serial.println("Sending dt1...");
}

void sendCharacters() {
  char c[] = "abcdefghijlmnopqrst";
  Wire.write((byte *) &c, 12 * sizeof(char));
  Serial.println("Sending debug");
}

void senddataproducer() {
  if (sendDt1ElseDt2) {
    sendRealdt1();
  } else {
    sendRealdt2();
  }  
}


void changeState(int numBytes) {
  uint8_t t = Wire.read();
  Serial.print("msg = ");
  Serial.println(t);
  switch (t) {
    case 0x00:
    sendDt1ElseDt2 = true;
    break;
    case 0x01:
    sendDt1ElseDt2 = false;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(ARDUINO_ADDR);
  Wire.onRequest(senddataproducer);
  Wire.onReceive(changeState);
  dt1.humidity = 32.07f;
  dt1.temperature = 23.05f;
  dt1.heatIndex = -24.55;
  dt2.x = 0.23f;
  dt2.y = 9.56f;
  dt2.z = -2.035f;
  Serial.print("dataproducer(humidity=");
  Serial.print(dt1.humidity);
  Serial.print(", temperature=");
  Serial.print(dt1.temperature);
  Serial.print(", heatIndex=");
  Serial.print(dt1.heatIndex);
  Serial.print(", x=");
  Serial.print(dt2.x);
  Serial.print(", y=");
  Serial.print(dt2.y);
  Serial.print(", z=");
  Serial.print(dt2.z);
  Serial.println(");");
}

void loop() {
  Serial.println("Arduino is working!");
  delay(10000);
}
