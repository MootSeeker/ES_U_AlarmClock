/*
 * main.c
 *
 * Created: 29.09.2022 18:15:35
 * Author : MootSeeker
 */ 

// Main include file
#include "main.h"

//global time structure
st_time_t gst_time; 
st_alarm_t gst_alarm; 

// Event Group
EventGroupHandle_t btnEventGrp; 

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
	
	// Create Event Group
	btnEventGrp = xEventGroupCreate( ); 
		
	//Create Tasks		 
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
							(const char *) "uiTask",					// Task Name
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
	
	st_alarm_t *pst_alarm = &gst_alarm; 

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
			xEventGroupSetBits( btnEventGrp, BIT_2 ); // Activate Alarm
		}
		if(getButtonPress(BUTTON3) == LONG_PRESSED) 
		{

		}
		if(getButtonPress(BUTTON4) == LONG_PRESSED) 
		{
			xEventGroupSetBits( btnEventGrp, BIT_7 ); //Bit 7 = Opens Alarm Value
		}
		vTaskDelay((1000/BUTTON_UPDATE_FREQUENCY_HZ)/portTICK_RATE_MS);
	}
}

void vWatchTask( void *pvParameters )
{
	( void ) pvParameters;  // Not used in this Task
	
	st_time_t *pst_time = &gst_time; 
	
	pst_time->hour = 12; 
	pst_time->minute = 0; 
	pst_time->second = 0; 
	
	for( ;; )
	{
		if( pst_time->second >= 59 )								//Wenn eine Minute vorbei
		{
			pst_time->second = 0;
			
			if( pst_time->minute >= 59 )							// Wenn eine Stunde vorbei
			{
				pst_time->minute = 0;
				
				if( pst_time->hour >= 23 ) pst_time->hour = 0;		// Wenn ein Tag vorbei
				else pst_time->hour++; 
			} 
			else pst_time->minute++; 
		} 
		else pst_time->second++; 
		
		vTaskDelay( 200 / portTICK_RATE_MS ); 
	} 
}

void vUiTask( void *pvParameters )
{
	( void ) pvParameters; // Not used in this function
	
	//Define Structure pointer
	st_time_t *pst_time = &gst_time;
	st_alarm_t *pst_alarm = &gst_alarm; 
	
	// Display Buffer 
	char disp_buffer[ 50 ]; 
	
	// Event bits
	EventBits_t bits; 
	
	//Init LED	
	PORTF.DIRSET = (1 << 1) | (1 << 0); /*LED2 & LED2*/
	PORTF.OUTSET = 0x03;
	 
	// Init Display 
	vDisplayClear();
	vDisplayWriteStringAtPos( 0, 0, "Alarm Clock V1.0" );
	
	for( ;; )
	{
		// Wait to receive Event bits / also generates new delay :)
		bits = xEventGroupWaitBits( btnEventGrp, BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 
									| BIT_5 | BIT_6 | BIT_7, pdFALSE, pdFALSE, 200 / portTICK_RATE_MS );
				
		if( bits == BIT_7 )	// Alarm Menu
		{
			
		}
			
		if( bits == BIT_2 )	// Toggle Alarm
		{
			if( !pst_alarm->is_alarm_active ) pst_alarm->is_alarm_active = 1;	// Turn on when not active
			else pst_alarm->is_alarm_active = 0;								// Turn off when active
			xEventGroupClearBits( btnEventGrp, BIT_2 );							// Clear bit after set alarm	
		}
		
		/* Display and LED output ................................................................................*/
		// Print actual time
		sprintf( disp_buffer, "Time: %02d:%02d:%02d", pst_time->hour, pst_time->minute, pst_time->second ); 
		vDisplayWriteStringAtPos( 1, 2, disp_buffer );
		
		memset(disp_buffer, 0 , sizeof(disp_buffer));	//Clear buffer
		vTaskDelay(10);									//Small delay to clear buffer
		
		// Print Alarm time
		sprintf( disp_buffer, "Alarm: %02d:%02d:%02d", pst_alarm->hour, pst_alarm->minute, pst_alarm->second ); 
		vDisplayWriteStringAtPos( 2, 2, disp_buffer ); 
		
		// Print Alarm state 
		if( pst_alarm->is_alarm_active )
		{
			memset(disp_buffer, 0 , sizeof(disp_buffer));	//Clear buffer
			vTaskDelay(10);									//Small delay to clear buffer
			
			// Print Alarm active
			sprintf( disp_buffer, "Alarm aktiv" );
			vDisplayWriteStringAtPos( 3, 2, disp_buffer );
			
			//blink LED 
			PORTF.OUTTGL = 0x03;
		}
		else
		{
			PORTF.OUTSET = 0x03;	// Turn LED off
		}
		
		//// TODO: Alarm not need since Waiting for Event bits will do the delay 
		//vTaskDelay( 200 / portTICK_RATE_MS ); 
	}
}