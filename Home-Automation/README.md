# Home Automation

A home automation project for simple electronics. 

NOTE: This project utilizes the [micro:bit runtime Device Abstration Layer](https://github.com/lancaster-university/microbit-dal/) developed by Lancaster University for C/C++ and the [bluezero](https://github.com/ukBaz/python-bluezero) library developed by ukBaz for Python.

The setup requires a Raspberry Pi 3, a Sony DualShock4 controller, and a BBC micro:bit. A general overview of the project is as follows: The DS4 controller sends button press data to the Pi, which is then processed. After the data has been processed, string commands are sent over bluetooth to the micro:bit which are then parsed and appropriate commands are executed. The commands **"lamp on:"** and **"lamp off:"** are responsible for controlling an LED externally wired from the micro:bit. All other commands deal with LED matrix on the micro:bit. 

The Raspberry Pi 3 is running a Python script that handles the reading of input from the DS4 controller as well as the UART communication with the micro:bit. 

The DS4 controller is able to be read as a linux input device under */dev/input/*. 

The micro:bit is executing C++ code which parses the strings sent from the Pi into working commands.

