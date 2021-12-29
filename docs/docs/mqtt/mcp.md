# MCP23027

The MCP was quite easy to develop, but the cable management was hard. We would advise to follow a schematic and doing it calmly.

## Code
We used tried some libraries to work with the MCP23027, as using adafruit MCP23027, but it didn't seem to work for us. At the end, we used Wire and the plain protocol.

```cpp
/**
 * MQTT NodeMCU
 */
#include "Wire.h"

#define WIRE_SPEED 3400000

#define MCPADDRESS 0x20 // Conf LOW-LOW-LOW

// Main registers
#define IODIRA 0x00
#define IODIRB 0x01
#define IOCON  0x05
#define GPIOA  0x12
#define GPIOB  0x13
#define GPPUA  0x0C
#define GPPUB  0x0D

// Control
#define BANK   0x80 // 0b10000000 --> 1 Different banks, 0 Same bank
#define MIRROR 0x40 // 0b01000000 --> 1 Int connected, 0 Int dissociated (A/B)
#define SEQOP  0x20 // 0b00100000 --> 1 SEQ, 0 DIRECT
#define DISSLW 0x10 // 0b00010000 --> 1 Disabled
#define HAEN   0x08 // 0b00001000 --> 1 HW , 0 Disabled
#define ODR    0x04 // 0b00000100 --> 1 Open Drain
#define INTPOL 0x02 // 0b00000010 --> 1 INT Active High, INT 0 Active Low

const int NUMBER_OF_LEDS = 4;


void mcpSetUp() {
    // MCP Configuration
  Wire.beginTransmission(MCPADDRESS);
  Wire.write(IOCON);  // Sequential access - better performance
  Wire.write(SEQOP | MIRROR | HAEN );
  Wire.endTransmission();

  Wire.beginTransmission(MCPADDRESS);
  Wire.write(IODIRA);
  Wire.write(0xFF);    // A Register INPUT
  Wire.write(0x00);    // B Register OUTPUT
  Wire.endTransmission();

  Wire.beginTransmission(MCPADDRESS);
  Wire.write(GPPUA);  // PULL-UP resistors
  Wire.write(0xFF);   // A Register
  Wire.write(0xFF);   // B Register
  Wire.endTransmission();
}



void setup() {
  Serial.begin(115200); //9600bps
  while (!Serial) {}; //Serial port initialization
  Serial.println();
  Serial.println();


  // Start the broker
  Wire.setClock(WIRE_SPEED); // I2C Bus speed

  Wire.begin();     // wake up I2C bus

  mcpSetUp();

}

int toLedValue(int num) {
  return int(pow(2, num) - 0.5);
}

void writeLeds(int num) {
  Wire.beginTransmission(MCPADDRESS);
  Wire.write(GPIOB);      // B Register
  Wire.write(num);      // value to send - all HIGH
  Wire.endTransmission();
}

int clients = 0;
void loop() {
  int ledsValue = constrain(clients, 0, NUMBER_OF_LEDS);
  sprintf(str, "Set leds to: %d", ledsValue);
  myBroker.publish("debug", str);
  writeLeds(toLedValue(ledsValue));
  delay(1000);
  clients = (clients + 1) % NUMBER_OF_LEDS;
}
```
