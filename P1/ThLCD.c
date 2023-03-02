#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "lcd.h"
#include "stdio.h"
#include "string.h"
#include "rtc.h"
#include "ThLCD.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
static osThreadId_t tid_ThLCD;                        // thread id

extern osMessageQueueId_t mid_MsgQueue1;       //id cola1

void ThLCD (void *argument);                   // thread function
 
int Init_ThLCD (void) {
 
  tid_ThLCD = osThreadNew(ThLCD, NULL, NULL);
  if (tid_ThLCD == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThLCD (void *argument) {

 //INICIALIZACION DEL LCD
	LCD_reset();
	LCD_Init();
	LCD_update();
	
  while (1) {
		
    fechayhora datosLCD;
		datosLCD=get_time_date();
		LCD_escribir_linea(datosLCD.time,datosLCD.date);
		LCD_update();
		osDelay(1000);//frec 1HZ para representar en el LCD info
		
		
    osThreadYield();                            // suspend thread
  }
}







	
