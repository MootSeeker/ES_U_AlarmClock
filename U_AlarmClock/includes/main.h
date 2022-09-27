/*
 * main.h
 *
 * Created: 24.09.2022 09:33:35
 *  Author: kevin
 */ 


#ifndef MAIN_H_
#define MAIN_H_


// Include files
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"
#include "ButtonHandler.h"

// Create Taskhandles for all tasks
#define TASK_STATE_MAX ( 6 )
typedef enum
{
	WatchTaskHandle = 0U
}enTaskHandle;





#endif /* MAIN_H_ */