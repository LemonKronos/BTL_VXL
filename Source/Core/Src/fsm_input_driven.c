/*
 * input_processing.c
 *
 *  Created on: Oct 9, 2024
 *      Author: TRONG DAT
 */
#include "fsm_input_driven.h"
#include "input_reading.h"
#include "Scheduler.h"

normal_mode Nmode = NORMAL;
config_mode Cmode = WAIT;

uint8_t fsm_input_driven_normal_StateChange = 1;
uint8_t fsm_input_driven_config_StateChange = 1;

void fsm_input_driven_normal(){
	switch(Nmode){
	case NORMAL:
		if(flagForButtonPress[1] == 1){
			fsm_input_driven_normal_StateChange = 1;
			Nmode = FREEZE;
			resetButtonFlag();
		}
		else if(flagForButtonPress[2] == 1){
			fsm_input_driven_normal_StateChange = 1;
			Nmode = FORWARD;
			resetButtonFlag();
		}
		else if(flagForButtonHold[2] == 1){
			fsm_input_driven_normal_StateChange = 1;
			Nmode = FAST;
			resetButtonFlag();
		}
		else{// RUN NORMAL
			if(fsm_input_driven_normal_StateChange){
				sprintf(currentMode, "NORMAL");
				SCH_AddTask(timeCountDown, 100, 100);
				fsm_input_driven_normal_StateChange = 0;
			}
			fsm_traffic(&t_mode_main, &main_traffic);
			fsm_traffic(&t_mode_side, &side_traffic);
		}
		break;
	case FREEZE:
		if(flagForButtonPress[1] == 1){// CHECK BUTTON
			fsm_input_driven_normal_StateChange = 1;
			Nmode = NORMAL;
			resetButtonFlag();
		}
		else if(flagForButtonPress[2] == 1){
			Nmode = FORWARD;
			resetButtonFlag();
		}
		else{// RUN FREEZE
			if(fsm_input_driven_normal_StateChange){
				sprintf(currentMode, "FREEZE");
				SCH_DeleteTask(timeCountDown);
				fsm_input_driven_normal_StateChange = 0;
			}
			fsm_traffic(&t_mode_main, &main_traffic);
			fsm_traffic(&t_mode_side, &side_traffic);
		}
		break;
	case FAST:
		if(BUTTON[2] == HOLD){
			if(fsm_input_driven_normal_StateChange){
				sprintf(currentMode, "FAST  ");
				SCH_DeleteTask(timeCountDown);
				SCH_AddTask(timeCountDown, 50, 50);
				fsm_input_driven_normal_StateChange = 0;
			}
			fsm_traffic(&t_mode_main, &main_traffic);
			fsm_traffic(&t_mode_side, &side_traffic);
		}
		else{
			SCH_DeleteTask(timeCountDown);
			fsm_input_driven_normal_StateChange = 1;
			Nmode = NORMAL;
			resetButtonFlag();
		}
		break;
	case FORWARD:
		SCH_DeleteTask(timeCountDown);
		if(main_traffic.count_down > side_traffic.count_down){
			main_traffic.count_down -= side_traffic.count_down;
			side_traffic.count_down = 0;
		}
		else{
			side_traffic.count_down -= main_traffic.count_down;
			main_traffic.count_down = 0;
		}
		updateLCD(traffic);
		fsm_input_driven_normal_StateChange = 1;
		Nmode = FREEZE;
		resetButtonFlag();
		break;
	default:
		Nmode = NORMAL;
		break;
	}
}

void fsm_input_driven_config(uint8_t* time1, uint8_t* time2){
	switch(Cmode){
	case WAIT:
		if(flagForButtonPress[1] == 1){
			Cmode = UP;
			resetButtonFlag();
		}
		else if(flagForButtonHold[1] == 1){
			fsm_input_driven_config_StateChange = 1;
			Cmode = UP_FAST;
			resetButtonFlag();
		}
		else if(flagForButtonDoubleTap[1] == 1){
			Cmode = DOWN;
			resetButtonFlag();
		}
		else if(flagForButtonTapHold[1] == 1){
			fsm_input_driven_config_StateChange = 1;
			Cmode = DOWN_FAST;
			resetButtonFlag();
		}
		else if(flagForButtonPress[2] == 1){
			if(mainCountDown != 0){
				if(mainCountDown >= *time1){
					*time2 = *time2 + (mainCountDown - *time1);
				}
				else{
					*time2 = *time2 - (*time1 - mainCountDown);
				}
				*time1 = mainCountDown;
				flag_updateCountDown = 1;
			}
		}
		else{// RUN WAIT
			if(fsm_input_driven_config_StateChange){
				// NOTHING HERE
				fsm_input_driven_config_StateChange = 0;
			}
		}
		break;
	case UP:
		if(mainCountDown < MAX_COUNT_DOWN) mainCountDown++;
		else mainCountDown = 0;
		updateLCD(counter);
		Cmode = WAIT;
		break;
	case UP_FAST:
		if(BUTTON[1] == HOLD){
			if(fsm_input_driven_config_StateChange){
				SCH_AddTask(timeCountUpFast, 25, 25);
				fsm_input_driven_config_StateChange = 0;
			}
		}
		else{
			SCH_DeleteTask(timeCountUpFast);
			resetButtonFlag();
			Cmode = WAIT;
		}
		break;
	case DOWN:
		if(mainCountDown > 0) mainCountDown--;
		else mainCountDown = MAX_COUNT_DOWN;
		updateLCD(counter);
		Cmode = WAIT;
		break;
	case DOWN_FAST:
		if(BUTTON[1] == TAP_HOLD){
			if(fsm_input_driven_config_StateChange){
				SCH_AddTask(timeCountDownFast, 25, 25);
				fsm_input_driven_config_StateChange = 0;
			}
		}
		else{
			SCH_DeleteTask(timeCountDownFast);
			resetButtonFlag();
			Cmode = WAIT;
		}
		break;
	default:
		Cmode = WAIT;
		break;
	}
}

void flushLed(){
	LedTraffic(main_way, LED_OFF);
	LedTraffic(side_way, LED_OFF);
}
