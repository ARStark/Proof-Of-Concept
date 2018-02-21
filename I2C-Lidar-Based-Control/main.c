/*
 * File:        main.c
 * Author:      Anthony Stark
 * Target PIC:  PIC32MX250F128B
 * 
*/

////////////////////////////////////
// clock AND protoThreads configure!
#include "config.h"
// threading library
#include "pt_cornell_bluetooth.h"


#include "i2c_LIDAR.h" // created library i2c functions --- lidar specific version
#include "LIDAR.h" // library to hold all functions needed to setup and use lidar


// suppress error
#define _SUPPRESS_PLIB_WARNING 
#define _DISABLE_OPENADC10_CONFIGPORT_WARNING
////////////////////////////////////

//distances found when debugging sensor. Only 3 values came up. Not sure why but it works for basic use.
//will research how to get other values later
#define DISTANCE_STATUS_0 24319 
#define DISTANCE_STATUS_1 18687
#define DISTANCE_STATUS_2 8447

#define RED_LED BIT_15
#define YELLOW_LED BIT_14


static struct pt pt_serial, pt_input, pt_DMA_output, pt_sensor, pt_LED;

static char cmd[16]; //might not need this will see

static unsigned int distance=0; //global variable for distance sent from sensor;

/* thread to handle serial communication with terminal application */
//pretty much only used for debugging in this application
static PT_THREAD (protothread_serial(struct pt *pt))
{
    PT_BEGIN(pt);
    while(1){
        
       printf("distance: %d\r\n", distance); 
       PT_YIELD_TIME_msec(200);
       
    }//end while(1)     
    PT_END(pt);

}// end pt_serial

//takes values from sensor
static PT_THREAD (protothread_sensor(struct pt *pt))
{
    PT_BEGIN(pt);
    while(1){
        distance = readRangeContinuousMillimeters();
        PT_YIELD_TIME_msec(200);
    }//end while(1)
    PT_END(pt);
}//end of pt_sensor

//proof of concept thread; will extend use of distance proximity to sensor to start and stop robot autonomously 
static PT_THREAD(protothread_LED(struct pt *pt))
{
    PT_BEGIN(pt);
    if(distance == DISTANCE_STATUS_0){mPORTBSetBits(RED_LED);mPORTBClearBits(YELLOW_LED);}
    if(distance == DISTANCE_STATUS_1){mPORTBSetBits(YELLOW_LED);mPORTBClearBits(RED_LED);}
    if(distance == DISTANCE_STATUS_2){mPORTBClearBits(RED_LED);mPORTBClearBits(YELLOW_LED);}
         
    PT_END(pt);
}//end pt_LED

void main(void)
{
    
    //Set up LED pins
    mPORTBSetPinsDigitalOut(YELLOW_LED | RED_LED );
    
    /* config the uart, DMA, vref, timer5 ISR */
    PT_setup();
    
    //Enable channel
    //BRG computed value for the baud rate generator. The value is
    // calculated as follows: BRG = (Fpb / 2 / baudrate) - 2.
    OpenI2C1( I2C_ON, 44 );
    
    
    /* setup system wide interrupts */
    /* needed to use PT_YIELD_TIME_msec because it uses timer5 ISR
      also needed for any other ISR that may be used */ 
    INTEnableSystemMultiVectoredInt(); 
   
    //sensor setup
    sensor_initialize();
    
    //makes the sensor read continously based on period value parameter (in milliseconds)
    startContinousReadings(0);
  
    /* initialize threads */
    PT_INIT(&pt_serial);
    PT_INIT(&pt_sensor);
    PT_INIT(&pt_LED);
    
    
    //round-robin scheduler for threads
    while (1)
    {
        
        PT_SCHEDULE(protothread_serial(&pt_serial));
        PT_SCHEDULE(protothread_sensor(&pt_sensor));
        PT_SCHEDULE(protothread_LED(&pt_LED));
    }// end round-robin scheduler

}// end main