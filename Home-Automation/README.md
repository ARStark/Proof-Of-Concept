# Home Automation

A home automation project for simple electronics. The project utilizes a Raspberry Pi 3, a Sony DualShock4 controller, and a BBC micro:bit. A general overview of the project is as follows: The DS4 controller sends button press data to the Pi, which is then processed. After the data has been processed, string commands are sent over bluetooth to the micro:bit which are then parsed and appropriate commands are executed. The commands "lamp on:" and "lamp off:" are responsible for controlling an LED externally wired from the micro:bit. All other commands deal with LED matrix on the micro:bit. 

The Raspberry Pi 3 is running a python script that handles the reading of input from the DS4 controller as well as the UART communication with the micro:bit. 

The DS4 controller is able to be read as a linux input device under /dev/input/. 

The micro:bit is running C++ which parses the strings sent from the Pi into working commands.

