////////////////////////////////////
// clock AND protoThreads configure!
#include "config.h"
// threading library
#include "pt_cornell_bluetooth.h"

// suppress error
#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
////////////////////////////////////


static struct pt pt_BLE, pt_keyboard, pt_DMA_output, pt_input, pt_LED;

static char cmd[16];
volatile int LED_control=0;

//Controls LED based on value of LED_control variable
static PT_THREAD (protothread_LED(struct pt *pt))
{
    PT_BEGIN(pt);
    
    
        if(LED_control == 0){mPORTBSetBits(BIT_8);} // on
        if(LED_control == 1){mPORTBClearBits(BIT_8);} // off
        
        
      
    PT_EXIT(pt);
    PT_END(pt);

}

//prints to terminal and receives input from terminal
//updates LED_control based on input from terminal
static PT_THREAD (protothread_BLE(struct pt *pt))
{
    PT_BEGIN(pt);
    while(1){
        
            // send the prompt via DMA to serial
            sprintf(PT_send_buffer,"command: ");
            // by spawning a print thread
            PT_SPAWN(pt, &pt_DMA_output, PT_DMA_PutSerialBuffer(&pt_DMA_output) );
 
          //spawn a thread to handle terminal input
            // the input thread waits for input
            // -- BUT does NOT block other threads
            // string is returned in "PT_term_buffer"
            PT_SPAWN(pt, &pt_input, PT_GetSerialBuffer(&pt_input) );
            // returns when the thread dies
            // in this case, when <enter> is pushed
            // now parse the string
             sscanf(PT_term_buffer, "%s" , cmd);
             
            if (cmd[0]=='n' ) {LED_control=1;} 
            if (cmd[0]=='y' ) {LED_control=0;} 
                 
    }//end while(1)     
    PT_END(pt);

}


void main(void)
{
    mPORTBSetPinsDigitalOut(BIT_8);
    mPORTBSetBits(BIT_8); //ensures LED is turned off
    
    //initialize threads
    PT_setup();
    PT_INIT(&pt_LED);
    PT_INIT(&pt_BLE);
    
    //round-robin scheduler for threads
    while(1){
    PT_SCHEDULE(protothread_LED(&pt_LED));
    PT_SCHEDULE(protothread_BLE(&pt_BLE));
    }

}
