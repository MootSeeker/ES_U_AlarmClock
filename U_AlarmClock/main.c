/*
 * U_Taskmanegement.c
 *
 * Created: 20.03.2018 18:32:07
 * Author : chaos
 */ 

// Main include file
#include "main.h"

//global time structure

//Time structure
typedef struct
{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}st_time_t;

st_time_t gst_time; 

// Task handle structure
typedef struct
{
	TaskHandle_t handle;
	
}st_task_state_t;

st_task_state_t task_state[ TASK_STATE_MAX ];

// Button Task
void vButtonTask( void *pvParameters );
// Watch task
void vWatchTask( void *pvParameters );
// UI-Task
void vUiTask( void *pvParameters ); 

// Application Idel Hook 
extern void vApplicationIdleHook( void );
void vApplicationIdleHook( void )
{	
	
}

int main(void)
{
	BaseType_t task_status;
	
    resetReason_t reason = getResetReason();

	vInitClock();
	vInitDisplay();
				 
	task_status = xTaskCreate( vButtonTask,				// Funktions Name
							(const char *) "btTask",		// Task Name
							configMINIMAL_STACK_SIZE,	// Stack grösse
							NULL,						// Übergabe Wert
							3,							// Prio
							NULL);						// Handle
	configASSERT( task_status == pdPASS );				// Prüfen ob der Task korrekt erstellt wurde
	
	task_status = xTaskCreate( vWatchTask,							// Funktions Name
							(const char *) "wtTask",					// Task Name
							configMINIMAL_STACK_SIZE,				// Stack grösse
							NULL,									// Übergabe Wert
							1,										// Prio
							task_state[WatchTaskHandle].handle);		// Handle
	configASSERT( task_status == pdPASS );							// Prüfen ob der Task korrekt erstellt wurde
	
	task_status = xTaskCreate( vUiTask,							// Funktions Name
							(const char *) "wtTask",					// Task Name
							configMINIMAL_STACK_SIZE,				// Stack grösse
							NULL,									// Übergabe Wert
							1,										// Prio
							task_state[UiTaskHandle].handle);		// Handle
	configASSERT( task_status == pdPASS );							// Prüfen ob der Task korrekt erstellt wurde
	
	vDisplayClear();
	vTaskStartScheduler( );
	return 0;
}


void vButtonTask( void *pvParameters ) 
{
	( void ) pvParameters; //Not used in this task
	initButtons();

	for(;;) 
	{
		updateButtons();
		
		if(getButtonPress(BUTTON1) == SHORT_PRESSED) 
		{
 
		}
		if(getButtonPress(BUTTON2) == SHORT_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON3) == SHORT_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON4) == SHORT_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON1) == LONG_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON2) == LONG_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON3) == LONG_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON4) == LONG_PRESSED) 
		{

		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}

void vWatchTask( void *pvParameters )
{
	( void ) pvParameters;  // Not used in this Task
	
	gst_time.hour = 12; 
	gst_time.minute = 0; 
	gst_time.second = 0; 
	
	for( ;; )
	{
		if(gst_time.second >= 59)								//Wenn eine Minute vorbei
		{
			gst_time.second = 0;
			
			if(gst_time.minute >= 59)							// Wenn eine Stunde vorbei
			{
				gst_time.minute = 0;
				
				if(gst_time.hour >= 23) gst_time.hour = 0;		// Wenn ein Tag vorbei
				else gst_time.hour++; 
			} 
			else gst_time.minute++; 
		} 
		else gst_time.second++; 
		
		vTaskDelay( 200 / portTICK_RATE_MS ); 
	} 
}

void vUiTask( void *pvParameters )
{
	( void ) pvParameters; // Not used in this function
	
	// Display Buffer 
	char disp_buffer[ 50 ]; 
	 
	// Init Display 
	vDisplayClear();
	vDisplayWriteStringAtPos(0,0,"Alarm Clock V1.0");
		
	
	for( ;; )
	{
		// Print actual time
		sprintf( disp_buffer, "Time: %02d:%02d:%02d", gst_time.hour, gst_time.minute, gst_time.second); 
		vDisplayWriteStringAtPos( 1, 3, disp_buffer );
		
		// Print Alarm time
		
		
		// Print Alarm state 
		
		vTaskDelay( 200 / portTICK_RATE_MS ); 
	}
}