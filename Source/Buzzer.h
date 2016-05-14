#ifndef Buzzer_H_
#define Buzzer_H_

#include "common_macros.h"

#define OUTPUTPIN      PB0		  //Pin 53
#define OUTPUTPORT     PORTB
#define OUTPUTPORT_DIR DDRB


void Buzzer_init()
{
    SET_BIT(OUTPUTPORT_DIR,OUTPUTPIN);
}


void Alert(long duration,int freq) 
{
    duration = duration *1000;
    int period = (1.0 / freq) * 1000000;
    long elapsed_time = 0;
    
    while (elapsed_time < duration) 
    {
        SET_BIT(OUTPUTPORT,OUTPUTPIN);
        delayMicroseconds(period / 2);
        
        CLEAR_BIT(OUTPUTPORT,OUTPUTPIN);
        delayMicroseconds(period / 2);
        
        elapsed_time = elapsed_time + period;
    }
}


#endif /* Buzzer_H_ */
