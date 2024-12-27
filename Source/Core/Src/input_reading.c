/*
 * input_processing.c
 *
 *  Created on: Oct 9, 2024
 *      Author: TRONG DAT
 */
#include "input_reading.h"
#include "LCD_I2C.h"

#define HOLD_TIME 100
#define RELEASE_TIME 20

#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

//for unit test
#ifdef UNIT_TEST
uint8_t test_button = 0;
#endif
//buffer after debouncing
static GPIO_PinState buttonBuffer[NUMBER_OF_BUTTONS] = { [0 ... (NUMBER_OF_BUTTONS - 1)] = BUTTON_IS_RELEASED};
//buffer for debouncing
static GPIO_PinState debounceButtonBuffer1[NUMBER_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NUMBER_OF_BUTTONS];
//counter
static uint16_t counterForButtonHold[NUMBER_OF_BUTTONS];
static uint16_t counterForButtonRelease[NUMBER_OF_BUTTONS];
//check initial press
static uint8_t initial_press[NUMBER_OF_BUTTONS];

GPIO_PinState button_pin_read(uint8_t index){
	switch(index){
	case 0:
		return HAL_GPIO_ReadPin(Button0_Port, Button0_Pin);
		break;
	case 1:
		return HAL_GPIO_ReadPin(Button1_Port, Button1_Pin);
		break;
	case 2:
		return HAL_GPIO_ReadPin(Button2_Port, Button2_Pin);
		break;
	default:
		break;
	}
	return SET;
}

void button_reading(){
    for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++){
    	//DEBOUNCE
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = button_pin_read(i);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i])
            buttonBuffer[i] = debounceButtonBuffer1[i];

        // UPDATE COUNTER
        if(buttonBuffer[i] == BUTTON_IS_RELEASED){
        	if(initial_press[i] == 5) initial_press[i] = 0;
        	if(initial_press[i] == 6) initial_press[i] = 0;
        	if(initial_press[i] == 1) initial_press[i] = 2;
        	if(initial_press[i] == 3) initial_press[i] = 4;

           	counterForButtonHold[i] = 0;
           	if(counterForButtonRelease[i] < RELEASE_TIME) counterForButtonRelease[i]++;
         }
        else if(buttonBuffer[i] == BUTTON_IS_PRESSED){
        	if(initial_press[i] == 0) initial_press[i] = 1;
        	if(initial_press[i] == 2) initial_press[i] = 3;

        	counterForButtonRelease[i] = 0;
        	if(counterForButtonHold[i] < HOLD_TIME) counterForButtonHold[i]++;
        }

        //REGCONIZE
        if(initial_press[i] == 0){	// IDLE
        	BUTTON[i] = IDLE;
        }
        if(initial_press[i] == 1 || initial_press[i] == 5){	// HOLD
        	if(counterForButtonHold[i] >= HOLD_TIME){
        		flagForButtonHold[i] = 1;
        		BUTTON[i] = HOLD;
        		initial_press[i] = 5;
        	}
        }
        if(initial_press[i] == 2){
        	if(counterForButtonRelease[i] >= RELEASE_TIME){	// PRESS
        		flagForButtonPress[i] = 1;
        		BUTTON[i] = PRESS;
        		initial_press[i] = 0;
        	}
        }
        if(initial_press[i] == 3 || initial_press[i] == 6){	// TAP HOLD
        	if(counterForButtonHold[i] >= HOLD_TIME){
        		flagForButtonTapHold[i] = 1;
        		BUTTON[i] = TAP_HOLD;
        		initial_press[i] = 6;
        	}
        }
        if(initial_press[i] == 4){
        	if(counterForButtonRelease[i] >= RELEASE_TIME){	// DOUBLE TAP
        		flagForButtonDoubleTap[i] = 1;
        		BUTTON[i] = DOUBLE_TAP;
        		initial_press[i] = 0;
        	}
        }

#ifdef UNIT_TEST
        if(i == 0) test_button = 0;
        test_button = test_button | !buttonBuffer[i];
#endif
    }
}

void resetButtonFlag(){
    memset(flagForButtonPress, 0, sizeof(flagForButtonPress));
    memset(flagForButtonHold, 0, sizeof(flagForButtonHold));
    memset(flagForButtonDoubleTap, 0, sizeof(flagForButtonDoubleTap));
    memset(flagForButtonTapHold, 0, sizeof(flagForButtonTapHold));
}

#ifdef UNIT_TEST
void unit_test_button_press(){
	if(test_button == 1) HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, SET);
	if(test_button == 0) HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, RESET);
}

void unit_test_button_read(){
	switch(BUTTON[1]){
	char button_state[20];
	case IDLE:
		break;
	case PRESS:
		sprintf(button_state, "PRESS           ");
		lcd_goto_XY(0, 0);
		lcd_send_string(button_state);
		break;
	case HOLD:
		sprintf(button_state, "HOLD            ");
		lcd_goto_XY(0, 0);
		lcd_send_string(button_state);
		break;
	case DOUBLE_TAP:
		sprintf(button_state, "DOUBLE TAP      ");
		lcd_goto_XY(0, 0);
		lcd_send_string(button_state);
		break;
	case TAP_HOLD:
		sprintf(button_state, "TAP HOLD        ");
		lcd_goto_XY(0, 0);
		lcd_send_string(button_state);
		break;
	}
}
#endif
