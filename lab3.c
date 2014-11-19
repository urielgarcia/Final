// ******************************************************************************************* //

// BY : Team 205: Jesus Chavolla, Uriel Garcia , Joshua Hurley

///////////////////////////////////////////////////////////////////////////////////////////////
// Include file for PIC24FJ64GA002 microcontroller. This include file defines
// MACROS for special function registers (SFR) and control bits within those
// registers.

#include "p24fj64ga002.h"
#include <stdio.h>
#include "lcd.h"
#include "ADC.h"
#include "PWM.h"


// ******************************************************************************************* //
// Configuration bits for CONFIG1 settings.
//
// Make sure "Configuration Bits set in code." option is checked in MPLAB.
//
// These settings are appropriate for debugging the PIC microcontroller. If you need to
// program the PIC for standalone operation, change the COE_ON option to COE_OFF.


_CONFIG1( JTAGEN_OFF & GCP_OFF & GWRP_OFF &
		 BKBUG_ON & COE_ON & ICS_PGx1 &
		 FWDTEN_OFF & WINDIS_OFF & FWPSA_PR128 & WDTPS_PS32768 )

// ******************************************************************************************* //
// Configuration bits for CONFIG2 settings.
// Make sure "Configuration Bits set in code." option is checked in MPLAB.

_CONFIG2( IESO_OFF & SOSCSEL_SOSC & WUTSEL_LEG & FNOSC_PRIPLL & FCKSM_CSDCMD & OSCIOFNC_OFF &
		 IOL1WAY_OFF & I2C1SEL_PRI & POSCMOD_XT )

// ******************************************************************************************* /
/*
VSS is connected to 9V battery and VS is connected to 5V on the board
pin 3 on H-bridge OUTPUT 1 is connected to RED >> LEFT motor
pin 6 on H-bridge OUTPUT 2 is connect ed to BLACK >> LEFT motor
pin 14 on H-bridge OUTPUT 3 is connected to RED >> RIGHT motor
pin 11 on H-bridge OUTPUT 4 is connected to BLACK >> RIGHT motor

SW1 is RB5 and is used to transition from IDLE->forward->IDLE->Backward->IDLE....etc.
 
JP5 ( potentiometer in board) is  connected to pin 2 and is configured as analog inputs
pin 7 is RP 3 connected to pin 2 on H-bridge (input 1)  controls counter-clockwise for left motor
pin 6 is RP 2 connected to pin 7 on H-bridge  (input 2) controls clockwise for left motor
pin 4 is RP 0 connected to pin 15 on H-bridge (input 3) controls counter-clockwise for right motor
pin 5 is RP 1 connected to pin 10 on H-bridge  (input 4)controls clockwise for right motor
********************************************************************/
// ******************************************************************************************* //
 #define Black 150
 #define Speed 420 // speed off motors at "full" speed
#define Pause 0 // speed for stop
volatile unsigned int state;//variable used to assign direction
volatile int Left;
volatile int Right;
volatile int Middle;

int main(void)
{
    TRISBbits.TRISB5=1;//enables the SW1 as input

//    CNEN2bits.CN27IE = 1;//change notification for SW1
//    IFS1bits.CNIF = 0;//enables the change notification interrupt
//    IEC1bits.CNIE = 1;//sets flag down


    int i=0;//used for printing delay
    int loop=0;
    state=0;//intializes in iddle state before going forward
     Left = 0;
     Right = 0;
     Middle=0;

   
    
    char ADV[4];//used to print the ADC value
    char ADV2[4];//used to print the duty cycles
    char ADV3[4];
   
    
//initializes LCD, the ADC and the PWM
     LCDInitialize();
    InADC();
   InPWM();

   TRISBbits.TRISB5 = 1;
   while(PORTBbits.RB5 == 1);
   
   OC1RS = Speed;
   OC2RS = Speed;

   RPOR4bits.RP8R= 19;//input 1 red left motor
   RPOR1bits.RP2R = 0;//input 2 black left motor

   RPOR0bits.RP0R = 18;//input 4 red right motor
   RPOR0bits.RP1R = 0;//input 3 black left motor
   

 
   
    while(1){

        //Clears the LCD screen, and displays the ADC value on the 1st line
//         LCDClear();
       //  value = AnalogtoDigital();
        Left = ADCLeft();
        Right = ADCRight();
        Middle = ADCCenter();
       
              LCDClear();
         LCDMoveCursor(1,0);

         sprintf(ADV,"%4.0d",Left);
         LCDPrintString(ADV);
         
         LCDMoveCursor(1,4);
         sprintf(ADV2,"%4.0d",Right);
         LCDPrintString(ADV2);

         LCDMoveCursor(0,0);
         sprintf(ADV3,"%4.0d",Middle);
         LCDPrintString(ADV3);

       //loop= Calibrate(Left,Center, Right,loop);
//        while (loop==1){
//        Left = ADCLeft();
//        Right = ADCRight();
//        Center = ADCCenter();
//        OC1RS = Speed; // // left motor
//        OC2RS = Speed; // //right motor
//        if(Left <= Black && Center<=Black && Right <= Black && loop==1){ // if on the black path
//            loop=0;
//            RPOR4bits.RP8R= 19;//input 1 red left motor
//            RPOR1bits.RP2R = 0;//input 2 black left motor
//            RPOR0bits.RP0R = 18;//input 4 red right motor
//            RPOR0bits.RP1R = 0;//input 3 black left motor
//        }
//    }
  

//         while(PORTBbits.RB5 == 0);
       

//         //gets the OC values to calculate the duty cycles
//        OC1RS = 1023- value;
//         OC2RS = value;
//
//        if((state==0 ) || (state==2)){
//        //for both of the Idle states,
//        //print a Duty cycle of 0 since the motors arent running
//          LCDMoveCursor(1,0);
//          LCDPrintString("0");
//         LCDMoveCursor(1,4);
//         LCDPrintString("0");
//
//        }
//
//        else if((state==1) ||(state==3) ){
//        //if in the forward or backward states
//
//            if(OC1RS>513 && OC2RS<511){
//            //for the right motor working at full speed
//
//            convert=(OC2RS*100)/511;//calculate duty cycle for left motor
//            sprintf(ADV2,"%3.0f",convert);
//            LCDMoveCursor(1,0);
//            LCDPrintString(ADV2);//print left motor duty cycle percentage
//             LCDMoveCursor(1,4);
//            LCDPrintString("100");//print right motor duty cycle percentage which is at 100%
//            }
//
//            else if(OC1RS<510 && OC2RS>513){
//            //for left motor working at full speed
//            LCDMoveCursor(1,0);
//            LCDPrintString("100");//print 100% for the left motor
//            convert=(OC1RS*100)/511;//calculate duty cycle for right motor
//            sprintf(ADV2,"%3.0f",convert);
//            LCDMoveCursor(1,4);
//            LCDPrintString(ADV2);//print duty cycle for left motor
//            }
//
//            else{// if both motors working at full speed,
//                //print 100% for both duty cycles
//            LCDMoveCursor(1,0);
//            LCDPrintString("100");
//            LCDMoveCursor(1,4);
//            LCDPrintString("100");
//            }
//       }
////
         for(i=0;i<32000; i++);//small delay for LCD to print
//
//
//
   }
}
//    void __attribute__((interrupt, auto_psv)) _CNInterrupt(void) {
//        //change interrupt for SW1
//
//
//        IFS1bits.CNIF = 0;//set flag down
//
//    }


