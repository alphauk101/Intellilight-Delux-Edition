#ifndef __DEFINES__
#define __DEFINES__
#include "RTClib.h"

#define DEBUG

#define MAIN_DELAY          500

#define LONG_PRESS_COUNT    100 //how long we wait before considering a long press 100 = 5seconds (ish)

#define DAY_LED_BRIGHTNESS      35  //This is the brightness level set to ALL leds when in day mode
#define EVE_LED_BRIGHTNESS      180 //Evening brightness - I'm trying to avoid full power for heat/power reasons
#define NIGHT_LED_BRIGHTNESS    200 //The night brightness
#define OFF_LED_BRIGHTNESS      0   //off mode 

#define RGB_LED_INTENSITY       250 //Individual led intensity (not brightness) during none night modes
#define MAX_LED_INTENSITY       255
#define LOW_LED_INTENSITY       30
#define NIGHT_BLUE_INTENSITY    250
#define NIGHT_WHITE_INTENSITY   60  // The red and green led intens during night 

/*Rose*/
#define ROSE_RED                MAX_LED_INTENSITY
#define ROSE_BLUE               60
#define ROSE_GREEN              60
/*SKY*/
#define SKY_RED                 90
#define SKY_GREEN               180
#define SKY_BLUE                MAX_LED_INTENSITY
/*GREEN*/
#define GREEN_RED               50
#define GREEN_GREEN             MAX_LED_INTENSITY
#define GREEN_BLUE              50

#define TRANS_SPEED_MS      50 //Time of steps during transistion

#define DAY    9/*This inidicates that the day phase starts at 9am*/
#define EVE    18/*eve pahase starts at 6pm*/
#define NIGHT  20 /*Night 8pm*/
#define OFF    23/*Off at midnight*/

typedef uint8_t BUTTON_PRESS;
typedef uint8_t LIGHT_PHASE;

#define SHORT 1
#define LONG  2
#define NONE  3

typedef struct {
  LIGHT_PHASE   current; /*current phase*/

  uint16_t      elapsed; /*the amount of minutes elapsed in to the new phase this is used for effects*/
} TIME_PHASE;

//Our application data structure
typedef struct {
  DateTime        current_time; //Current time

  TIME_PHASE      time_phase; //current light phase

  BUTTON_PRESS    button_press; //Last state of button
} INTELLI_DATA;


#endif
