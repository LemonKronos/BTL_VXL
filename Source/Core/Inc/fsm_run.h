/*
 * fsm_run.h
 *
 *  Created on: 28 thg 10, 2024
 *      Author: TRONG DAT
 */

#ifndef INC_FSM_RUN_H_
#define INC_FSM_RUN_H_

#include "global.h"
#include "fsm_input_driven.h"
#include "traffic_light.h"

uint8_t fsm_run_StateChange;

void fsm_run(machine_state mode);

#endif /* INC_FSM_RUN_H_ */
