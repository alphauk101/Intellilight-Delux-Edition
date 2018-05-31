#ifndef __TIME_CTRL__
#define __TIME_CTRL__
#include "defines.h"
#include <Wire.h>
#include "RTClib.h"

class time_ctrl
{
    /*Realt Time Clock*/
    RTC_DS1307 rtc;
  public:
    /*If the clock is not running then false is return else true = ok*/
    bool time_init(void);
    void get_time_update(INTELLI_DATA *);
  private:
    LIGHT_PHASE get_phase(DateTime *);
};


#endif
