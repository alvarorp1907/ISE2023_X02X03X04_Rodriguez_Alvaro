#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file

#ifndef __SNTP_H
//ESTRUCTURAS
typedef struct {
	uint8_t segundos;
	uint8_t minutos;
	uint8_t hora;
	uint8_t dia;
	uint8_t mes;
	uint8_t ano;
	
  }ntp;
//FUNCIONES
void get_time (void);
void fecha_hora_ntp(uint32_t seconds);

#endif