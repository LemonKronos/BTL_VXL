/*
 * traffic_light.c
 *
 *  Created on: Oct 28, 2024
 *      Author: TRONG DAT
 */
#include "traffic_light.h"

traffic_mode	t_mode_main = RED,
				t_mode_side = YELLOW;

void fsm_traffic(traffic_mode* mode, traffic_way* Tway){
	switch(*mode){
	case RED:
		if(Tway->count_down == 0){
			*mode = GREEN;
			LedTraffic(Tway->way, LED_GREEN);
			Tway->count_down = Tway->green;
			updateLCD(traffic);
		}
		break;
	case YELLOW:
		if(Tway->count_down == 0){
			*mode = RED;
			LedTraffic(Tway->way, LED_RED);
			Tway->count_down = Tway->red;
			updateLCD(traffic);
		}
		break;
	case GREEN:
		if(Tway->count_down == 0){
			*mode = YELLOW;
			LedTraffic(Tway->way, LED_YELLOW);
			Tway->count_down = Tway->yellow;
			updateLCD(traffic);
		}
		break;
	default:
		*mode = RED;
		LedTraffic(Tway->way, LED_RED);
		Tway->count_down = Tway->red;
		break;
	}
}
