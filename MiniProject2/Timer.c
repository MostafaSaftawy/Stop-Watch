/*
 * Timer.c
 *
 *  Created on: Oct 18, 2021
 *      Author: Mostafa Saftawy
 */

#include "Timer.h"

unsigned char  sec=0,min=0, hour=0;	// to use as a counters
unsigned char  last_count=0; // to save the digit before go to isr

void Timer1_CTC_Init(void)
{
	TCNT1 = 0;

	OCR1A = 15625;

	TIMSK = (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

	TCCR1A = (1<<FOC1A);

	/* Configure timer control register TCCR1B
	 * 1. CTC Mode WGM12=1 WGM13=0 (Mode Number 4)
	 * 2. Prescaler = F_CPU/64 CS10=1 CS11=1 CS12=0
	 */
	TCCR1B =  ( (1<<WGM12) | (1<<CS11) | (1<<CS10) );

	SREG  |= (1<<7);

}

void INT0_Init(void){
	DDRD  &= ~((1<<PD2));
	GICR |= (1<<6);
	GIFR |= (1<<6);
	MCUCR |= (1<<ISC01);
	SREG |= (1<<7);
}

void INT1_Init(void){
	DDRD  &= ~((1<<PD3));
	GICR |= (1<<7);
	GIFR |= (1<<7);
	MCUCR |= ((1<<ISC11)|(1<<ISC10));
	SREG |= (1<<7);
}

void INT2_Init(void){
	DDRD  &= ~((1<<PD3));
	GICR |= (1<<5);
	GIFR |= (1<<5);
	MCUCSR |= (1<<ISC2);
	SREG |= (1<<7);
}

void Program_Init(void){

	INT0_Init();
	INT1_Init();
	INT2_Init();

	DDRA = 0x3F;
	PORTA = 0x3F;
	DDRC = 0x0F;
	PORTC &= 0xF0;

	SREG  |= (1<<7);

	Timer1_CTC_Init();
}

void Set_Value(void){

	for(unsigned char i =0;i<6;i++){
		PORTA =0x00;
		if(i==0){
			PORTC = (PORTC & 0xF0) | (sec%10 & 0x0F);
		}
		else if(i==1){
			PORTC = (PORTC & 0xF0) | (sec/10 & 0x0F);
		}
		else if(i==2){
			PORTC = (PORTC & 0xF0) | (min%10 & 0x0F);
		}
		else if(i==3){
			PORTC = (PORTC & 0xF0) | (min/10 & 0x0F);
		}
		else if(i==4){
			PORTC = (PORTC & 0xF0) | (hour%10 & 0x0F);
		}
		else if(i==5){
			PORTC = (PORTC & 0xF0) | (hour/10 & 0x0F);
		}
		else{
			PORTC = 0;
		}
		PORTA = 0xFF & (1<<i);
		_delay_ms(1);
	}
}


void Update_State(void){
	if(sec==60){
		sec=0;
		min++;
	}
	if(min == 60){
		min=0;
		hour ++;
	}
	if(hour==24){
		hour = 0;
	}

}



ISR(TIMER1_COMPA_vect)
{
	sec++;
	Update_State();
}



ISR(INT0_vect){
	sec=0,min=0,hour=0;
	TCNT1=0;
	Update_State();
}


ISR(INT1_vect){
	last_count=TCNT1;
	TIMSK &= ~(1 << OCIE1A)	;
}



ISR(INT2_vect){
	TCNT1 = last_count;
	TIMSK |= (1 << OCIE1A)	;
}



