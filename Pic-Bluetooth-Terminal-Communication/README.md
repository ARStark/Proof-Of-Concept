# Pic32 Bluetooth Terminal Communication

An implemetation of a very simple communication protocol between a Bluetooth terminal and PIC32MX250F128B.

*main.c* is the main file that parses the information sent from the bluetooth terminal into commands to execute on the Pic as well as sending information from the Pic to the bluetooth terminal.

*config.h* sets up the Pic's CPU clock speed, peripheral bus speed, macros for enabling UART and Vref pins, and baud rate for serial communication.

*pt_cornell_bluetooth.h* is the threading library Protothreads. To read more click [here](http://people.ece.cornell.edu/land/courses/ece4760/PIC32/index_Protothreads.html).
