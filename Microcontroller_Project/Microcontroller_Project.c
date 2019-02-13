/*
 * Microcontroller_Project.c
 *
 * Created: 14/06/2017 05:08:22 Øµ
 *  Author: Ahmed Adel
 */ 

#define F_CPU 1000000ul
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "lcd/lcdlib.h"

int main(void)
{
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01);
	
	ADCSRA |= (1<<ADPS2);
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADIE);
	ADCSRA |= (1<<ADEN);

	DDRB |= (1 << DDB1)|(1 << DDB2);
	ICR1 = 1023;
	TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12)|(1 << WGM13);
	TCCR1B |= (1 << CS10);

	ADCSRA |= (1<<ADSC);	
	sei();
    while(1)
    {
    }
}

ISR(ADC_vect){
	sei();
	LCD_Init();
	int adcValue = ADCW;
	OCR1A = ADCW;
	LCD_CmdWrite(_LCD_CLR);
	lcd_write_string(" speed=");
	lcd_write_int(adcValue);
	lcd_write_string(" R.P.M");
	ADCSRA |= (1<<ADSC);
}

ISR(INT0_vect){
	ADCSRA &= ~(1<<ADEN);
	ADMUX |= (1<<MUX0);
	ADCSRA |= (1<<ADEN);
	ADCSRA |= (1<<ADSC);
	
	while (ADCSRA & (1<<ADSC));
	LCD_Init();
	int adcValue = ADCW;
	adcValue *= 0.48828125;
	LCD_CmdWrite(_LCD_CLR);
	lcd_write_string(" temp = ");
	lcd_write_int(adcValue);
	_delay_ms(1000);
	
	ADMUX &= ~(1<<MUX0);
	ADCSRA |= (1<<ADSC);
}
