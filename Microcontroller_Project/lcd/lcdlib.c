#define F_CPU 1000000ul

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcdlib.h"

void LCD_Init()
{
    _delay_ms(50);
    databus_direction |= 0xF0; // Configure both databus and controlbus as output
    control_direction |= (1 << RS);
    control_direction |= (1 << EN);
    LCD_CmdWrite(0x02); //Initilize the LCD in 4bit Mode
    LCD_CmdWrite(0x28);
    LCD_CmdWrite(0x0E); // Display ON cuRSor ON
    LCD_CmdWrite(0x01); // Clear the LCD
    LCD_CmdWrite(0x80); // Move the CuRSor to FiRSt line FiRSt Position
}

void LCD_CmdWrite(char cmd)
{
    databus = (cmd & 0xF0);    // SENd the Higher Nibble of the command to LCD
    control_bus &= ~(1 << RS); // Select the Command Register by pulling RS LOW
    control_bus |= 1 << EN;    // SENd a High-to-Low Pusle at Enable Pin
    _delay_us(1);
    control_bus &= ~(1 << EN);

    _delay_us(10); // wait for some time

    databus = ((cmd << 4) & 0xF0); // SENd the Lower Nibble of the command to LCD
    control_bus &= ~(1 << RS);     // Select the Command Register by pulling RS LOW
    control_bus |= 1 << EN;        // SENd a High-to-Low Pusle at Enable Pin
    _delay_us(1);
    control_bus &= ~(1 << EN);

    _delay_ms(1);
}

void LCD_DataWrite(char dat)
{
    databus = (dat & 0xF0);   // SENd the Higher Nibble of the Data to LCD
    control_bus |= (1 << RS); // Select the Data Register by pulling RS HIGH
    control_bus |= (1 << EN); // SENd a High-to-Low Pusle at Enable Pin
    _delay_us(1);
    control_bus &= ~(1 << EN);
    _delay_us(10);

    databus = ((dat << 4) & 0xF0); // SENd the Lower Nibble of the Data to LCD
    control_bus |= (1 << RS);      // Select the Data Register by pulling RS HIGH
    control_bus |= (1 << EN);      // SENd a High-to-Low Pusle at Enable Pin
    _delay_us(1);
    control_bus &= ~(1 << EN);
    _delay_ms(1);
}

void lcd_write_string(char *str1)
{
    int i = 0;
    while (str1[i] != '\0')
    {
        LCD_DataWrite(str1[i++]);
    }
    /*
	for(i = 0; i < strlen(str1); i++)
	{
		LCD_DataWrite(str1[i]);
	}
	*/
}

void lcd_write_int(int val)
{
    char str[5];
    sprintf(str, "%d", val);
    lcd_write_string(str);
}

void lcd_write_float(int val)
{
    char str[5];
    sprintf(str, "%d.%d", (val / 10), (val % 10));
    lcd_write_string(str);
}
