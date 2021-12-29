# ESP-01 Subscriber

This component is the one that has the responsability of subscribing to the different topics that we deffined in the MQTT broker to retrieve the information sent by the data producer.

## AT Commands Failure

As a first try to tackle this part of the supervision station, we tried to flash the AT Commands firmware that was provided into the ESP-01. Doing it this way, we would had the ESP as a slave of the arduino, leting us work and to build all the code in the arduino part.

For flashing the firmware into the ESP we used a python tool called **esptool** built by the manofacturers of the chip.

This never worked and a lot of time was expended trying to make it work. Furthermore, we tryed to send AT commands directly thorugh the serial with the ESP connected into one USB port of our computer and it did not work. For this reason we decided to implemented our own protocol.

## ESP-01 Protocol

The ESP mantains allways the latest data that has arribed thorugh te MQTT and parallely it's listening through the serials to some predefined commands that arduino sends to request the data. These commands are described in **Supervision Station/Arduino** section.

## ESP-01 code

```cpp
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Dictionary.h>

const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_server = "192.168.4.1";

const char* clientID = "consumer";
const char* clientUserName = "consumer";
const char* clientPassword = "password";
Dictionary *mapper = new Dictionary(6);
Dictionary &dataproducer1 = *(new Dictionary(3));
Dictionary &dataproducer2 = *(new Dictionary(3));
Dictionary &data = *(new Dictionary(6));
WiFiClient espClient;
PubSubClient client(espClient);
int value = 0;
char request;
String response;


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  randomSeed(micros());
}

void callback(char* topic, byte* payload, unsigned int len) {
  char msg[len+2];
  for (int i = 0; i < len; i++) {
    msg[i] = (char)payload[i];
  }
  msg[len] = ';';
  msg[len+1] = '\0';
  if(len > 21 && topic[21]  == '1'){
    data("dataproducer1", msg);
    }else if (len > 21 && topic[21] == '2' ){
    data("dataproducer2", msg);
  }
  data.remove("json");
  data("json", data.json());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    // Attempt to connect
    if (!client.connect(clientID)) {
      // Wait 5 seconds before retrying
      delay(5000);
    }else{
      Serial.write('i');
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect(clientID,clientUserName,clientPassword);
  client.subscribe("windmill/dataproducer1");
  client.subscribe("windmill/dataproducer2");
  mapper->insert("0", "ping");
  mapper->insert("1", "dataproducer1");
  mapper->insert("2", "dataproducer2");
  mapper->insert("j", "json");
  data("ping", "test");
  Serial.write('i');
}

void loop() {
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  if(Serial.available()){
    request = (char)Serial.read();
    Serial.println(data[mapper->search((String)request)]);
  }
}

```
