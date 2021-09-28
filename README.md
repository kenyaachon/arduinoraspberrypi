# Temperature System

The goal of this project was to create a cooling system that would adjust to the current ambient temperature

## Hardware

2 Raspberry Pi's
Raspberry PiCam
Arduino
Fan, 4-wire type
TMP 36 - Temperature Sensor
Ethernet Cables
USB Cables

## Architecture

We set up our system using a master-slave architecture
One raspberry pi was the master, and a second raspbery pi was a slave
that served as a server for sending real-time imaging to the master.
The Arduino was also a slave, it handled the TMP36 temperature sensor and a small 4 wire Fan.

![System Diagram](https://github.com/kenyaachon/arduinoraspberrypi/blob/main/SystemDiagram.png)
