/**
    For all things concerning the LED matrix on the 
    microbit.
*/

#ifndef __LED_CONTROL_
#define __LED_CONTROL_


/* header files */
#include "classObjects.h"


/* macros */
#define LED_ON_VALUE            255
#define LED_OFF_VALUE           0


/* function prototypes */
void showConnected();
void showDisconnected();
void turnLedOn(int row, int col);
void turnLedOff(int row, int col);


/* function declarations */
void showConnected()
{
    uBit.display.scroll("C");
}

void showDisconnected()
{
    uBit.display.scroll("D");
}

void turnLedOn(int row, int col)
{
    uBit.display.image.setPixelValue(row,col,LED_ON_VALUE);
}

void turnLedOff(int row, int col)
{
    uBit.display.image.setPixelValue(row,col,LED_OFF_VALUE);   
}


#endif