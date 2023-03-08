#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file

#ifndef __RTC_H
//ESTRUCTURAS
typedef struct {
	char time[30];
	char date[30];
	
  }fechayhora;

//FUNCIONES
	void init_RTC(void);
	fechayhora get_time_date(void);
	void set_time_date(uint8_t hora, uint8_t minutos, uint8_t segundos, uint8_t dia, uint8_t diaSemana, uint8_t mes, uint8_t ano);
	void set_alarm(void);
	int Init_TimerToggleLed (void);
  static void Timer1_Callback (void const *arg);
  int Init_TimerTimeout (void);
  static void Timer2_Callback (void const *arg);
	
#endif
