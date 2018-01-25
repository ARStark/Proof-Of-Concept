### needed for reading Linux input event devices ###
import evdev
from evdev import InputDevice, categorize, ecodes
### end imports for reading Linux input event devices ###

### needed to send data from RPi3 to Pic32 over UART ###
import serial
### end import for RPi3 to Pic32 ###

### set up input event associated with DualShock 3 controller to be read from ###
device = evdev.InputDevice('/dev/input/event2')

### set up serial port, baud rate and timeout ### 
ser=serial.Serial('/dev/ttyUSB0', 9600, timeout=0)


### read continously from DualShock3 controller. ###
for event in device.read_loop():
    if event.type != 0 and event.type != 3 and event.type != 4:
        if event.value == 1:
            if event.code == 302:
                print "X button pressed"
                ser.write('y\n')  # terminating string with \n for Protothreads to parse correctly
            elif event.code == 300:
                print "Triangle button pressed"
                ser.write('n\n')  # terminating string with \n for Protothreads to parse correctly
