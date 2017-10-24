/*
 * main.c
 *
 *  Created on: Sep 3, 2017
 *      Author: DEBSON
 */


/* TODO organize brace brackets
* random() should generate diffrent coordinates every restart
* add score
* add start/end screen
*
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <util/delay.h>

#include "Snake/snake.h"
#include "LCD5110/nokia5110.h"

void set_borders(void);
void switch_keys(void);

volatile uint16_t Timer1;

uint8_t game_on;

uint8_t key1_lock, key2_lock, key3_lock, key4_lock;


int main(void)
{
		PORTC |= CUR_RIGHT | CUR_LEFT | CUR_UP | CUR_DOWN;


	/* Timer2 – program timers engine configuration */
		OCR2A 	= 78;			// przerwanie porównania co 10ms (100Hz)
		TCCR2A 	|= (1<<WGM21);			// set to CTC Mode
		TCCR2B 	|= (1<<CS22)|(1<<CS21)|(1<<CS20);	// set prescaler to 1024 and starts PWM
		TIMSK2 	= (1<<OCIE2A);	// set interrupt on compare match

		check_and_load_defaults();


		nokia_lcd_init();
		nokia_lcd_clear();
//		set_borders();
//		nokia_lcd_write_string("test", 1);
		nokia_lcd_draw();
		nokia_lcd_render();
		_delay_ms(3000);
		nokia_lcd_clear();
		_delay_ms(500);
		snake_init(ram_cfg.users_cnt);

		nokia_lcd_render();
		sei(); // enable interrupts
		game_on = 1;

	while(1)
	{

		if(!Timer1){

			switch_keys();
			set_borders();
			if(snakes(0)->draw_snake_fun) snakes(0)->draw_snake_fun(snakes(0));

			Timer1 = ram_cfg.speed;
		}

	}
}

void set_borders(void)
{
	// y borders
	for(int i = 1; i < 44; i++)
			{
				nokia_lcd_set_pixel(0, i, 1);
				nokia_lcd_set_pixel(82, i, 1);
			}

	// x borders
	for(int i = 0; i < 83; i++)
			{
				nokia_lcd_set_pixel(i, 1, 1);
				nokia_lcd_set_pixel(i, 44, 1);
			}

}

void switch_keys(void){

		if(!key1_lock && !(PINC & CUR_RIGHT))
		{
			key1_lock = 1;
			snakes(0)->direction = right;
		} else if(key1_lock && (PINC & CUR_RIGHT)) key1_lock = 0;;

		//** LEFT DIRECTION
		if(!key2_lock && !(PINC & CUR_LEFT))
		{
			key2_lock = 1;
			snakes(0)->direction = left;
		} else if(key2_lock && (PINC & CUR_LEFT)) key2_lock = 0;

		//** UP DIRECTION
		if(!key3_lock && !(PINC & CUR_UP))
		{
			key3_lock = 1;
			snakes(0)->direction = up;
		} else if(key3_lock && (PINC & CUR_UP)) key3_lock = 0;

		//** DOWN DIRECTION
		if(!key4_lock && !(PINC & CUR_DOWN))
		{
			key4_lock = 1;
			snakes(0)->direction = down;
		} else if(key4_lock && (PINC & CUR_DOWN)) key4_lock = 0;
}


ISR (TIMER2_COMPA_vect)
{
	uint16_t n;

	n = Timer1;
	if(n) Timer1 = --n;
}
