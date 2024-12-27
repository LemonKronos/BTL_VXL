/*
 * fsm_run.c
 *
 *  Created on: 28 thg 10, 2024
 *      Author: TRONG DAT
 */
#include "fsm_run.h"
#include "input_reading.h"
#include "Scheduler.h"
#include "LCD_I2C.h"

uint8_t fsm_run_StateChange = 1;
ledColor color = 0;

void fsm_run(machine_state mode){
	if(flagForButtonHold[0] == 1){
		sprintf(currentMode, "RESET ");
		m_mode = modeReset;
	}
	switch(mode){
	case mode1:// run normally
		if(flagForButtonPress[0] == 1){
			fsm_run_StateChange = 1;
			SCH_DeleteTask(timeCountDown);
			m_mode = mode2;
			resetButtonFlag();
		}
		else{
			if(fsm_run_StateChange){
				t_mode_main = RED;
				t_mode_side = YELLOW;
				main_traffic.count_down = 0;
				side_traffic.count_down = 0;
				fsm_input_driven_normal_StateChange = 1;
				fsm_run_StateChange = 0;
			}
			fsm_input_driven_normal();
		}
		break;

	case mode2:
		if(flagForButtonPress[0] == 1){
			fsm_run_StateChange = 1;
			SCH_DeleteTask(toggleLedColor);
			m_mode = mode3;
			resetButtonFlag();
		}
		else{
			if(fsm_run_StateChange){
				sprintf(currentMode, "CONFIG");
				flushLed();
				color = lRED;
				TaskPara.para1 = (void*)&color;
				SCH_AddTask(toggleLedColor, 50, 50);

				mainCountDown = main_traffic.red;
				sideCountDown = 2;
				updateLCD(counter);
				fsm_run_StateChange = 0;
			}
			fsm_input_driven_config(&main_traffic.red, &side_traffic.green);
		}
		break;

	case mode3:
		if(flagForButtonPress[0] == 1){
			fsm_run_StateChange = 1;
			SCH_DeleteTask(toggleLedColor);
			m_mode = mode4;
			resetButtonFlag();
		}
		else{
			if(fsm_run_StateChange){
				flushLed();
				color = lYELLOW;
				TaskPara.para1 = (void*)&color;
				SCH_AddTask(toggleLedColor, 50, 50);

				mainCountDown = main_traffic.yellow;
				sideCountDown = 3;
				updateLCD(counter);
				fsm_run_StateChange = 0;
			}
			fsm_input_driven_config(&main_traffic.yellow, &side_traffic.red);
		}
		break;

	case mode4:
		if(flagForButtonPress[0] == 1){
			fsm_run_StateChange = 1;
			SCH_DeleteTask(toggleLedColor);
			m_mode = mode1;
			resetButtonFlag();
		}
		else{
			if(fsm_run_StateChange){
				flushLed();
				color = lGREEN;
				TaskPara.para1 = (void*)&color;
				SCH_AddTask(toggleLedColor, 50, 50);

				mainCountDown = main_traffic.green;
				sideCountDown = 4;
				updateLCD(counter);
				fsm_run_StateChange = 0;
			}
			fsm_input_driven_config(&main_traffic.green, &side_traffic.red);
		}
		break;

	default:// modeReset
		if(BUTTON[0] != HOLD){
			NVIC_SystemReset();
		}
		break;
	}
}
