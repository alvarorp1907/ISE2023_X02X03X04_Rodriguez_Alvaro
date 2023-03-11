#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "sntp.h"
#include "rl_net.h"
#include <time.h>
#include "rtc.h"
#include "ThAlarm.h"

const  NET_ADDR4 ntp_server = { NET_ADDR_IP4 , 0, 217, 79, 179, 106 };
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);

extern osThreadId_t tid_ThAlarm;
 
void get_time (void) {
  if (netSNTPc_GetTime (NULL, time_callback) == netOK) {
    printf ("SNTP request sent.\n");
  }
  else {
    printf ("SNTP not ready or bad parameters.\n");
  }
}
 
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
  if (seconds == 0) {
    printf ("Server not responding or bad response.\n");
  }
  else {
    printf ("%d seconds elapsed since 1.1.1970\n", seconds);
		osThreadFlagsSet(tid_ThAlarm ,0x00000001U);
		fecha_hora_ntp(seconds);
		
  }
}

void fecha_hora_ntp(uint32_t seconds){
	time_t rawtime = seconds+3600;
    struct tm  ts;
    char       buf[80];

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    ts = *localtime(&rawtime);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
    printf("%s\n", buf);
	  uint8_t ano= ts.tm_year-100;
	  uint8_t mes= ts.tm_mon+1;
	  set_time_date(ts.tm_hour, ts.tm_min, ts.tm_sec,ts.tm_wday,ts.tm_mday,mes, ano );//SET RTC 
	  set_alarm();//SET RTC ALARM
}

