
#include "DHT.h"

#define maxX 127//159 
#define maxY 63 //127


DHT dht(22, DHT11);

/* OUR FUNCTIONS */
void setX(byte posX) //0-127 or 0-159 pixels
{
  //Set the X position 
  Serial1.write(0x7C);
  Serial1.write(0x18);//CTRL x
  Serial1.write(posX);
}

//-------------------------------------------------------------------------------------------
void clearScreen()
{
  //clears the screen, you will use this a lot!
  Serial1.write(0x7C);
  Serial1.write((byte)0); //CTRL @
  //can't send LCD.write(0) or LCD.write(0x00) because it's interprestted as a NULL
}
void setY(byte posY)//0-63 or 0-127 pixels
{
  //Set the y position 
  Serial1.write(0x7C);
  Serial1.write(0x19);//CTRL y
  Serial1.write(posY);
  
}

void setHome() {
  setX((byte)0);
  setY((byte)0);
}


void setup() {
  dht.begin();
  Serial1.begin(115200);
  delay(1200);

  clearScreen();
  setHome();//set the cursor back to 0,0.
  delay(10);

  Serial1.print("Starting dataproducer1");
  delay(1000);

}

void tempAndHumidity(float tempC, float humidity, float heatIndex) {
  //This function shows how you could read the data from a temerature and humidity
  //sensor and then print that data to the Graphic LCD.

  clearScreen();
  //these could be varaibles instead of static numbers

  Serial1.print("Temperature = ");
  Serial1.print(tempC);
  Serial1.print("C ");
  Serial1.print("Humidity = ");
  Serial1.print(humidity);
  Serial1.print("%    ");
  Serial1.print("Heat index = ");
  Serial1.print(heatIndex);
  Serial1.print("C ");

}


void loop() {
  // put your main code here, to run repeatedly:
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));

  } else {
    float hic = dht.computeHeatIndex(t, h, false);
    tempAndHumidity(t, h, hic);
  }
  // Compute heat index in Celsius (isFahreheit = false)
  delay(2500);

}
