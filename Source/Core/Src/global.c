/*
 * global.c
 *
 *  Created on: 28 thg 10, 2024
 *      Author: TRONG DAT
 */

#include "global.h"

// traffic light
traffic_way main_traffic = {
		.way = main_way,
		.red = 4,
		.yellow = 2,
		.green = 4
};
traffic_way side_traffic = {
		.way = side_way,
		.red = 6,
		.yellow = 1,
		.green = 3
};

// finite state machine
volatile machine_state m_mode = mode1;

// LCD
uint8_t led_index = 0;
void updateLCD(update_from where){
	if(where == traffic){
		mainCountDown = main_traffic.count_down;
		sideCountDown = side_traffic.count_down;
	}
	else if(where == counter){
		// nothing here
	}
};

// led traffic
void LedTraffic(whichWay way, LED_TRAFFIC led_traffic){
	if(way == main_way){
		switch (led_traffic){
			case LED_OFF:
				HAL_GPIO_WritePin(LedMain0_Port, LedMain0_Pin, RESET);
				HAL_GPIO_WritePin(LedMain1_Port, LedMain1_Pin, RESET);
				break;
			case LED_RED:
				HAL_GPIO_WritePin(LedMain0_Port, LedMain0_Pin, SET);
				HAL_GPIO_WritePin(LedMain1_Port, LedMain1_Pin, SET);
				break;
			case LED_YELLOW:
				HAL_GPIO_WritePin(LedMain0_Port, LedMain0_Pin, RESET);
				HAL_GPIO_WritePin(LedMain1_Port, LedMain1_Pin, SET);
				break;
			case LED_GREEN:
				HAL_GPIO_WritePin(LedMain0_Port, LedMain0_Pin, SET);
				HAL_GPIO_WritePin(LedMain1_Port, LedMain1_Pin, RESET);
				break;
			default:
				break;
		}
	}
	else if(way == side_way){
		switch (led_traffic){
			case LED_OFF:
				HAL_GPIO_WritePin(LedSide0_Port, LedSide0_Pin, RESET);
				HAL_GPIO_WritePin(LedSide1_Port, LedSide1_Pin, RESET);
				break;
			case LED_RED:
				HAL_GPIO_WritePin(LedSide0_Port, LedSide0_Pin, SET);
				HAL_GPIO_WritePin(LedSide1_Port, LedSide1_Pin, SET);
				break;
			case LED_YELLOW:
				HAL_GPIO_WritePin(LedSide0_Port, LedSide0_Pin, RESET);
				HAL_GPIO_WritePin(LedSide1_Port, LedSide1_Pin, SET);
				break;
			case LED_GREEN:
				HAL_GPIO_WritePin(LedSide0_Port, LedSide0_Pin, SET);
				HAL_GPIO_WritePin(LedSide1_Port, LedSide1_Pin, RESET);
				break;
			default:
				break;
		}
	}
}
