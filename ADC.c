// ******************************************************************************************* //

#include "p24fj64ga002.h"
#include "ADC.h"
#include "LCD.h"
// ******************************************************************************************* //
#define Black_Left 230
#define Black_Middle 230 // black threshold, upper limit
#define Black_Right 230

 #define Speed 425 // speed off motors at "full" speed
#define Pause 0 // speed for stop
void InADC(){//intializes ADC

    AD1PCFG&= 0xFFFC;// select analog input AN0 to be connected to Jp 5
     
      AD1CON1 =0x20E4;  //  Auto converting after end of sampling
      AD1CON2 = 0x003C;//configures A/D voltage reference
      AD1CON3 = 0x0D01;
     

    AD1CSSL = 0x0000;        // no scanning
    AD1CHS = 0x0000;             //Configure input channels,

        
        AD1CON1bits.ADON = 1; // Turn on A/D
     

};

int AnalogtoDigital(){//function that returns the digital value
   
    AD1CHS = 0x0000;    // select analog input channel-AN0
    AD1CON1bits.SAMP = 1;// start sampling, automatic conversion will follow

   
    while(!AD1CON1bits.DONE);   // wait to complete the sampling
    AD1CON1bits.DONE = 0; // reset flag


    return(ADC1BUF0);//returns the ADC value
};

int ADCLeft(){

    int avg = 0;
        AD1CON1bits.ADON = 0; // Turn on A/D

    AD1CHS = 0x0000;    // select analog input channel-AN0
    
    AD1CON1bits.ADON = 1; // Turn on A/D

    AD1CON1bits.SAMP = 1;// start sampling, automatic conversion will follow

    while(!AD1CON1bits.DONE);   // wait to complete the sampling
    AD1CON1bits.DONE = 0; // reset flag

    AD1CON1bits.ADON = 0; // Turn on A/D

    avg = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 + ADC1BUF8 +
            ADC1BUF9 + ADC1BUFA + ADC1BUFB + ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF)/16;


    return(avg);//returns the ADC value
}

int ADCRight(){
    int avg = 0;

        AD1CON1bits.ADON = 0; // Turn on A/D

    AD1CHS = 0x0001;    // select analog input channel-AN0

    AD1CON1bits.ADON = 1; // Turn on A/D

    AD1CON1bits.SAMP = 1;// start sampling, automatic conversion will follow

    while(!AD1CON1bits.DONE);   // wait to complete the sampling
    AD1CON1bits.DONE = 0; // reset flag

    AD1CON1bits.ADON = 0; // Turn on A/D

    avg = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 + ADC1BUF8 +
            ADC1BUF9 + ADC1BUFA + ADC1BUFB + ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF)/16;

    return(avg);//returns the ADC value
}
int ADCCenter(){
    int avg = 0;

        AD1CON1bits.ADON = 0; // Turn on A/D

    AD1CHS = 0x0005;    // select analog input channel-AN5

    AD1CON1bits.ADON = 1; // Turn on A/D

    AD1CON1bits.SAMP = 1;// start sampling, automatic conversion will follow

    while(!AD1CON1bits.DONE);   // wait to complete the sampling
    AD1CON1bits.DONE = 0; // reset flag

    AD1CON1bits.ADON = 0; // Turn on A/D

    avg = (ADC1BUF0 + ADC1BUF1 + ADC1BUF2 + ADC1BUF3 + ADC1BUF4 + ADC1BUF5 + ADC1BUF6 + ADC1BUF7 + ADC1BUF8 +
            ADC1BUF9 + ADC1BUFA + ADC1BUFB + ADC1BUFC + ADC1BUFD + ADC1BUFE + ADC1BUFF)/16;

    return(avg);//returns the ADC value
}

int Calibrate(int Left, int Middle, int Right, int loop){

    if(Left <= Black_Left && Middle<=Black_Middle && Right <= Black_Right && loop==0){ // if on the black path
        OC1RS = Speed; // // left motor
        OC2RS = Speed; // //right motor

        if(OC1RS > 512   && OC2RS > 512){ // if it is going too fast
            OC1RS = 400;
            OC2RS = 400;
        }
    }

    else if(Left < Black_Left && Middle<=Black_Middle && Right > Black_Right && loop==0){ // if right sensor is off path
       OC1RS = Pause;
        OC2RS = Speed;
    }

    else if(Left > Black_Left && Middle<=Black_Middle && Right < Black_Right && loop==0){ // if left sensor is off path
       OC1RS = Speed;
       OC2RS = Pause;
    }

//    else if (Left>Black && Center> Black && Right >Black && loop==0){
//        loop=1;
//        OC1RS = Pause;
//       OC2RS = Pause;
//        RPOR4bits.RP8R= 19;//input 1 red left motor
//        RPOR1bits.RP2R = 0;//input 2 black left motor
//        RPOR0bits.RP0R = 0;//input 4 red right motor
//        RPOR0bits.RP1R = 18;//input 3 black left motor
//    }


    return(loop);

    

}