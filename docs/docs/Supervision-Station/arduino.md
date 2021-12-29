# Arduino

The arduino is between the RPi, which is connected to the LCD Screen and the ESP01, which is the data consumer in the MQTT system.

At first it will be discussed the I2C protocol implementation with an example. Then, it will be explained different approaches that we tried for the connection between ESP01 and Arduino. 

## I2C Protocol for RPi B connection
```c
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
```

## ESP01 connection
At first we tried to use AT commands, as we thought that it was easier than programming a whole protocol. But, after having some troubles to make the ESP01 work against the firmware, and having already a subscriber for the mqtt broker for the ESP01, we made a simple protocol from reading that corresponds as this:
```txt
'0' -> "test"
'1' -> dataproducer1 JSON
'2' -> dataproducer2 JSON
'j' -> all the state as a JSON

```
The options '0' and 'j' are meant to be used for debugging, and the options '1' and '2' are meant to be used by the arduino. This messages are terminated with a ';' for reasons discussed later.

### The arduino code
The arduino has quite some troubles for parsing the data, as, sometimes, ESP01 adds some noise for the lasts characters. We think that the print library sends the entire size of the array instead of a marked '\0' character array. For this reason, we added a peculiarity: it reads until the character ';', then it discards the rest of the buffer, as the Arduino didn't request for more characters.

The example for this code is:
```cpp
#include <ArduinoJson.h>;

char msg[512];
DynamicJsonDocument dataproducer1(256);
DynamicJsonDocument dataproducer2(256);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(100);
  Serial.println("Start up");
  while (Serial1.available()>0){}
  Serial1.read();
}

void readDataproducer(DynamicJsonDocument dataproducer) {
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
  deserializeJson(dataproducer, msg);
  Serial.println("Dataproducer X = ");
  serializeJson(dataproducer, Serial);
  Serial.println();
  char c;
  while(Serial1.available() > 0) { Serial1.readBytes(&c, 1); }
}

void loop() {
  Serial.println("Stated arduino comunication");
  //GET DATAPRODUCER1 INFO
  Serial1.write("1");
  Serial.println("Sended request for dataproducer1 info");
  readDataproducer(dataproducer1);
  //GET DATAPRODUCER2 INFO
  Serial1.write("2");
  Serial.println("Sended request for dataproducer2 info");
  readDataproducer(dataproducer2);
  delay(2000);

}

```

## Merge of both codes for Arduino Mega
```cpp
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
  delay(2000);

}

```

## Merge of both codes for Arduino Uno
```cpp
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

```
