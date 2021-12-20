#include <ArduinoJson.h>;
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
char msg[512];
DynamicJsonDocument dataproducerJson1(256);
DynamicJsonDocument dataproducerJson2(256);


void sendRealdt1() {
  Wire.write((byte *) &dt1, 12);
  }
  
void sendRealdt2() {
  Wire.write((byte *) &dt2, 12);
  }

void sendCharacter() {
  char c = 'a';
  Wire.write((byte *)&c, sizeof(char));
}

void sendCharacters() {
  char c[] = "abcdefghijlmnopqrst";
  Wire.write((byte *) &c, 12 * sizeof(char));
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
  switch (t) {
    case 0x00:
    sendDt1ElseDt2 = true;
    break;
    case 0x01:
    sendDt1ElseDt2 = false;
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(ARDUINO_ADDR);
  Wire.onRequest(senddataproducer);
  Wire.onReceive(changeState);
  delay(100);
  while (Serial.available()>0){}
  Serial.read();
}

void readDataproducer(DynamicJsonDocument *dataproducer) {
  char last_char = 'a';
  while(Serial.available()<=0){}
  delay(100);
  int i = 0;
  while(last_char != ';'){
    if (Serial.available()>0){
      msg[i] = (char)Serial.read();
      last_char = msg[i];
      i = i + 1;
    }
  }
  msg[i-1] = '\0';
  delay(500);
  deserializeJson(*dataproducer, msg);
  char c;
  while(Serial.available() > 0) { Serial.readBytes(&c, 1); }
}

void loop() {
  //GET dataproducerJson1 INFO
  Serial.write("1");
  readDataproducer(&dataproducerJson1);
  dt1.humidity = dataproducerJson1["humidity"];
  dt1.temperature = dataproducerJson1["temperature"];
  dt1.heatIndex = dataproducerJson1["hic"];
  //GET dataproducerJson2 INFO
  Serial.write("2");
  readDataproducer(&dataproducerJson2);
  dt2.x = dataproducerJson2["X"];
  dt2.y = dataproducerJson2["Y"];
  dt2.z = dataproducerJson2["Z"];
  delay(2000);

}
