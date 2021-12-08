#include <ArduinoJson.h>;

char msg[100];
DynamicJsonDocument dataproducer1(256);
DynamicJsonDocument dataproducer2(256);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Start up");
  while (!Serial1.available()){}
  Serial1.read();
}

void loop() {
  char last_char = 'a';
  Serial.println("Stated arduino comunication");
  //GET DATAPRODUCER1 INFO
  Serial1.write("1");
  Serial.println("Sended request for dataproducer1 info");
  while(!Serial1.available()){}
  Serial.println("Recived a message");
  int i = 0;
  while(last_char != ';'){
    if (Serial1.available()){
      msg[i] = (char)Serial1.read();
      last_char = msg[i];
      i = i + 1;
    }
  }
  msg[i] = '\0';
  deserializeJson(dataproducer1, msg);
  Serial.println("Got Response");
  last_char = 'a';
  //GET DATAPRODUCER2 INFO
  Serial1.write("2");
  Serial.println("Sended request for dataproducer1 info");
  while(!Serial1.available()){}
  Serial.println("Recived a message");
  i = 0;
  while(last_char != ';'){
    if (Serial1.available()){
      msg[i] = (char)Serial1.read();
      last_char = msg[i];
      i = i + 1;
    }
  }
  msg[i] = '\0';
  deserializeJson(dataproducer2, msg);
  Serial.println("Got Response");
  last_char = 'a';

  serializeJson(dataproducer1, Serial);
  serializeJson(dataproducer2, Serial);

  delay(2000);

}
