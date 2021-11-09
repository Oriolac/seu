# Accelerometer

In this document, the accelerometer "ADXL345" will be discussed. At first it will be discussed the libraries necessary for programming it. The document will continue with the initialization and it will conclude with an example.

## Instalation
To install, please install the libraries `Adafruit ADXL345` and `Adafruit Unified Sensor` in the library gestor.

## Initialization
```cpp

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
void setup() {
  if(!accel.begin()) // needed for detecting the ADXL345 at the i2c
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1) delay(100);
  }
  accel.setRange(ADXL345_RANGE_16_G); // sets the communication width
 
}

```

## Example
```cpp
#include <Adafruit_Sensor.h>

#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);


void setup()
{
  Serial.begin(9600);
  Serial.println("Initializating sensor");
  if(!accel.begin()) {
    /* There was a problem detecting the ADXL345 ... 
    check your connections */
    Serial.println(
      "Ooops, no ADXL345 detected ... Check your wiring!"
      );
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

}

void loop()
{
    Serial.println("============= Processing event =============");
    sensors_event_t event; 
    accel.getEvent(&event);
    Serial.print("X: "); 
    Serial.print(event.acceleration.x); Serial.print("  ");
    Serial.print("Y: "); Serial.print(event.acceleration.y);
    Serial.print("  ");
    Serial.print("Z: "); Serial.print(event.acceleration.z); 
    Serial.print("  ");Serial.println("m/s^2 ");
    delay(500);
}

```
