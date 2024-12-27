/*
 * SCH_Task.c
 *
 *  Created on: Nov 28, 2024
 *      Author: TRONG DAT
 */
#include "SCH_Task.h"
#include "fsm_input_driven.h"
#include "LCD_I2C.h"
#include "traffic_light.h"

extern traffic_way main_traffic, side_traffic;

void LedBlinky(){
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
void LedBlinky1(){
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
}
void timeCountDown(){
	if(main_traffic.count_down > 0) main_traffic.count_down--;
	if(side_traffic.count_down > 0) side_traffic.count_down--;
	updateLCD(traffic);
}
void timeCountUpFast(){
	if(mainCountDown < MAX_COUNT_DOWN) mainCountDown++;
	else mainCountDown = 0;
	updateLCD(counter);
}
void timeCountDownFast(){
	if(mainCountDown > 0) mainCountDown--;
	else mainCountDown = MAX_COUNT_DOWN;
	updateLCD(counter);
}
void displayLCD(){
	char buffer[17];
	char color[2] = " ";

	if(t_mode_main == RED) color[0] = 'R';
	else if(t_mode_main == YELLOW) color[0] = 'Y';
	else if(t_mode_main == GREEN) color[0] = 'G';
	if(mainCountDown >= 100) sprintf(buffer, "WTF main?");
	else if(mainCountDown >= 10) sprintf(buffer, "MAIN:%ds%s|MODE: ", mainCountDown, color);
	else sprintf(buffer, "MAIN: %ds%s|MODE: ", mainCountDown, color);
	lcd_goto_XY(0, 0);
	lcd_send_string(buffer);

	if(strcmp(currentMode, "CONFIG") == 0){// CONFIG MODE
		char token[4] = "   ";
		if(flag_updateCountDown == 1){
			flag_updateCountDown = 0;
			sprintf(token, "SET");
		}
		if(sideCountDown == 2){
			sprintf(buffer, "RED   %s|", token);
		}
		else if(sideCountDown == 3){
			sprintf(buffer, "YELLOW%s|", token);
		}
		else if(sideCountDown == 4){
			sprintf(buffer, "GREEN %s|", token);
		}
		else{
			sprintf(buffer, "WTFCOLOR |");
		}
		lcd_goto_XY(1, 0);
		lcd_send_string(buffer);
	}
	else{// RUN NORMALLY
		if(t_mode_side == RED) color[0] = 'R';
		else if(t_mode_side == YELLOW) color[0] = 'Y';
		else if(t_mode_side == GREEN) color[0] = 'G';
		if(sideCountDown >= 100) sprintf(buffer, "WTF side?");
		else if(sideCountDown >= 10) sprintf(buffer, "SIDE:%ds%s|", sideCountDown, color);
		else sprintf(buffer, "SIDE: %ds%s|", sideCountDown, color);
		lcd_goto_XY(1, 0);
		lcd_send_string(buffer);
	}

	if(currentMode[6] != '\0') sprintf(buffer, "BUGGED");
	else sprintf(buffer, currentMode);
	lcd_goto_XY(1, 10);
	lcd_send_string(buffer);
}
void toggleLedColor(){
	ledColor* color = (ledColor*)TaskPara.para1;
	switch(*color){
	case lRED:
		if(HAL_GPIO_ReadPin(LedMain0_Port, LedMain0_Pin) == GPIO_PIN_SET){
			LedTraffic(main_way, LED_OFF);
			LedTraffic(side_way, LED_OFF);
		}
		else{
			LedTraffic(main_way, LED_RED);
			LedTraffic(side_way, LED_RED);
		}
		break;
	case lYELLOW:
		if(HAL_GPIO_ReadPin(LedMain1_Port, LedMain1_Pin) == GPIO_PIN_SET){
			LedTraffic(main_way, LED_OFF);
			LedTraffic(side_way, LED_OFF);
		}
		else{
			LedTraffic(main_way, LED_YELLOW);
			LedTraffic(side_way, LED_YELLOW);
		}
		break;
	case lGREEN:
		if(HAL_GPIO_ReadPin(LedMain0_Port, LedMain0_Pin) == GPIO_PIN_SET){
			LedTraffic(main_way, LED_OFF);
			LedTraffic(side_way, LED_OFF);
		}
		else{
			LedTraffic(main_way, LED_GREEN);
			LedTraffic(side_way, LED_GREEN);
		}
		break;
	}
}
