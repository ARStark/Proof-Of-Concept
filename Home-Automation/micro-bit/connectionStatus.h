/**
    For all things that happen while a device is connected to 
    the microbit.
*/

#ifndef __CONNECTION_STATUS_
#define __CONNECTION_STATUS_


/* header files */
#include "MicroBit.h"
#include "connectionStatus.h"
#include "lampControl.h"
#include "ledControl.h"
#include "matrixOperations.h"
#include "validationFunctions.h"


/* macros */
#define CONFIRM_ON              "on"
#define CONFIRM_OFF             "off"
#define CONFIRM_LAMP            "lamp"
#define CONFIRM_MOVE            "move"
#define MESSAGE_ROW_INDEX_ON    3
#define MESSAGE_COL_INDEX_ON    5
#define MESSAGE_ROW_INDEX_OFF   4
#define MESSAGE_COL_INDEX_OFF   6
#define MESSAGE_ROW_INDEX_MOVE  5
#define MESSAGE_COL_INDEX_MOVE  7


/* variables */
int isConnected=0; 
int row=0; 
int col=0;
int previous_row=0;
int previous_col=0;
int temp_row=0;
int temp_col=0;

/* function prototypes */
void onConnected(MicroBitEvent e);
void onDisconnected(MicroBitEvent e);


/* functions declarations */
void onConnected(MicroBitEvent e)
{
    showConnected();
    isConnected=true;

    // my client will send ASCII strings terminated with the colon character
    ManagedString eom(":");

    while (isConnected == true)
    {
        ManagedString message = uart->readUntil(eom);
        
        // check to see if message for moving cursor is valid length
        // valid move message == 8
        if (isMoveMessageLengthValid(message))
        {
            // checks to see if the command sent is a move command
            if(isMoveMessage(message) == CONFIRM_MOVE)
            {
                // store current values of row and col
                // into temporary variables.
                // Will be used to hold previous row and col
                // values.
                temp_row = row;
                temp_col = col;
                
                // pulls row and column values from string
                // stores ASCII value of digit pulled
                row = getMatrixRow(message, MESSAGE_ROW_INDEX_MOVE);
                col = getMatrixCol(message, MESSAGE_COL_INDEX_MOVE);
                
                // sets previous rol and col values
                previous_row = temp_row;
                previous_col = temp_col;
                
                // checks to see if row and column values are valid digits
                if(isValidRowAndCol(row, col))
                { 
                    // converts ASCII values from get commands to actual 
                    // represented integer values. 
                    // turns LED at specified location on
                    row = setMatrixRow(row);
                    col = setMatrixCol(col);
                    turnLedOn(row, col);
                    
                    // if the previous LED was on before keep the LED 
                    // on once the cursor moves position
                    if (isPreviousLedOn(previous_row, previous_col))
                    {
                        turnLedOff(previous_row, previous_col);
                    }
                }
            }
        }
                
        // check to see if message for led control is valid length
        // valid on message == 6
        // valid off message == 7
        if (isLedMessageLengthValid(message)) 
        {
            // checks to see if the command sent is an on command
            if (isLedMessageOn(message) == CONFIRM_ON) 
            {   
                // pulls row and column values from string
                // stores ASCII value of digit pulled
                row = getMatrixRow(message, MESSAGE_ROW_INDEX_ON);
                col = getMatrixCol(message, MESSAGE_COL_INDEX_ON);
                
                // checks to see if row and column values are valid digits
                if(isValidRowAndCol(row, col))
                { 
                    // converts ASCII values from get commands to actual 
                    // represented integer values. 
                    // turns LED at specified location on
                    row = setMatrixRow(row);
                    col = setMatrixCol(col);
                    turnLedOn(row, col);   
                    
                    // update current status of LED
                    currentLedStatus[row][col] = true;
                }
            }
            
            //checks to see if the command sent is an off command
            else if (isLedMessageOff(message) == CONFIRM_OFF)
            {   
                // pulls row and column values from string
                // stores ASCII value of digit pulled
                row = getMatrixRow(message, MESSAGE_ROW_INDEX_OFF);
                col = getMatrixCol(message, MESSAGE_COL_INDEX_OFF);
                
                // checks to see if row and column values are valid digits
                if(isValidRowAndCol(row, col))
                {   
                    // converts ASCII values from get commands to actual 
                    // represented integer values. 
                    // turns LED at specified location off
                    row = setMatrixRow(row);
                    col = setMatrixCol(col);
                    turnLedOff(row, col);
                    
                    // update current status of LED
                    currentLedStatus[row][col] = false;
                }
            } 
        }
        
        // check to see if message for lamp control is valid length
        // valid on message == 7
        // valid off message == 8
        if(isLampMessageLengthValid(message))
        {    
            // checks to see if the command sent is a lamp command
            if(isLampMessage(message) == CONFIRM_LAMP)
            {
                // checks to see if the command sent is an on command
                if (isLampMessageOn(message) == CONFIRM_ON)
                {
                    turnLampOn();
                }
                
                // checks to see if the command sent is an off command
                else if (isLampMessageOff(message) == CONFIRM_OFF)
                {
                    turnLampOff();
                }
            }
        }
    } // end while isConnected
} // end onConneted()


void onDisconnected(MicroBitEvent e)
{
    showDisconnected();
    isConnected=false;
}// end onDisconnected


#endif


