/*****************
   Intelli light delux
   The fish tank light returns this time hes angry!
 *****************/
#include <Wire.h>
#include "RTClib.h"
#include "defines.h"
#include "light_ctrl.h"


#define SET_CLOCK /*sets the current time in the clock on startup (shouldnt be used in final)*/

/*Realt Time Clock*/
RTC_DS1307 rtc;
/*The lighting control class*/
light_control light_ctrl;

//Local application data
static INTELLI_DATA intel_data;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Intellight Delux - debugging enabled");
#endif

  //Make sure our lighting is up and running before doing anything else
  light_ctrl.init();

  memset(&intel_data, 0, sizeof(INTELLI_DATA));
#ifdef DEBUG
  Serial.println("Start up complete");
#endif
}

void loop() {
  //We need to check the time and return what time of the day it is
  intel_data.current_time = rtc.now();//update or time so its current
  /*Converts the current time into a phase object used to control the lighting*/
  get_day_phase();


  delay(MAIN_DELAY);
}

static void get_day_phase()
{
  //We will simplily use hours at the moment to change phase
  //We olny want to action the change at the start of the hour, we do this to solve one simple issue
  //Pressing the button effectively cancels the current phase - we dont want the phase to then in turn be cancelled by this
  if (intel_data.current_time.minute() == 0) {
    switch (intel_data.current_time.hour())
    {
      case HOUR_DAY_PHASE:
        intel_data.time_phase.current = HOUR_DAY_PHASE;
        break;
      case HOUR_EVE_PHASE:
        intel_data.time_phase.current = HOUR_EVE_PHASE;
        break;
      case HOUR_NIGHT_PHASE:
        intel_data.time_phase.current = HOUR_NIGHT_PHASE;
        break;
      case HOUR_OFF_PHASE:
        intel_data.time_phase.current = HOUR_OFF_PHASE;
        break;
      default:
        //Do nothing as we dont want to change the phase until next phase shift - hehe
        break;
    }
  }/*This is not the 0 minute so do nothing*/

  if (intel_data.time_phase.current == HOUR_DAY_PHASE)
  {
    /*Work out what minute phase we are in, the day*/
    uint8_t hours = (intel_data.current_time.hour() - HOUR_DAY_PHASE);
    intel_data.time_phase.day_minute = (hours * 60);
    intel_data.time_phase.day_minute += intel_data.current_time.minute();

  }

#ifdef DEBUG
  DateTime now = intel_data.current_time;
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(now.dayOfTheWeek(), DEC);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), HEX);
  Serial.println();

  if (intel_data.time_phase.current == HOUR_DAY_PHASE)
  {
    Serial.print("Day minutes: ");
    Serial.println(intel_data.time_phase.day_minute, HEX);
  }
#endif
}


static void setup_rtc()
{
  Wire.begin();
  rtc.begin();

#ifdef SET_CLOCK
  //when in test mode actaul set the real time and date
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#endif

  if (! rtc.isrunning())
  {
    //If the clock is not running then this means something has gone wrong. Its batt backup so in theory once its up its good to go forever.
    //We should tell the user the clock is not set as they will be able to set the clock with a button hold.
    light_ctrl.show_error(ERROR_RTC_FAIL);
#ifdef DEBUG
    Serial.println("RTC not running");
#endif
  }
}
