# Pi - Pic Integration

An implemetation of a very simple communication protocol between a Raspberry Pi 3 and PIC32MX250F128B. Currently only supports from Pi to Pic communication. I currently have no need to set up communication the other way but I will implement that later. 

NOTE: you will need a USB-to-TTL cable to communicate from the Pi to the Pic. The one I use is [here](https://www.adafruit.com/product/954). This is because the Python code for the Pi 3 opens a serial port tied to the USB cable. This makes it much simpler to implement rather than messing around with the UART and 'mini' UART channels on the Pi 3. Just plug the USB into one of the Pi's USB ports and wire the cable up as laid out [here](http://people.ece.cornell.edu/land/courses/ece4760/labs/f2016/lab4.html). The wiring instructions are about right above the section titled **Concurrency**.

*Pi-to-Pic.py* is the file that runs on the Raspbeery Pi 3. It uses a Playstation 3 DualShock3 controller to send commands to the Pic over a serial port.

*main.c* is the main file that parses the information sent from the Raspberry Pi 3 into commands to execute on the Pic.

*config.h* sets up the Pic's CPU clock speed, peripheral bus speed, macros for enabling UART and Vref pins, and baud rate for serial communication.

*pt_cornell_bluetooth.h* is the threading library Protothreads. To read more click [here](http://people.ece.cornell.edu/land/courses/ece4760/PIC32/index_Protothreads.html).
