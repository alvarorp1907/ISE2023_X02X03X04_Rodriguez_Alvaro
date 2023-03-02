#include "cmsis_os2.h"                          // CMSIS RTOS header file
 #include "rtc.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_ThAlarm;                        // thread id

extern osTimerId_t tim_toggle;
extern osTimerId_t tim_timeout;
 
void ThAlarm (void *argument);                   // thread function
 
int Init_ThAlarm (void) {
 
  tid_ThAlarm = osThreadNew(ThAlarm, NULL, NULL);
  if (tid_ThAlarm == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void ThAlarm (void *argument) {
 //INICIALIZACION DE LOS TIMERS PARA LA GESTION DE LA ALARMA DEL RTC
	Init_TimerTimeout();//creamos timer timeout
	Init_TimerToggleLed();//creamos timer periodic
	
  while (1) {
	//cuando se produzca una Alarma del RTC se activaran los timers para gestionar el parpadeo del LED0 durante 5 s
	osThreadFlagsWait(0x00000001U, osFlagsWaitAny, osWaitForever);
	osTimerStart(tim_toggle,500U);
	osTimerStart(tim_timeout,5000U);
		
    osThreadYield();                            // suspend thread
  }
}
