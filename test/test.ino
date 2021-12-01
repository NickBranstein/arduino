#include <FastLED.h>

typedef void (*GenericFP)();

#define NUMLEDS 29
#define DATAPIN 2

CRGB leds[NUMLEDS];
CRGB randomColors[] = {CRGB::Red, CRGB::Green, CRGB::Indigo, CRGB::Blue, CRGB::Orange, CRGB::Yellow, CRGB::Violet};
int bells[] = {4, 8, 12, 16, 21, 24};
int lightPairs1[] = {0, 1, 2, 3, 9, 10, 11, 17, 18, 19, 25, 26, 27, 28};
int lightPairs2[] = {5, 6, 7, 13, 14, 15, 21, 22, 23};
GenericFP patterns[] = {&alternateLightPairs, &allLightsOneRandomColor, &walkColors, &randomizeAllLights};
int patternFunction = 0;
int patternLoop = 1;

// the setup function runs once when you press reset or power the board
void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(DATAPIN, OUTPUT); // setup pin 2 as output

  FastLED.addLeds<WS2811, DATAPIN, RGB>(leds, NUMLEDS);
  FastLED.clear();

  for (int i = 0; i < NUMLEDS; i++)
  {
    if (isBell(i))
    {
      leds[i] = CRGB::Black; // WHite
    }
  }

  delay(2000); //wait two seconds to settle in // just in case
}

// the loop function runs over and over again forever
void loop()
{
  // leds[0] = CRGB::Red;
  // leds[1] = CRGB::Green;
  // for (int i = 2; i < NUMLEDS; i++)
  // {
  //   leds[i] = CRGB(0, 0, 0);
  // }

  // // leds[1] = CRGB::Black;
  //randomizeAllLights();
  patterns[patternFunction]();

  FastLED.show();
  delay(1000);

  patternLoop += 1;

  if (patternLoop >= 5)
  {
    patternFunction = (patternFunction + 1) % 4;
    patternLoop = 1;
  }
}

void randomizeAllLights()
{
  for (int i = 0; i < NUMLEDS; i++)
  {
    if (!isBell(i))
    {
      leds[i] = randomColor();
    }
  }
}

void alternateLightPairs()
{
  CRGB color1 = randomColor();
  CRGB color2 = randomColor();
  int lightPair1Length = sizeof(lightPairs1) / sizeof(int);
  int lightPair2Length = sizeof(lightPairs2) / sizeof(int);

  for (int j = 0; j < 5; j++)
  {
    for (int i = 0; i < lightPair1Length; i++)
    {
      leds[lightPairs1[i]] = color1;
    }

    for (int i = 0; i < lightPair2Length; i++)
    {
      leds[lightPairs2[i]] = CRGB::Black;
    }

    FastLED.show();

    delay(500);

    for (int i = 0; i < lightPair1Length; i++)
    {
      leds[lightPairs1[i]] = CRGB::Black;
    }

    for (int i = 0; i < lightPair2Length; i++)
    {
      leds[lightPairs2[i]] = color2;
    }

    FastLED.show();

    delay(500);
  }
}

void allLightsOneRandomColor()
{
  CRGB color = randomColor();

  for (int i = 0; i < NUMLEDS; i++)
  {
    if (!isBell(i))
    {
      leds[i] = color;
    }
  }
}

void walkColors()
{
  for (int i = 0; i < NUMLEDS; i++)
  {
    if (!isBell(i))
    {
      leds[i] = CRGB(0, 0, 0); // turn all leds off
    }
  }

  FastLED.show();

  for (int i = 0; i < NUMLEDS; i++)
  {
    if (!isBell(i))
    {
      leds[i] = randomColor();
      FastLED.show();
      delay(100);
    }
  }

  for (int i = NUMLEDS - 1; i >= 0; i--)
  {
    leds[i] = CRGB(0, 0, 0);
    FastLED.show();
    delay(100);
  }
}

void blinkInternalLight()
{
  int x = random(1000, 5000);

  digitalWrite(LED_BUILTIN, HIGH); // turn the LED on (HIGH is the voltage level)
  delay(x);                        // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // turn the LED off by making the voltage LOW
  delay(x);                        // wait for a second
}

CRGB randomColor()
{
  return randomColors[random(0, 7)];
}

bool isBell(int value)
{
  return value == 4 || value == 8 || value == 12 || value == 16 || value == 20 || value == 24;
}