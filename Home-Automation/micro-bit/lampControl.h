/**
    Functions that control the lamp or other
    external LED.
    
    Desgined for proof of concept with home automation.
*/

#ifndef __LAMP_CONTROL_
#define __LAMP_CONTROL_


/* header files */
#include "classObjects.h"


/* macros */
#define LAMP_PIN                0


/* function prototypes */
void turnLampOn();
void turnLampOff();


/* function declarations */
void turnLampOn()
{
    uBit.io.pin[LAMP_PIN].setDigitalValue(true);
}

void turnLampOff()
{
    uBit.io.pin[LAMP_PIN].setDigitalValue(false);
}


#endif