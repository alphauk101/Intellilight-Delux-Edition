/*****************
 * Intelli light delux
 * The fish tank light returns this time hes angry!
 *****************/
#include <Wire.h>
#include "RTClib.h"
#include "defines.h"

#define SET_CLOCK /*sets the current time in the clock on startup (shouldnt be used in final)*/

/*Realt Time Clock*/
RTC_DS1307 rtc;

 
void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Intellight Delux - debugging enabled");
#endif

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Intelli 5 - debugging enabled");
#endif
  

}

void loop() {
  // put your main code here, to run repeatedly:

}



static void setup_rtc()
{
  Wire.begin();
  rtc.begin();


  if (! rtc.isrunning())
  {
    //If the clock is not running then this means something has gone wrong. Its batt backup so in theory once its up its good to go forever.
    //We should tell the user the clock is not set as they will be able to set the clock with a button hold.
    //In test mode always sets the time
#ifdef TEST_MODE
    //when in test mode actaul set the real time and date
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
#else
    lighting.show_error(ERROR_RTC_FAIL);
#endif
  }
}
