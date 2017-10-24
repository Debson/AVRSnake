/*
 * snake.c
 *
 *  Created on: Sep 3, 2017
 *      Author: DEBSON
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "snake.h"
#include "../LCD5110/nokia5110.h"

TSNAKE snake_tab[1];

// Food coordinates
uint8_t food[2];
uint8_t xx[] = { 2, 5, 8, 11, 14, 17, 20, 23, 26,
				29, 32, 35, 38, 41, 44, 47, 50, 53,
				56, 59, 62, 65, 68, 71, 74, 77, 80 }; 					//27
uint8_t yy[] = { 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42 }; // 14



TCFG eem_cfg EEMEM;

TCFG ram_cfg;

const TCFG pgm_cfg PROGMEM =
{
		-64,
		15,
		1
};

uint8_t restart;
uint8_t snake_cnt;

static void (*start_screen_callback)(uint8_t usr_cnt);
static void (*end_screen_callback)(uint8_t usr_cnt);

void register_start_screen_callback( void (*callback)(uint8_t usr_cnt))
{
	start_screen_callback = callback;
}

void registet_end_screen_callback( void (*callback)(uint8_t usr_cnt))
{
	end_screen_callback = callback;
}


void snake_init(uint8_t cnt)
{

	snake_cnt = cnt;
	snakes(3);

}

void init_snake(void)
{

	snakes(0)->speed = ram_cfg.speed;
	snakes(0)->byx[0] = (42<<8) + 2; // starting position Y = 4, X = 6;
	snakes(0)->body_len = 10;
	snakes(0)->direction = right;
	snakes(0)->draw_snake_fun = (void*)draw_snake;
//	snakes(0)->keys[left] = CUR_LEFT;
//	snakes(0)->keys[right] = CUR_RIGHT;
//	snakes(0)->keys[up] = CUR_UP;
//	snakes(0)->keys[down] = CUR_DOWN;
	snakes(0)->head = 0;
	snakes(0)->points = 0;

	food[0] = yy[random() % 14];
	food[1] = xx[random() % 27];
	nokia_lcd_write_food(food[1], food[0], 1);

	//if(start_screen_callback) start_screen_callback(snake_cnt);

}


TSNAKE * snakes(uint8_t n)
{

	if(n == 3) init_snake();

	return &snake_tab[n];
}


uint8_t check_body(TSNAKE * tsn, uint8_t y, uint8_t x)
{

	uint8_t i;
	uint8_t head = tsn->head;

	if(tsn->body_len == 1) return 0;

	for(i = 0; i<tsn->body_len; i++)
	{
		if( x == get_x(tsn->byx[head]) && y == get_y(tsn->byx[head])) return 1;
			head = (head - 1) & 511;
	}
	return 0;
}

uint8_t check_error(TSNAKE * tsn, uint8_t y, uint8_t x)
{

		uint8_t y1 = 0, x1 = 0;

		if(y == food[0] && x == food[1]) {
			while(1){

//				make_food();
				y1 = yy[random() % 14];
				x1 = xx[random() % 27];

				if(y1 == food[0] && x1 == food[1]) continue;

				if(!check_body(tsn, y1, x1)) break;
			}

			food[0] = y1;
			food[1] = x1;
			tsn->points++;
			return 2;
		}

		if(y > 44 || y < 3) return 1;
		if(x > 80 || x < 2) return 1;

		return check_body(tsn, y, x);
}


void draw_snake(TSNAKE * tsn)
{

	uint8_t head = tsn->head, tail = 0;
	uint8_t x = get_x(tsn->byx[head]);
	uint8_t y = get_y(tsn->byx[head]);

	uint8_t check_result;



	switch(tsn->direction){
		case left:
			{
				s_direction.leftDirection = 1;
				s_direction.upDirection = 0;
				s_direction.downDirection = 0;

				if(s_direction.rightDirection == 0)
					x-=3;
				else x+=3;
				break;
			}
		case right:
			{
				s_direction.rightDirection = 1;
				s_direction.upDirection = 0;
				s_direction.downDirection = 0;

				if(s_direction.leftDirection == 0)
					x+=3;
				else x-=3;
				break;
			}
		case up:
			{
				s_direction.rightDirection = 0;
				s_direction.leftDirection = 0;
				s_direction.upDirection = 1;

				if(s_direction.downDirection == 0)
					y-=3;
				else y+=3;
				break;
			}
		case down:
			{
				s_direction.rightDirection = 0;
				s_direction.leftDirection = 0;
				s_direction.downDirection = 1;

				if(s_direction.upDirection == 0)
					y+=3;
				else y-=3;
				break;
			}
	}

	check_result = check_error(tsn, y ,x);

	if(check_result == 1){

		restart = 0;

		tsn->points--;

		if(tsn->points > ram_cfg.max_score) {
			ram_cfg.max_score = tsn->points;
			copy_ram_to_eem();
		};

		while(1)
		{
			if( restart ) {
				restart=0;
				nokia_lcd_init();
				nokia_lcd_clear();
				snakes(3);
			}
		}
	}

	head = (head + 1) & 511;

	tsn->head = head;

	tsn->byx[head] = (y<<8) | x;

	nokia_lcd_write_square(get_x(tsn->byx[head]), get_y(tsn->byx[head]), 1);

	tail = (head - tsn->body_len) & 511;
	nokia_lcd_write_square(get_x(tsn->byx[tail]), get_y(tsn->byx[tail]), 0);

	if(check_result == 2) tsn->body_len++;

	nokia_lcd_write_food(food[1], food[0], 1);
	nokia_lcd_render();
}

void copy_eem_to_ram(void)
{
	eeprom_read_block( &ram_cfg, &eem_cfg, sizeof(ram_cfg));
}

void copy_ram_to_eem(void)
{
	eeprom_write_block(&ram_cfg, &eem_cfg, sizeof(ram_cfg));
}

void copy_pgm_to_ram(void)
{
	memcpy_P( &ram_cfg, &pgm_cfg, sizeof(ram_cfg));
}

void load_defaults(void)
{
	copy_pgm_to_ram();
	copy_ram_to_eem();
}

void check_and_load_defaults(void)
{

	uint8_t i, len = sizeof(ram_cfg);
	uint8_t * ram_ptr = (uint8_t*)&ram_cfg;

	copy_eem_to_ram();
	for(i = 0; i<len; i++){
		if(0xff == *ram_ptr++) continue;
		break;
	}

	if( i == len){
		load_defaults();
	}
}

















