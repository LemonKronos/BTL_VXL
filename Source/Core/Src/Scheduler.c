/*
 * Scheduler.c
 *
 *  Created on: Nov 28, 2024
 *      Author: TRONG DAT
 */
#include "Scheduler.h"
#include "input_reading.h"
#include "LCD_I2C.h"

#ifdef SCH_O1
STask* SCH_Task_List = NULL;

void SCH_Init(){
	while(SCH_Task_List != NULL){
		STask* temp = SCH_Task_List;
		SCH_Task_List = SCH_Task_List->next;
		free(temp);
	}
	RUNNING_TASK = 0;
	ID_HOLDER = 0;
	ERROR_CODE = 0;
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_IWDG_Init(&hiwdg);
}

void SCH_Update(){
	if(SCH_Task_List != NULL){
		if(SCH_Task_List->Delay > 0){
			SCH_Task_List->Delay--;
		}
		else{
			STask* task = SCH_Task_List;
			while(task != NULL && task->Delay == 0){
				task->RunMe = 1;
				task = task->next;
			}
		}
	}
}

void SCH_AddTask(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD){
	if(RUNNING_TASK >= SCH_MAX_TASK){
		ERROR_CODE = ERROR_SCH_REACH_MAX_TASKS;
		lcd_clear_display();
		lcd_goto_XY(0, 0);
		lcd_send_string(" Max Task");
		return;
	}
	STask* newTask = malloc(sizeof(STask));
	if(newTask == NULL){
		ERROR_CODE = ERROR_SCH_CANT_ALLOC;
		lcd_clear_display();
		lcd_goto_XY(0, 0);
		lcd_send_string(" Cant Alloc");
	}
	newTask->pTask = pFunction;
	newTask->RunMe = 0;
	newTask->Delay = DELAY;
	newTask->Period = PERIOD;
	newTask->TaskID = ++ID_HOLDER;
	newTask->next = NULL;

	__disable_irq();
	if(SCH_Task_List == NULL){
		SCH_Task_List = newTask;
	}
	else{
		STask* temp = SCH_Task_List;
		STask* pre = NULL;
		while(temp != NULL && temp->Delay < newTask->Delay){
			newTask->Delay -= temp->Delay;
			pre = temp;
			temp = temp->next;
		}
		newTask->next = temp;
		if(pre == NULL){
			SCH_Task_List = newTask;
		}
		else{
			pre->next = newTask;
		}
		if(temp != NULL){
			temp->Delay -= newTask->Delay;
		}
	}
	__enable_irq();
	RUNNING_TASK++;
}

void SCH_Dispatch() {
	if(SCH_Task_List != NULL){
		while(SCH_Task_List->RunMe >= 1){
			SCH_Task_List->pTask();
			SCH_Task_List->RunMe--;
			if(SCH_Task_List->Period){
				SCH_AddTask(SCH_Task_List->pTask, SCH_Task_List->Period, SCH_Task_List->Period);
			}
			RUNNING_TASK--;
			__disable_irq();
			STask* deleteTask = SCH_Task_List;
			SCH_Task_List = SCH_Task_List->next;
			__enable_irq();
			free(deleteTask);
		}
	}
#ifdef SCH_REPORT_STATUS
    SCH_ReportStatus();
#endif
    SCH_GoSleep();
}

R_CODE SCH_DeleteTask(const void (*pFunction)()){
	if(SCH_Task_List == NULL){
		ERROR_CODE = ERROR_SCH_TASK_EMPTY;
		lcd_clear_display();
		lcd_goto_XY(0, 0);
		lcd_send_string("Delete Empty");
		return FAILED;
	}
	else{
		__disable_irq();
		STask* temp = SCH_Task_List;
		if(SCH_Task_List->pTask == pFunction){
			SCH_Task_List = SCH_Task_List->next;
			free(temp);
		}
		while(temp != NULL){
			if(temp->next != NULL && temp->next->pTask == pFunction){
				STask* deleteTask = temp->next;
				temp->next = deleteTask->next;
				free(deleteTask);
			}
			temp = temp->next;
		}
		__enable_irq();
		return DONE;
	}
	lcd_clear_display();
	lcd_goto_XY(0, 0);
	lcd_send_string("No Task Deleted");
	return FAILED;
}

#ifdef SCH_REPORT_STATUS
void SCH_ReportStatus(void) {
    #ifdef SCH_REPORT_ERRORS
    if (ERROR_CODE != LAST_ERROR_CODE) {
    	ERROR_PORT = 255 - ERROR_CODE;
    	LAST_ERROR_CODE = ERROR_CODE;
        if (ERROR_CODE != 0) {
            ERROR_TICK_COUNT = 60000;
        } else {
        	ERROR_TICK_COUNT = 0;
        }
    } else {
        if (ERROR_TICK_COUNT != 0) {
            if (--ERROR_TICK_COUNT == 0) {
            	ERROR_CODE = 0;
            }
        }
    }
    #endif
}
#endif
#endif

#ifdef SCH_On

void SCH_Init(){
	memset(SCH_Task_List, 0, sizeof(SCH_Task_List));
	ERROR_CODE = 0;
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_IWDG_Init(&hiwdg);
}

void SCH_Update() {
    for (uint8_t Index = 0; Index < SCH_MAX_TASK; Index++) {
        if (SCH_Task_List[Index].pTask) {
            if (SCH_Task_List[Index].Delay == 0) {
                SCH_Task_List[Index].RunMe += 1;
                if (SCH_Task_List[Index].Period) {
                    SCH_Task_List[Index].Delay = SCH_Task_List[Index].Period;
                }
            } else {
                SCH_Task_List[Index].Delay--;
            }
        }
    }
}

void SCH_AddTask(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    unsigned char Index = 0;
    while ((SCH_Task_List[Index].pTask != 0) && (Index < SCH_MAX_TASK)) {
        Index++;
    }
    if (Index == SCH_MAX_TASK) {
        ERROR_CODE = ERROR_SCH_REACH_MAX_TASKS;
    }
    SCH_Task_List[Index].pTask = pFunction;
    SCH_Task_List[Index].Delay = DELAY;
    SCH_Task_List[Index].Period = PERIOD;
    SCH_Task_List[Index].RunMe = 0;
}

void SCH_Dispatch() {
    for (uint8_t Index = 0; Index < SCH_MAX_TASK; Index++) {
        if (SCH_Task_List[Index].RunMe > 0) {
            (*SCH_Task_List[Index].pTask)();
            SCH_Task_List[Index].RunMe -= 1;
            if (SCH_Task_List[Index].Period == 0) {
        		memset(&SCH_Task_List[Index], 0, sizeof(STask));
            }
        }
    }
#ifdef SCH_REPORT_STATUS
    SCH_ReportStatus();
#endif
    SCH_GoSleep();
}

R_CODE SCH_DeleteTask(const void (*pFunction)()){
    for(uint8_t i = 0; i < SCH_MAX_TASK; i++){
    	if(SCH_Task_List[i].pTask == pFunction){
    		memset(&SCH_Task_List[i], 0, sizeof(STask));
    		return DONE;
    	}
    }
    return FAILED;
}

#ifdef SCH_REPORT_STATUS
void SCH_ReportStatus(void) {
    #ifdef SCH_REPORT_ERRORS
    if (ERROR_CODE != LAST_ERROR_CODE) {
    	ERROR_PORT = 255 - ERROR_CODE;
    	LAST_ERROR_CODE = ERROR_CODE;
        if (ERROR_CODE != 0) {
            ERROR_TICK_COUNT = 60000;
        } else {
        	ERROR_TICK_COUNT = 0;
        }
    } else {
        if (ERROR_TICK_COUNT != 0) {
            if (--ERROR_TICK_COUNT == 0) {
            	ERROR_CODE = 0;
            }
        }
    }
    #endif
}
#endif
#endif

void SCH_GoSleep(){
	__WFI(); // System wait for interrupt
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim){
	if(htim->Instance == TIM2){
		button_reading();
		SCH_Update();
		HAL_IWDG_Refresh(&hiwdg);
	}
}
