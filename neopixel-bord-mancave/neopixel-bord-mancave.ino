#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define TOP_PIN D2
#define BOTTOM_PIN D1
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define NUM_LEDS    56
#define MANCAVE  0x5688C7

CRGB top[NUM_LEDS];
CRGB bottom[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  60

void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,TOP_PIN>(top, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE,BOTTOM_PIN>(bottom, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {intro, mancave, rainbow, rainbowWithGlitter, confetti, sinelon, juggle };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
  
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void intro(){
  for( int i = 0; i < NUM_LEDS; i++) {
    top[i] = MANCAVE;
    bottom[i] = MANCAVE;
  }
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( top, NUM_LEDS, gHue, 7);
  fill_rainbow( bottom, NUM_LEDS, gHue, 7);

}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    top[ random16(NUM_LEDS) ] += CRGB::White;
    bottom[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( top, NUM_LEDS, 20);
  fadeToBlackBy( bottom, NUM_LEDS, 20);  int pos = random16(NUM_LEDS);
  top[pos] += CHSV( gHue + random8(64), 200, 255);
  bottom[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( top, NUM_LEDS, 20);
  fadeToBlackBy( bottom, NUM_LEDS, 20);  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  top[pos] += CHSV( gHue, 255, 192);
  bottom[pos] += CHSV( gHue, 255, 192);
}

void mancave()
{

  for( int i = 0; i < NUM_LEDS; i++) { //9948
    bottom[i] = MANCAVE;
    top[i] = MANCAVE;
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( top, NUM_LEDS, 20);
  fadeToBlackBy( bottom, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    top[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    bottom[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

