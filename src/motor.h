#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>

void timer0_init(void);
void motor_set_power(uint8_t motor_index, double target_position);
void motor_write(volatile uint8_t* port_in, uint8_t pin, uint8_t high);
void motor_set_pulse_width(uint8_t motor_index, uint16_t target_pulse_width_us);

#endif