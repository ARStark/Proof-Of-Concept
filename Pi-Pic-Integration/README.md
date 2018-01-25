# Pi - Pic Integration

An implemetation of a very simple communication protocol between a Raspberry Pi 3 and PIC32MX250F128B.

*Pi-to--Pic.py* is the file that runs on the Raspbeery Pi 3. It uses a Playstation 3 DualShock3 controller to send commands to the Pic over a serial port.

*main.c* is the main file that parses the information sent from the Raspberry Pi 3 into commands to execute on the Pic.

*config.h* sets up the Pic's CPU clock speed, peripheral bus speed, macros for enabling UART and Vref pins, and baud rate for serial communication.

*pt_cornell_bluetooth.h* is the threading library Protothreads. To read more click [here](http://people.ece.cornell.edu/land/courses/ece4760/PIC32/index_Protothreads.html).
