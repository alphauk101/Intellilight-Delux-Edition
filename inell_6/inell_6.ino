/*****************
   Intelli light delux
   The fish tank light returns this time hes angry!
 *****************/

#include "defines.h"
#include "light_ctrl.h"
#include "time_ctrl.h"


/*The lighting control class*/
light_control light_ctrl;
time_ctrl time_ctl;

//Local application data
static INTELLI_DATA intel_data;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Intellight Delux - debugging enabled");
#endif

  time_ctl.time_init();//Important: setup the rtc comms

  //Make sure our lighting is up and running before doing anything else
  light_ctrl.init();

  memset(&intel_data, 0, sizeof(INTELLI_DATA));
#ifdef DEBUG
  Serial.println("Start up complete");
#endif
}

void loop() {
  /*update the data pointer object*/
  time_ctl.get_time_update(&intel_data);


  delay(MAIN_DELAY);
}





