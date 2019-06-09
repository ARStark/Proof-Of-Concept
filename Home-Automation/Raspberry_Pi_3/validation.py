#!/usr/bin/env python3
# validation.py

from evdev import ecodes

def isValidEventType(event):
	return (event.type != 0)

def isValidEventCode(event):
	return (event.code != 0 and event.code != 2 and event.code != 4)
	
def isXPressed(event):
	return (event.code == 304 and event.value == 1)
	
def isTrianglePressed(event):
	return (event.code == 307 and event.value == 1)
	
def isSquarePressed(event):
	return (event.code == 308 and event.value == 1)
	
def isOptionsPressed(event):
	return (event.code == 315 and event.value == 1)
	
def isDownPressed_DPad(event):
	return (event.code == 17 and event.value == 1)
	
def isUpPressed_DPad(event):
	return (event.code == 17 and event.value == -1)
	
def isRightPressed_DPad(event):
	return (event.code == 16 and event.value == 1)
	
def isLeftPressed_DPad(event):
	return (event.code == 16 and event.value == -1)
	

