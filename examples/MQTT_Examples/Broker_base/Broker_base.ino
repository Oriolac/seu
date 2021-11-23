/*
   uMQTTBroker demo for Arduino

   Minimal Demo: the program simply starts a broker and waits for any client to connect.
*/

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

uMQTTBroker myBroker;

/*
   Your WiFi config here
*/
char ssid[] = "ssid";      // your network SSID (name)
char pass[] = "password"; // your network password


int counter = 0;


void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();


  WiFi.softAP(ssid, pass);
  Serial.println("AP started");
  Serial.println("IP address: " + WiFi.softAPIP().toString());

  // Start the broker
  Serial.println("Starting MQTT broker");
  myBroker.init();

  myBroker.subscribe("#");
}

char str[80];

void loop()
{
  sprintf(str, "Clients: %d", myBroker.getClientCount());
  myBroker.publish("broker/clients", str);
  // wait a second
  delay(1000);
}
