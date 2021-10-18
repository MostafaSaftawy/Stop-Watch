/*
 * Timer.h
 *
 *  Created on: Oct 18, 2021
 *      Author: Mostafa Saftawy
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>




void Timer1_CTC_Init(void);
void INT0_Init(void);
void INT1_Init(void);
void INT2_Init(void);
void Program_Init(void);
void Set_Value(void);
void Update_State(void);
ISR(TIMER1_COMPA_vect);
ISR(INT0_vect);
ISR(INT1_vect);
ISR(INT2_vect);

#endif /* TIMER_H_ */
