#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file

  RTC_HandleTypeDef hrtc;
  RTC_TimeTypeDef sTime = {0};
	RTC_DateTypeDef sDate = {0};
	RTC_AlarmTypeDef alarmRtc;
	
	extern osThreadId_t tid_ThAlarm;
	
void init_RTC(void){
	
  /* USER CODE END RTC_Init */
  /** Initialize RTC Only 
  */

	__HAL_RCC_PWR_CLK_ENABLE();
	HAL_PWR_EnableBkUpAccess();//HABILITAMOS REG DE RESG
  __HAL_RCC_RTC_ENABLE();
	
	
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 0x7F;
  hrtc.Init.SynchPrediv = 0xFF;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

	
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
   printf("error al iniciar el RTC\n\r");
  }
 
  /** Initialize RTC and set the Time and Date 
  */
	uint8_t hora=0x23;
	uint8_t minutos=0x59;
	uint8_t segundos=0x30;
	uint8_t dia=RTC_WEEKDAY_WEDNESDAY;
	uint8_t mes=RTC_MONTH_MARCH;
	uint8_t diaSemana = 0x01;//numero dia del mes
	uint8_t ano =0x23;
  set_time_date(hora,minutos,segundos,dia,diaSemana,mes,ano);
	
	 /**set alarm 
  */
	//set_alarm();
	
}

fechayhora get_time_date(void){//obtencion de la hora y la fecha del RTC
	
  fechayhora tiempo;
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	sprintf(tiempo.time,"Time: %.2d:%.2d:%.2d ",sTime.Hours,sTime.Minutes,sTime.Seconds);
	//printf("%s",tiempo.time);
	
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(tiempo.date,"Date: %02d / %02d / %02d",sDate.Date,sDate.Month,sDate.Year);
	//printf("%s",tiempo.date);
	
	return tiempo;
}

void set_time_date(uint8_t hora, uint8_t minutos, uint8_t segundos, uint8_t dia, uint8_t diaSemana, uint8_t mes, uint8_t ano ){
	//HORA
	sTime.Hours = hora;
  sTime.Minutes = minutos;
  sTime.Seconds = segundos;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    printf("error al establecer la hora del RTC\n\r");
	}
	//FECHA
  sDate.WeekDay = dia;//lunes-domingo
  sDate.Month = mes;
  sDate.Date = diaSemana;//numero dia del mes
  sDate.Year = ano;
 
  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
   printf("error al establecer la fecha del RTC\n\r");
  }
}

void set_alarm(void){
  HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	uint8_t mins_alarm=sTime.Minutes+1;
	uint8_t horas_alarm=sTime.Hours;
	
	if(mins_alarm==60){
		mins_alarm=0;
		if(horas_alarm==23){
			horas_alarm=0;
		}else{
			horas_alarm++;
		}
	}
	
	
	alarmRtc.AlarmTime.Hours = horas_alarm;
  alarmRtc.AlarmTime.Minutes = mins_alarm;
  alarmRtc.AlarmTime.Seconds = sTime.Seconds;
  alarmRtc.AlarmTime.SubSeconds = 0x0;
  alarmRtc.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  alarmRtc.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  alarmRtc.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  alarmRtc.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  alarmRtc.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  alarmRtc.AlarmDateWeekDay = 0x1;
  alarmRtc.Alarm = RTC_ALARM_A;
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &alarmRtc, RTC_FORMAT_BIN) != HAL_OK) {
  printf("error al establecer la alarma\n\r");
  }
}




//TIMER PERIODIC 
//cada 500 ms cambiara el estado del LED0 para que generar el parpadeo couando se produzca una alarma del RTC

osTimerId_t tim_toggle;                            // timer id
static uint32_t exec2;                          // argument for the timer call back function

// Periodic Timer Function
static void Timer1_Callback (void const *arg) {
  // add user code here
HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
}
 
// Example: Create and Start timers
int Init_TimerToggleLed (void) {
  osStatus_t status;                            // function return status
  // Create periodic timer
  exec2 = 2U;
  tim_toggle = osTimerNew((osTimerFunc_t)&Timer1_Callback, osTimerPeriodic, &exec2, NULL);
	/*
  if (tim_toggle != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart(tim_toggle, 500U);            
    if (status != osOK) {
      return -1;
    }
  }
	*/
  return NULL;
}

//TIMER TIMEOUT 5 s

osTimerId_t tim_timeout;                            // timer id
static uint32_t exec1;                          // argument for the timer call back function

// Periodic Timer Function
static void Timer2_Callback (void const *arg) {
  // add user code here
osTimerStop(tim_toggle);
}
 
// Example: Create and Start timers
int Init_TimerTimeout (void) {
  osStatus_t status;                            // function return status
  // Create periodic timer
  exec1 = 2U;
  tim_timeout = osTimerNew((osTimerFunc_t)&Timer2_Callback, osTimerOnce, &exec1, NULL);
	/*
  if (tim_toggle != NULL) {  // Periodic timer created
    // start timer with periodic 1000ms interval
    status = osTimerStart(tim_timeout, 5000U);            
    if (status != osOK) {
      return -1;
    }
  }
	*/
  return NULL;
}

void RTC_Alarm_IRQHandler(void) {
  HAL_RTC_AlarmIRQHandler(&hrtc);
	//osThreadFlagsSet(tid_ThAlarm ,0x00000001U);
	//set_alarm();//establecemos alarma dentro de un minuto
}
