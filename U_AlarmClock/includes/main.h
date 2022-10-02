/*
 * main.h
 *
 * Created: 24.09.2022 09:33:35
 *  Author: MootSeeker
 */ 


#ifndef MAIN_H_
#define MAIN_H_

// Global Includes 
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "stack_macros.h"

// Private include files
#include "avr_compiler.h"
#include "pmic_driver.h"
#include "TC_driver.h"
#include "clksys_driver.h"
#include "sleepConfig.h"
#include "port_driver.h"

#include "mem_check.h"

#include "init.h"
#include "utils.h"
#include "errorHandler.h"
#include "NHD0420Driver.h"
#include "ButtonHandler.h"

// Event bits
#define BIT_0	( 1 << 0 )	// Count up
#define BIT_1	( 1 << 1 )	// Count down
#define BIT_2	( 1 << 2 )	// Set Alarm active
#define BIT_3	( 1 << 3 )
#define BIT_4	( 1 << 4 )
#define BIT_5	( 1 << 5 )
#define BIT_6	( 1 << 6 )
#define BIT_7	( 1 << 7 )	// Open Alarm Menu

// Max Tasks used in this application
#define TASK_STATE_MAX ( 6 )

// defined handle names for all tasks 
typedef enum
{
	WatchTaskHandle = 0U, 
	UiTaskHandle = 1U
}enTaskHandle;

//Time structure
typedef struct
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}st_time_t;

//Alarm Structure
typedef struct 
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
	
	uint8_t is_alarm_active; 
}st_alarm_t;

extern st_time_t gst_time; 
extern st_alarm_t gst_alarm; 

#endif /* MAIN_H_ */