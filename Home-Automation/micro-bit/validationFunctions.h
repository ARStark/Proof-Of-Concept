/**
    Functions that handle validation for different
    scenarios while connected to microbit.
*/

#ifndef __VALIDATION_FUNCTIONS_
#define __VALIDATION_FUNCTIONS_


/* macros */
#define MESSAGE_START_INDEX     0
#define MESSAGE_END_INDEX_ON    2
#define MESSAGE_END_INDEX_OFF   3
#define MESSAGE_LENGTH_ON       6
#define MESSAGE_LENGTH_OFF      7
#define LAMP_ON_MESSAGE         7
#define LAMP_OFF_MESSAGE        8
#define LAMP_END_INDEX          4
#define LAMP_ON_INDEX_BEGIN     5
#define LAMP_ON_INDEX_END       7
#define LAMP_OFF_INDEX_BEGIN    5
#define LAMP_OFF_INDEX_END      8
#define MOVE_MESSAGE_LENGTH     8
#define MOVE_END_INDEX          4

/* arrarys */
extern int currentLedStatus[5][5] = {};


/* function prototypes */
int isLedMessageLengthValid(ManagedString message);
int isLampMessageLengthValid(ManagedString message);
int isMoveMessageLengthValid(ManagedString messsage);
int isValidRowAndCol(int row, int col);
int isPreviousLedOn(int previous_row, int previous_col);
ManagedString isLedMessageOn(ManagedString message);
ManagedString isLedMessageOff(ManagedString message);
ManagedString isLampMessage(ManagedString message);
ManagedString isLampMessageOn(ManagedString message);
ManagedString isLampMessageOff(ManagedString message);
ManagedString isMoveMessage(ManagedString message);


/* function declarations */
int isLedMessageLengthValid(ManagedString message){
    return (message.length() == MESSAGE_LENGTH_ON || message.length() == MESSAGE_LENGTH_OFF);
}// end isLedMessageLengthValid

int isLampMessageLengthValid(ManagedString message){
    return (message.length() == LAMP_ON_MESSAGE || message.length() == LAMP_OFF_MESSAGE);
}// end isLampMessageLengthValid

int isMoveMessageLengthValid(ManagedString message){
    return message.length() == MOVE_MESSAGE_LENGTH;
}// end isMoveMessageLengthValid()

int isValidRowAndCol(int row, int col){
    return (isdigit(row) && isdigit(col));
}// end isValidRowColValues()

int isPreviousLedOn(int previous_row, int previous_col){
    return (currentLedStatus[previous_row][previous_col] == false);
}//end isPreviousLedOn()

ManagedString isLedMessageOn(ManagedString message){
    return message.substring(MESSAGE_START_INDEX ,MESSAGE_END_INDEX_ON);     
}//end of isLedMessageOn()

ManagedString isLedMessageOff(ManagedString message){
    return message.substring(MESSAGE_START_INDEX ,MESSAGE_END_INDEX_OFF);     
}//end of isLedMessageOff()

ManagedString isLampMessage(ManagedString message){
    return message.substring(MESSAGE_START_INDEX, LAMP_END_INDEX);     
}//end of isLampMessage()

ManagedString isLampMessageOn(ManagedString message){
    return message.substring(LAMP_ON_INDEX_BEGIN, LAMP_ON_INDEX_END);     
}//end of isLampMessageOn()

ManagedString isLampMessageOff(ManagedString message){
    return message.substring(LAMP_OFF_INDEX_BEGIN, LAMP_OFF_INDEX_END);     
}//end of isLampMessageOff()

ManagedString isMoveMessage(ManagedString message){
    return message.substring(MESSAGE_START_INDEX, MOVE_END_INDEX);  
}// end isMoveMessage()


#endif