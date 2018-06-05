#ifndef __LIGHTING_H__
#define __LIGHTING_H__
#include <arduino.h>
#include "defines.h"
#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
#include <avr/power.h>
#endif

#define NEW_CODE /*Any recent changes are enabled by this*/


//We have multiple modes during evening (evening being our important time)
typedef byte EVE_EFFECT;
#define STANDARD  0
#define ROSE      1
#define SKY       2
#define GREEN     3
#define SUN_LED   strip.Color(255, 255, 0) /*the sun led*/
static EVE_EFFECT eve_effect;

#define LED_DATA_PIN      5
#define NUMPIXELS         60 /*Even though we have more the blocks of 50 are paralleled up*/

#define DAY_MIN_DIVIDE    9 /*how many minutes between transistion of sun led ie. every 9 mins the sun changes*/

#define ERROR_RTC_FAIL    0xE1

typedef struct {
  uint8_t red;
  uint8_t green;
  uint8_t blue;

} LED_MATRIX;

class light_control
{
  public:
    void init();
    void show_error(byte);
    void set_light_phase(INTELLI_DATA *);
    void effect_shift_timer(void);
  private:
    //We use this to determine what the last phase was primarily so we know when to transist and when not to do anything
    LIGHT_PHASE _last_phase;
    uint16_t min_count; /*daya minute counter*/
    void set_night_mode(bool);
    void fade_current_lights(void);
    void rainbowCycle(uint8_t);
    uint32_t Wheel(byte);
    void colorWipe(uint32_t , uint8_t);
    void set_day_mode(bool , INTELLI_DATA *);
    void set_eve_mode(bool);
    void set_off_mode(bool);
    void set_rgb_level(uint8_t , bool );
    void set_night_step(uint8_t);
    void set_red_tint(void);
    void set_effect(EVE_EFFECT, EVE_EFFECT);
    bool update_pixel_transistion(LED_MATRIX new_c, uint16_t pix);
    uint8_t compare_pix_ammend(uint8_t old_col, uint8_t new_col);
    void apply_matrix(void);
    void set_day_sky(uint16_t);
};



#endif /*__LIGHTING_H__*/
