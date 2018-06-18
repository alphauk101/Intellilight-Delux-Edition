#include "light_ctrl.h"



// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

static LED_MATRIX g_led_matrix[NUMPIXELS];

void light_control::init()
{
#ifdef DEBUG
  Serial.println("Lighting starting");
#endif
  strip.begin(); // This initializes the NeoPixel library.
  //On init we may want to makesure the LEDs are in a good state

  //To show the unit is init'ing we do a little chase:
  rainbowCycle(5);
  colorWipe(strip.Color(0, 0, 0), 20); // Off

  _last_phase = OFF;//They are off

  //Set this to the standard effect on boot
  eve_effect = STANDARD;

  min_count = 0;
}

void light_control::set_light_phase(INTELLI_DATA *light_data_ptr)
{
  bool trans = false;
  //First thing we do is check whether we are changing phase

  if (light_data_ptr->time_phase.current != this->_last_phase)
  {
    //Now we need to go to whichever lighting is next
    trans = true; // we do a transistion
    //Lastly update of current light state
    this->_last_phase = light_data_ptr->time_phase.current;
  } else {
    trans = false;
  }

  //now set the appropriate mode

  switch (light_data_ptr->time_phase.current)
  {
    case DAY:
      this->set_day_mode(trans, light_data_ptr);
      break;
    case EVE:
      this->set_eve_mode(trans);
      min_count = 0;
      break;
    case NIGHT:
      this->set_night_mode(trans);
      min_count = 0;
      break;
    case OFF:
      this->set_off_mode(trans);
      min_count = 0;
      break;
    default:
      min_count = 0;
      break;
  }
}

void light_control::set_night_mode(bool trans) 
{
  //If we are trans'ing then we need to fade up to out night mode problem is our rg lights are duller than the b lights
  //For now (and this will change) we will fade them up indepedantly

  if (trans) { //were transitioning
    this->trans_eve_night();
  }


  /*Allows all leds to be set in night mode at the given brightness*/
  void light_control::set_night_step(uint8_t brightness)
  {
    for (uint8_t j = 0; j < strip.numPixels(); j++) {

      if ((j % 5) == 0) {
        strip.setPixelColor(j, strip.Color(NIGHT_WHITE_INTENSITY, NIGHT_WHITE_INTENSITY, NIGHT_WHITE_INTENSITY));
      } else {
        strip.setPixelColor(j, strip.Color(0, 0, NIGHT_BLUE_INTENSITY));
      }

    }
    //Once all the leds have been stored we can update the display
    strip.setBrightness(brightness);
    strip.show();
  }

  /*set day mode
    pass true to the function causes a transistion**/
  uint16_t last_change;
  void light_control::set_day_mode(bool trans, INTELLI_DATA * light_data_ptr)
  {

    /*In day mode we have a low white led and a yellow led which changes throughout the day*/

    /*now using the minutes workout which led is yellow*/
    //DAY_MIN_DIVIDE

    if (trans)
    {
      /*This is where we do the transistion to make the thing less jerky when changing
        we know in this state that the transistion will be off -> day so*/
      this->trans_off_day();
    }



    /*now work out the day minutes*/
    if ( ((light_data_ptr->time_phase.elapsed % DAY_MIN_DIVIDE) == 0) &&
         (light_data_ptr->time_phase.elapsed != last_change) )
    {
      min_count++;
      last_change = light_data_ptr->time_phase.elapsed;
    }

    //Serial.print("Sun index: ");
    //Serial.println(min_count, DEC);
    /*first set all the LEDs*/
    this->set_day_sky(min_count);
  }

  /*From eve to night*/
  void light_control::trans_eve_night() {
    strip.clear();
    strip.setBrightness(255);


    for (uint16_t b = 0 ; b < EVE_LED_BRIGHTNESS; b++)
    {
      for (uint16_t a = 0 ; a < strip.numPixels(); a++)
      {
        if ((a % 3) == 0) {
          strip.setPixelColor(a, strip.Color(0, 0, NIGHT_LED_BRIGHTNESS));
        } else {
          strip.setPixelColor(a, strip.Color((NIGHT_LED_BRIGHTNESS - b), (NIGHT_LED_BRIGHTNESS - b), NIGHT_LED_BRIGHTNESS));
        }
      }

      strip.show();
      delay(50);
    }

  }


  /*Transist from day to eve*/
  void light_control::trans_day_eve() {


    strip.clear();
    strip.setBrightness(255);

    for (uint16_t b = DAY_LED_BRIGHTNESS ; b < EVE_LED_BRIGHTNESS; b++)
    {
      //strip.setBrightness(b);

      for (uint16_t a = 0 ; a < strip.numPixels(); a++)
      {
        if ((a % 3) == 0) {
          strip.setPixelColor(a, strip.Color(0, 0, b));
        } else {
          strip.setPixelColor(a, strip.Color(b, b, b));
        }
      }
      strip.show();
      delay(50);
    }
  }


  void light_control::trans_off_day()
  {
    //Serial.println("trans off -> day");
    strip.clear();
    strip.setBrightness(255);

    for (uint16_t b = 0 ; b < DAY_LED_BRIGHTNESS; b++)
    {
      //strip.setBrightness(b);

      for (uint16_t a = 0 ; a < strip.numPixels(); a++)
      {
        if ((a % 2) == 0) {
          strip.setPixelColor(a, strip.Color(b, b, b));
        } else {
          strip.setPixelColor(a, strip.Color(0, 0, b));
        }
      }
      strip.show();
      delay(50);
    }
    //all done!
  }


  void light_control::set_day_sky(uint16_t sun_idx)
  {
    strip.clear();
    strip.setBrightness(255);
    for (uint16_t a = 0 ; a < strip.numPixels(); a++)
    {
      //Serial.print(sun_idx,DEC);
      if (a != sun_idx)
      {

        if ((a % 2) == 0) {
          strip.setPixelColor(a, strip.Color(DAY_LED_BRIGHTNESS, DAY_LED_BRIGHTNESS, DAY_LED_BRIGHTNESS));
        } else {
          strip.setPixelColor(a, strip.Color(0, 0, DAY_LED_BRIGHTNESS));
        }

      }
      else
      {
        strip.setPixelColor(a, strip.Color(RGB_LED_INTENSITY, RGB_LED_INTENSITY, 50));
      }
    }
    //this->apply_matrix();
    //DAY_LED_BRIGHTNESS
    //Serial.println("update strip");
    //strip.setBrightness(DAY_LED_BRIGHTNESS);
    strip.show();
    //delay(500);
  }

  /*set eve mode*/
  EVE_EFFECT prev_effect;
  void light_control::set_eve_mode(bool trans)
  {
    if (trans) {
      this->trans_day_eve();
    }

    strip.clear();
    strip.setBrightness(255);

    for (uint16_t a = 0 ; a < strip.numPixels(); a++)
    {
      if ((a % 3) == 0) {
        strip.setPixelColor(a, strip.Color(0, 0, NIGHT_LED_BRIGHTNESS));
      } else {
        strip.setPixelColor(a, strip.Color(NIGHT_LED_BRIGHTNESS, NIGHT_LED_BRIGHTNESS, NIGHT_LED_BRIGHTNESS));
      }
    }
    strip.show();
  }




  /*set off mode*/
  void light_control::set_off_mode(bool trans)
  {
    strip.clear();
    strip.show();
  }



  // Fill the dots one after the other with a color
  void light_control::colorWipe(uint32_t c, uint8_t wait) {
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
    }
  }

  // Slightly different, this makes the rainbow equally distributed throughout
  void light_control::rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for (j = 0; j < 256 * 2; j++) { // 5 cycles of all colors on wheel
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }


  // Input a value 0 to 255 to get a color value.
  // The colours are a transition r - g - b - back to r.
  uint32_t light_control::Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) {
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }

  //Used to change the evening effect driven by the RTC
  void light_control::effect_shift_timer()
  {
    eve_effect = random(0, 4);
    /*
      eve_effect++;
      if (eve_effect > GREEN) eve_effect = STANDARD;
    */
  }

