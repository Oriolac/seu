#include <ArduinoJson.h>;

char msg[100];
DynamicJsonDocument dataproducer1(256);
DynamicJsonDocument dataproducer2(256);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial.println("Start up");
  bool esp_connected = false;
  while (!esp_connected){
    Serial.println("Retrying...");
    Serial1.write("0");
    delay(2000);
    if (Serial1.read() != -1){
      esp_connected = true;
    }
  }
}

void loop() {

  Serial.println("Stated arduino comunication");
  //GET DATAPRODUCER1 INFO
  Serial1.write("1");
  Serial.println("Sended request for dataproducer1 info");
  while(!Serial1.available()){}
  deserializeJson(dataproducer1, Serial1);
  Serial.println("Got Response");
  
  //GET DATAPRODUCER2 INFO
  Serial1.write("2");
  Serial.println("Sended request for dataproducer1 info");
  while(!Serial1.available()){}
  deserializeJson(dataproducer2, Serial1);
  Serial.println("Got Response");

  serializeJson(dataproducer1, Serial);
  serializeJson(dataproducer2, Serial);

  delay(2000);

}
