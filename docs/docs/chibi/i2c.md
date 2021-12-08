# ChibiOS and requesting information on I2C
ChibiOS has a function to request the information on I2C bus, and in this document it shall be described how it works.

## The function
The function in question is:
```cpp
void i2cMasterTransmitTimeout(
    &I2C0, // I2C bus
     // the address that you are requesting information
    arduino_address,
    NULL,// the request information
    0, // size sended in the request information
    (uint8_t *)&data, // data received
    sizeof(sensor_data_t), // size of the data received
    MS2ST(1000) // I don't really know anything about this
    );
```

