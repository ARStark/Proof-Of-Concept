#!/usr/bin/env python3

import lightControl as lc
import formatting as f
import validation as valid
import evdev
from bluezero import microbit
from evdev import InputDevice, categorize, ecodes
import time

ubit = microbit.Microbit(adapter_addr='B8:27:EB:E8:AD:4D',
                         device_addr='FA:33:DE:18:91:F6',
                         accelerometer_service=False,
                         button_service=False,
                         led_service=False,
                         magnetometer_service=False,
                         pin_service=False,
                         temperature_service=False,
                         uart_service=True)

device = evdev.InputDevice('/dev/input/event6')

ubit.connect()

command = ' '

for event in device.read_loop():
    
    if valid.isValidEventType(event):
        
        if valid.isValidEventCode(event):
            
            if valid.isXPressed(event):
                command = f.formatOnCommand()
                ubit.uart = command
                
            elif valid.isTrianglePressed(event):
                command = f.formatOffCommand()
                ubit.uart = command
                
            elif valid.isSquarePressed(event):
                lc.toggleLamp()
                command = f.formatLampCommand()
                ubit.uart = command
                
            elif valid.isDownPressed_DPad(event):
                lc.moveCursorDown()
                command = f.formatMoveCommand()
                ubit.uart = command
                
            elif valid.isUpPressed_DPad(event):
                lc.moveCursorUp()
                command = f.formatMoveCommand()
                ubit.uart = command
                
            elif valid.isRightPressed_DPad(event):
                lc.moveCursorRight()
                command = f.formatMoveCommand()
                ubit.uart = command
                
            elif valid.isLeftPressed_DPad(event):
                lc.moveCursorLeft()
                command = f.formatMoveCommand()
                ubit.uart = command
           
            elif valid.isOptionsPressed(event):
                ubit.disconnect()
                quit()
