/*
 * global.h
 *
 *  Created on: 28 thg 10, 2024
 *      Author: TRONG DAT
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stm32f1xx_hal.h"

#define UNIT_TEST
#define SCH_On
//#define SCH_O1

// PIN DEFINE
#define Button0_Port B1_GPIO_Port
#define Button0_Pin B1_Pin
#define Button1_Port BUTTON_1_GPIO_Port
#define Button1_Pin BUTTON_1_Pin
#define Button2_Port BUTTON_2_GPIO_Port
#define Button2_Pin BUTTON_2_Pin

#define LedMain0_Port LED_MAIN0_GPIO_Port
#define LedMain0_Pin LED_MAIN0_Pin
#define LedMain1_Port LED_MAIN1_GPIO_Port
#define LedMain1_Pin LED_MAIN1_Pin
#define LedSide0_Port LED_SIDE0_GPIO_Port
#define LedSide0_Pin LED_SIDE0_Pin
#define LedSide1_Port LED_SIDE1_GPIO_Port
#define LedSide1_Pin LED_SIDE1_Pin

// software timer
//#define SEC 1000
//#define NUM_FLAG 5
//	/* FLAG NOTE
//	 *0 blinky led
//	 *1 main way
//	 *2 side way
//	 *3 1s to count down for 7seg led
//	 *4 refresh for 7seg led
//	 *5 countdown 500ms in hold state
//	 * */
//
//volatile uint8_t timer_flag[NUM_FLAG];

// traffic light
#define MAX_COUNT_DOWN 99
typedef enum{
	main_way,
	side_way
}whichWay;

typedef struct{
	whichWay  way;
	uint8_t count_down;
	uint8_t red, yellow, green;
}traffic_way;

typedef enum{
	LED_OFF,
	LED_RED,
	LED_YELLOW,
	LED_GREEN
}LED_TRAFFIC;

void LedTraffic(whichWay way, LED_TRAFFIC led_traffic);


// button
#define NUMBER_OF_BUTTONS 3

typedef enum{
	IDLE,
	PRESS,
	HOLD,
	DOUBLE_TAP,
	TAP_HOLD
}button_state;

button_state BUTTON[NUMBER_OF_BUTTONS];

unsigned char
	flagForButtonPress[NUMBER_OF_BUTTONS],
	flagForButtonHold[NUMBER_OF_BUTTONS],
	flagForButtonDoubleTap[NUMBER_OF_BUTTONS],
	flagForButtonTapHold[NUMBER_OF_BUTTONS];

// finite state machine
typedef enum machine_state{
	mode1,
	mode2,
	mode3,
	mode4,
	modeReset
}machine_state;
volatile machine_state m_mode;

// SCH
typedef struct{
	void* para1;
	void* para2;
	void* para3;
}Task_parameter;
Task_parameter TaskPara;

// LCD
char currentMode[7];
uint8_t flag_updateCountDown;

uint8_t mainCountDown, sideCountDown;

typedef enum{
	traffic,
	counter
}update_from;

void updateLCD(update_from where);

#endif /* INC_GLOBAL_H_ */
