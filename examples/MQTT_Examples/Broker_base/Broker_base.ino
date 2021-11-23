/*
 * uMQTTBroker demo for Arduino
 * 
 * Minimal Demo: the program simply starts a broker and waits for any client to connect.
 */

#include <ESP8266WiFi.h>
#include "uMQTTBroker.h"

uMQTTBroker myBroker;

/*
 * Your WiFi config here
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

void loop()
{   
  myBroker.publish("broker/counter", (String)counter++);

 Serial.print("Clients:");
 Serial.println(myBroker.getClientCount());
 // wait a second
 delay(1000);
}
