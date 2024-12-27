/*
 * input_processing.h
 *
 *  Created on: Oct 9, 2024
 *      Author: TRONG DAT
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "global.h"

extern button_state BUTTON[];

GPIO_PinState button_pin_read(uint8_t index);
void button_reading();
void resetButtonFlag();

#ifdef UNIT_TEST
void unit_test_button_press();
void unit_test_button_read();
#endif

#endif /* INC_INPUT_READING_H_ */
