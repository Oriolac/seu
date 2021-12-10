# Supervision
In the image below is shown the prototype proposal of the **Supervision station**.
<center>
![Prototype](img/prototype.png)
</center>

## Requirements

The Supervision station has these requirements:

- There is just only one Supervision Station for the whole WTGF.
- All sensors are placed far from the Supervision Station.
- Whole WTGF data is transferred to the Supervision Station through a MQTT broker.

## User story 
- Maintenance manager requires graphically representation of the obtained sensor data, within the last 24h, in the Supervision Station.

As the last 24 hours is hard to debug, it is shown instead the last 24 values in the code. To change this, make the delay of requesting the data for an hour or compute the median for it.

## The schema
![The schematic of the supervision station](./img/chibi-esp-arduino.png)


## ChibiOS
In order to find more information of ChibiOS, go [here](chibios.md).
