# Broker

We based the broker for our project as the example given by the professor. We have the libraries used by them at the libraries folder.

## Regard the Example
The example was slightly modified. It usually returned just a counter, and printed by terminal the number of clients. As we had our NodeMCU connected directly to a source of electricity and not a computer for developing both dataproducers, we modified it so instead of printing the counter, it just prints the number of clients.

Also, we used a channel for debugging purposes, and both ESP01 send the data to debug at this channels, which makes it easier to know what is happening at them at all times.

## Code

```cpp
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


void setup() {
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

void loop() {
  sprintf(str, "Clients: %d", myBroker.getClientCount());
  myBroker.publish("broker/clients", str);
  // wait a second
  delay(1000);
}

```
