#include <avr/io.h>
#include <stddef.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "motor.h"


int main(){
    DDRB |= (1<<5);
    timer0_init();
    sei();

    while(1){
        loop();
    }
    return 0;
}

void loop(){
    //motor_set_power(0,1);
    
}