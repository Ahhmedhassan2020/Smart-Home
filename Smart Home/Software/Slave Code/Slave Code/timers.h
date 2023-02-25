/*
 * timers.h
 *
 * Created: 12/30/2022 5:00:27 PM
 *  Author: User
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_
#include "std_macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
void timer_CTC_init_interrupt(void);
void timer_wave_nonPWM(void);
void timer_wave_fastPWM(void);
void timer0_stop(void);
#endif /* TIMERS_H_ */