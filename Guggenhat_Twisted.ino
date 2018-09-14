// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Modified by Philip Heady to scroll - THIS SPACE FOR RENT -
// added StrandTest by Adafruit and fit it into the program for effects

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN D5 // easiest pin for me to hook to

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Matrix.  In this application we'd like to use it
// as two 32 x 8 tall matrix, with the USB port positioned at the bottom of the matrix
// with two matrix then I added togeter to make one  64 x 8 matrix (hopefully)
// for a total of 512 pixels for the Adafruit Neo-Strip functions.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(64, 8, PIN,
  NEO_MATRIX_BOTTOM     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);
  // added neopixel strip for effect functions
  Adafruit_NeoPixel strip = Adafruit_NeoPixel(512, PIN, NEO_GRB + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

void setup() { // setup neo-matrix and neo-strip
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(15);
  matrix.setTextColor(colors[0]);
  strip.begin();
  strip.setBrightness(15);
  strip.show(); // Initialize all pixels to 'off'
}
// set values for matrix width, pass number, and cycle number
// pass is the times text goes round and cycle lets the text scroll 3 times
// one pass for each color - red, green, and blue.
int x    = matrix.width();
int pass = 0;
int Cycle = 0;


void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(F("THIS SPACE FOR RENT"));
  if(--x < -112) {     // moves text around width
    x = matrix.width();
    if(++pass >= 3){
      pass = 0;
      Cycle = Cycle + 1;  // old school ++Cycle
    }
    matrix.setTextColor(colors[pass]);
    if (Cycle >= 9) Cycle = 0;
  }
  matrix.show();
  delay(40);
  if(Cycle == 1){                      // == compares : = changes
    theaterChaseRainbow(5);
    Cycle = Cycle + 1;   // Cycle should now be 2
  }
  if(Cycle == 3) {
    rainbowCycle(5);
    Cycle = Cycle + 1;  // Cycle should now be 4
  }
  if(Cycle == 5){
    theaterChase(strip.Color(127, 127, 127), 30);
    theaterChase(strip.Color(0, 0, 255), 30);
    theaterChase(strip.Color(255, 0, 0), 30);
    theaterChase(strip.Color(0, 255, 0), 30);
    theaterChase(strip.Color(127, 127, 127), 30);
    Cycle = Cycle +1;  // Cycle should now be 6
  }
  if(Cycle == 7){
    colorWipe(strip.Color(255, 0, 0), 1); // Red
    colorWipe(strip.Color(0, 255, 0), 1); // Green
    colorWipe(strip.Color(0, 0, 255), 1); // Blue
    Cycle = Cycle + 1;  // Cyce should now be 8
  }
  
}
// adding the functions for the strip calls

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
// wish i understood this better - would make for interesting effects!
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
