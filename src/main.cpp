#include <Arduino.h>
#include <FastLED.h>
//#include <Adafruit_NeoPixel.h>

#define NEOPIXEL_PIN 4
#define NUM_LEDS 20
CRGB leds[NUM_LEDS];
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);


// Heavy inspiration taken from https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectFadeInandFadeOutRedGreenandBlue


int randomChoice;


//prototypes

void showStrip();
void setPixel(int, byte, byte, byte);
void setAll(byte, byte, byte);
void RGBLoop();
void strobe(byte, byte, byte, int, int, int);
void twinkleRandom(int, int, boolean);
void leftToRight(byte, byte, byte, int, int, int);
void rightToLeft(byte, byte, byte, int, int, int);
void outsideToCenter(byte, byte, byte, int, int, int);
void centerToOutside(byte, byte, byte, int, int, int);
void newKITT(byte, byte, byte, int, int, int);

void setup()
{
  FastLED.addLeds<WS2811, NEOPIXEL_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);


  /* USE FOR NEOPIXEL LIBRARY */
  //strip.begin();
  //strip.show(); // Initialize all pixels to 'off'
  
  randomSeed(analogRead(0));
  randomChoice = random(1, 5);
}


void loop()
{
  // ---> here we call the effect function <---
  switch (randomChoice)
  {
  case 1:
    RGBLoop();
    break;
  case 2:
    strobe(0xff, 0xff, 0xff, 10, 50, 1000);
    break;
  case 3:
    twinkleRandom(20, 100, false);
    break;
  case 4:
    newKITT(0xff, 0, 0, 8, 10, 50);
    break;
  default:
    break;
  }
}


void showStrip()
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}


void setPixel(int Pixel, byte red, byte green, byte blue)
{
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}


void setAll(byte red, byte green, byte blue)
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    setPixel(i, red, green, blue);
  }
  showStrip();
}


void RGBLoop()
{
  for (int j = 0; j < 3; j++)
  {
    // Fade IN
    for (int k = 0; k < 256; k++)
    {
      switch (j)
      {
      case 0:
        setAll(k, 0, 0);
        break;
      case 1:
        setAll(0, k, 0);
        break;
      case 2:
        setAll(0, 0, k);
        break;
      }
      showStrip();
      delay(3);
    }
    // Fade OUT
    for (int k = 255; k >= 0; k--)
    {
      switch (j)
      {
      case 0:
        setAll(k, 0, 0);
        break;
      case 1:
        setAll(0, k, 0);
        break;
      case 2:
        setAll(0, 0, k);
        break;
      }
      showStrip();
      delay(3);
    }
  }
}


void strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause)
{
  for (int j = 0; j < StrobeCount; j++)
  {
    setAll(red, green, blue);
    showStrip();
    delay(FlashDelay);
    setAll(0, 0, 0);
    showStrip();
    delay(FlashDelay);
  }

  delay(EndPause);
}

void twinkleRandom(int Count, int SpeedDelay, boolean OnlyOne)
{
  setAll(0, 0, 0);

  for (int i = 0; i < Count; i++)
  {
    setPixel(random(NUM_LEDS), random(0, 255), random(0, 255), random(0, 255));
    showStrip();
    delay(SpeedDelay);
    if (OnlyOne)
    {
      setAll(0, 0, 0);
    }
  }

  delay(SpeedDelay);
}

void centerToOutside(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = ((NUM_LEDS - EyeSize) / 2); i >= 0; i--)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void outsideToCenter(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i <= ((NUM_LEDS - EyeSize) / 2); i++)
  {
    setAll(0, 0, 0);

    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);

    setPixel(NUM_LEDS - i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(NUM_LEDS - i - j, red, green, blue);
    }
    setPixel(NUM_LEDS - i - EyeSize - 1, red / 10, green / 10, blue / 10);

    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void leftToRight(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i < NUM_LEDS - EyeSize - 2; i++)
  {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void rightToLeft(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  for (int i = NUM_LEDS - EyeSize - 2; i > 0; i--)
  {
    setAll(0, 0, 0);
    setPixel(i, red / 10, green / 10, blue / 10);
    for (int j = 1; j <= EyeSize; j++)
    {
      setPixel(i + j, red, green, blue);
    }
    setPixel(i + EyeSize + 1, red / 10, green / 10, blue / 10);
    showStrip();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);
}

void newKITT(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay)
{
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  leftToRight(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  rightToLeft(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  outsideToCenter(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
  centerToOutside(red, green, blue, EyeSize, SpeedDelay, ReturnDelay);
}