#include "motor.h"
#include <avr/io.h>
#include <stddef.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define PWM_PERIOD_US 20000
#define PWM_MAX_PULSE 5000
#define INTERRUPT_ERROR_US 16

typedef struct{
    volatile uint8_t* port;
    uint8_t pin;

    uint16_t pulse_width;

    uint16_t min_pulse;
    uint16_t max_pulse;
}Motor;

static Motor motor_array[3] = {
    //             min  max
    //port  pin targetpulse 
    {&PORTD, 0, 0, 500, 1600},
    {&PORTD, 1, 0, 500, 1650},
    {&PORTD, 2, 0, 500, 1650},
};
static uint8_t motor_count = sizeof(motor_array) / sizeof(Motor);
uint16_t elapsed_time_us = 0;

void timer0_init(void){
    TCCR0B = (1 << CS00);
    TIMSK0 = (1 << TOIE0);
    TCNT0 = 0;
}

void timer1_init(void){
    
}

void motor_set_pulse_width(uint8_t motor_index, uint16_t target_pulse_width_us){
    motor_array[motor_index].pulse_width = target_pulse_width_us;
    return;
}

void motor_set_power(uint8_t motor_index, double power){
    if(power<0||power>1){
        return;
    }
    uint16_t pulse_width = (double)(motor_array[motor_index].max_pulse - motor_array[motor_index].min_pulse) * power;
    pulse_width += motor_array[motor_index].min_pulse;
    motor_set_pulse_width(motor_index, pulse_width);
    return;
}

void pulse_write(volatile uint8_t* port_in, uint8_t pin, uint8_t high){
    if(high){
        *port_in |= (1<<pin);
        return;
    }
    *port_in &= ~(1<<pin);
    return;
}

//every 16us
ISR(TIMER0_OVF_vect) {
    elapsed_time_us+=16;
    if(elapsed_time_us <= PWM_MAX_PULSE){
        for(int i = 0;i<motor_count;i++){
            if(elapsed_time_us>=motor_array[i].pulse_width){
                //turn off when duty cycle is up
                pulse_write(motor_array[i].port, motor_array[i].pin, 0);
            }
        }
    }

    if(elapsed_time_us >= PWM_PERIOD_US){
        //end of period
        elapsed_time_us = 0;

        //start of period
        for(int j = 0;j<motor_count;j++){//set all pulses high
            pulse_write(motor_array[j].port, motor_array[j].pin, 1);
        }
    }
}