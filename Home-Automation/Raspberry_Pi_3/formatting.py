#!/usr/bin/env python3
# formatting.py

import lightControl as lc

def formatMoveCommand():
    global cursorX
    global cursorY

    return "move " + str(lc.cursorX) + " " + str(lc.cursorY) + ":"

def formatOnCommand():
    global cursorX
    global cursorY

    return "on " + str(lc.cursorX) + " " + str(lc.cursorY) + ":"

def formatOffCommand():
    global cursorX
    global cursorY
 
    return "off " + str(lc.cursorX) + " " + str(lc.cursorY) + ":"

def formatLampCommand():
    global lampStatus

    if lc.lampStatus == 0:
        return "lamp off:"
    elif lc.lampStatus == 1:
        return "lamp on:"
