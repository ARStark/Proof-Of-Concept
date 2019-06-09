#!/usr/bin/env python3
# lightControl.py

cursorX = 0
cursorY = 0
lampStatus = 0

def moveCursorUp():
    global cursorY

    cursorY -= 1
    if cursorY < 0:
        cursorY = 4

def moveCursorDown():
    global cursorY 

    cursorY += 1
    if cursorY > 4:
        cursorY = 0

def moveCursorLeft():
    global cursorX

    cursorX -= 1
    if cursorX < 0:
        cursorX = 4

def moveCursorRight():
    global cursorX 
    
    cursorX += 1
    if cursorX > 4:
        cursorX = 0
        
def toggleLamp():
    global lampStatus

    if lampStatus == 0:
        lampStatus = 1
    else:
        lampStatus = 0
