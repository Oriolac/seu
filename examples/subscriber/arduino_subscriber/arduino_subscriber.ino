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
