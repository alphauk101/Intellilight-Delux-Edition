#include "time_ctrl.h"

#define SET_CLOCK /*sets the current time in the clock on startup (shouldnt be used in final)*/

bool time_ctrl::time_init()
{
  Wire.begin();
  rtc.begin();

#ifdef SET_CLOCK
  //when in test mode actaul set the real time and date
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.adjust(DateTime(2000, 1, 1, 8, 59, 0));
#endif

  return rtc.isrunning();
}



void time_ctrl::get_time_update(INTELLI_DATA *data_ptr)
{
  //We will simplily use hours at the moment to change phase
  //We olny want to action the change at the start of the hour, we do this to solve one simple issue
  //Pressing the button effectively cancels the current phase - we dont want the phase to then in turn be cancelled by this.

  /*First get the actual time*/
  DateTime time_now = rtc.now();

  /*Now convert this into a phase*/
  data_ptr->time_phase.current = this->get_phase(&time_now);

  /*we should have our phase - now work out the minutes, except for OFF as its pointless*/
  if (data_ptr->time_phase.current != OFF)
  {
    /*Work out what minute phase we are in, the day*/
    uint8_t hours = (time_now.hour() - data_ptr->time_phase.current);
    data_ptr->time_phase.elapsed = (hours * 60);
    data_ptr->time_phase.elapsed += time_now.minute();
  } else {
    /*OFF mode*/
    data_ptr->time_phase.elapsed = 0;
  }


  /*Debugging purposes only*/
#ifdef DEBUG
  Serial.print(time_now.year(), DEC);
  Serial.print('/');
  Serial.print(time_now.month(), DEC);
  Serial.print('/');
  Serial.print(time_now.day(), DEC);
  Serial.print(" (");
  Serial.print(time_now.dayOfTheWeek(), DEC);
  Serial.print(") ");
  Serial.print(time_now.hour(), DEC);
  Serial.print(':');
  Serial.print(time_now.minute(), DEC);
  Serial.print(':');
  Serial.println(time_now.second(), DEC);
  Serial.println("------------");

  switch (data_ptr->time_phase.current)
  {
    case DAY:
      Serial.print("Day phase, minutes: ");
      Serial.println(data_ptr->time_phase.elapsed, DEC);
      break;
    case EVE:
      Serial.print("Evening phase, minutes: ");
      Serial.println(data_ptr->time_phase.elapsed, DEC);
      break;
    case NIGHT:
      Serial.print("Night phase, minutes: ");
      Serial.println(data_ptr->time_phase.elapsed, DEC);
      break;
    case OFF:
      Serial.print("Off phase");
      break;
  }
  Serial.println();

#endif



}

/*get time phase based on the given time object*/
LIGHT_PHASE time_ctrl::get_phase(DateTime *dt_ptr)
{
  /*first day*/
  if ( (dt_ptr->hour() >= DAY) && (dt_ptr->hour() < EVE) )
  {
    /*Then we must be in day*/
    return DAY;
  } else if ( (dt_ptr->hour() >= EVE) && (dt_ptr->hour() < NIGHT) )
  {
    return EVE;
  } else if ( (dt_ptr->hour() >= NIGHT) && (dt_ptr->hour() < OFF) )
  {
    return NIGHT;
  } else {
    return OFF;
  }
}






