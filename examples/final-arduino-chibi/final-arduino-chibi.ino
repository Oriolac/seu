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
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Wire.begin(ARDUINO_ADDR);
  Wire.onRequest(senddataproducer);
  Wire.onReceive(changeState);
  delay(100);
  Serial.println("Start up");
  while (Serial1.available()>0){}
  Serial1.read();
}

void readDataproducer(DynamicJsonDocument *dataproducer) {
  char last_char = 'a';
  while(Serial1.available()<=0){}
  delay(100);
  Serial.println("Recived a message");
  int i = 0;
  while(last_char != ';'){
    if (Serial1.available()>0){
      msg[i] = (char)Serial1.read();
      last_char = msg[i];
      i = i + 1;
    }
  }
  msg[i-1] = '\0';
  delay(500);
  deserializeJson(*dataproducer, msg);
  Serial.println("Dataproducer X = ");
  serializeJson(*dataproducer, Serial);
  Serial.println();
  char c;
  while(Serial1.available() > 0) { Serial1.readBytes(&c, 1); }
}

void loop() {
  Serial.println("Stated arduino comunication");
  //GET dataproducerJson1 INFO
  Serial1.write("1");
  Serial.println("Sended request for dataproducer1 info");
  readDataproducer(&dataproducerJson1);
  dt1.humidity = dataproducerJson1["humidity"];
  dt1.temperature = dataproducerJson1["temperature"];
  dt1.heatIndex = dataproducerJson1["hic"];
  //GET dataproducerJson2 INFO
  Serial1.write("2");
  Serial.println("Sended request for dataproducer2 info");
  readDataproducer(&dataproducerJson2);
  dt2.x = dataproducerJson2["X"];
  dt2.y = dataproducerJson2["Y"];
  dt2.z = dataproducerJson2["Z"];
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
  delay(2000);

}
