/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Dictionary.h>

// Update these with values suitable for your network.

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
int x;
int y;
int z;
int temp;
int hum;
int i_heat;
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
  Serial.print("Callback called with: topic=");
  Serial.print(topic);
  Serial.print(", payload=");
  char msg[len];
  for (int i = 0; i < len; i++) {
    msg[i] = (char)payload[i];
  }
  Serial.println(msg);
  Serial.println(topic);
  Serial.print("Character 21 == ");
  Serial.println(topic[21]);
  if(len > 21 && topic[21]  == '1'){
    Serial.println("Message from Dataproducer1 Recibed");
    Serial.println(msg);
    data("dataproducer1", msg);
    }else if (len > 21 && topic[21] == '2' ){
    Serial.println("Message from Dataproducer1 Recibed");
    Serial.println(msg);
    data("dataproducer2", msg);
  }
  data.remove("json");
  data("json", data.json());
  Serial.print("Data temperature:");
  Serial.println(data["temperature"]);
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  Serial.println("Connecting to mqtt");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.connect(clientID,clientUserName,clientPassword);
  client.subscribe("windmill/dataproducer1");
  client.subscribe("windmill/dataproducer2");
  Serial.println("Suscribed to topics");
  mapper->insert("0", "ping");
  mapper->insert("1", "dataproducer1");
  mapper->insert("2", "dataproducer2");
  mapper->insert("j", "json");
  data("ping", "test");
  Serial.println("Initialized mapper");
}

void loop() {
  client.loop();
  if(Serial.available()){
    request = (char)Serial.read();
    Serial.print("request = ");
    Serial.println(request);
    Serial.print("response = ");
    Serial.println(data[mapper->search((String)request)]);
    //Serial.println(response);
    //Serial.write(response.c_str());
  }
}
