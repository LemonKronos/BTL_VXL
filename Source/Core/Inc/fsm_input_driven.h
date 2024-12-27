/*
 * input_processing.h
 *
 *  Created on: Oct 9, 2024
 *      Author: TRONG DAT
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "global.h"
#include "SCH_Task.h"
#include "traffic_light.h"

extern traffic_way main_traffic, side_traffic;
uint8_t fsm_input_driven_normal_StateChange;
uint8_t fsm_input_driven_config_StateChange;

typedef enum{
	NORMAL,
	FREEZE,
	FAST,
	FORWARD
}normal_mode;

normal_mode Nmode;

typedef enum{
	WAIT,
	UP,
	UP_FAST,
	DOWN,
	DOWN_FAST
}config_mode;

config_mode Cmode;

typedef enum{
	lRED,
	lYELLOW,
	lGREEN
} ledColor;

void fsm_input_driven_normal();
void fsm_input_driven_config(uint8_t* time1, uint8_t* time2);
void flushLed();

#endif /* INC_INPUT_PROCESSING_H_ */
