/*
 * timers.c
 *
 * Created: 12/30/2022 4:42:39 PM
 *  Author: User
 */ 
#include "timers.h"
void timer_CTC_init_interrupt(void)
{
	/*select CTC Mode*/
	SET_BIT(TCCR0,WGM01);
	/*load value on OCR0*/
	OCR0=80;
	/*select timer clock*/
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/*enable interrupt*/
	sei();
	SET_BIT(TIMSK,OCIE0);
}
void timer_wave_nonPWM(void)
 {
	/* set OC0 as output pin */
	 SET_BIT(DDRB,3);
	 /* select CTC mode*/
	 SET_BIT(TCCR0,WGM01);
	 /* load a value in OCR0 */
	 OCR0=64;
	 /* select timer clock */
	 SET_BIT(TCCR0,CS00);
	 SET_BIT(TCCR0,CS02);
	 /* toggle OC0 on compare match*/
	 SET_BIT(TCCR0,COM00);
 }
void timer_wave_fastPWM(void)
{
	/* set OC0 as output pin */
	SET_BIT(DDRB,3);
	/* select fast PWM mode*/
	SET_BIT(TCCR0,WGM00);
	SET_BIT(TCCR0,WGM01);
	/* load a value in OCR0 */
	OCR0=64;//duty cycle 25%
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* Set OC0 on compare match, clear OC0 at BOTTOM,(non inverting mode)*/
	//SET_BIT(TCCR0,COM00);
	SET_BIT(TCCR0,COM01);
}
void timer_wave_phasecorrectPWM(void)
{
	/* set OC0 as output pin */
	SET_BIT(DDRB,3);
	/* select phase correct PWM mode*/
	SET_BIT(TCCR0,WGM00);
	/* load a value in OCR0 */
	OCR0=64;//duty cycle 25%
	/* select timer clock */
	SET_BIT(TCCR0,CS00);
	SET_BIT(TCCR0,CS02);
	/* Clear OC0 on compare match when up-counting. Set OC0 on compare match when down counting.*/
	//SET_BIT(TCCR0,COM00);
	SET_BIT(TCCR0,COM01);
}
void timer2_overflow_init_interrupt(void)
{
	/*select timer clock*/
	SET_BIT(ASSR,AS2);
	/*select timer pre scaller*/
	SET_BIT(TCCR2,CS20);
	SET_BIT(TCCR2,CS22);
	/*enable interrupt*/
	sei();
	SET_BIT(TIMSK,TOIE2);
	
}
void timer1_wave_fastPWM(double value)
{
	/* set OC1A as output pin */
	SET_BIT(DDRD,5);
	/* select fast PWM mode*/
	SET_BIT(TCCR1A,WGM11);
	SET_BIT(TCCR1B,WGM12);
	SET_BIT(TCCR1B,WGM13);
	/* load a value in OCR1A,ICR1 */
	OCR1A=value*1000;
	ICR1=19999;
	/* select timer clock, no prescaling */
	SET_BIT(TCCR1B,CS10);
	/* clear OC1A on compare match, set OC1A at BOTTOM,(non inverting mode)*/
	SET_BIT(TCCR1A,COM1A1);
}
void timer0_stop(void)
{
	//disable the clock to stop the counter
	CLR_BIT(TCCR0, CS00);
	CLR_BIT(TCCR0, CS01);
	CLR_BIT(TCCR0, CS02);
}
void timer0_initializeCTC(void)
{
	//Config OCR0
	//OCR0 = the value to compare with
	OCR0 = 78; //to achieve tick equal to 10 msec
	
	//Set timer mode
	//Set the mode as Compare match
	SET_BIT(TCCR0, WGM01);
	CLR_BIT(TCCR0, WGM00);
	
	//Configure clock
	//set as clk/1024
	SET_BIT(TCCR0, CS00);
	CLR_BIT(TCCR0, CS01);
	SET_BIT(TCCR0, CS02);
	
	//Enable global interrupts
	sei();
	
	//Enable timer 0 interrupt for compare match
	SET_BIT(TIMSK, OCIE0);
}
