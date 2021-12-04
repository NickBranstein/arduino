#include <FastLED.h>

typedef void (*GenericFP)();

#define NUMLEDS 29
#define DATAPIN 2
#define BRIGHTNESS 192
#define FPS 60

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

  FastLED.addLeds<WS2811, DATAPIN, RGB>(leds, NUMLEDS).setCorrection(TypicalPixelString);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();

  for (int i = 0; i < NUMLEDS; i++)
  {
    if (isBell(i))
    {
      leds[i] = CRGB::White;
    }
  }

  delay(2000);
}

// the loop function runs over and over again forever
void loop()
{
  patterns[patternFunction]();

  FastLED.show();
  delay(1000 / FPS);

  EVERY_N_SECONDS(5){
    updatePattern();
  } 
}

template<typename T, size_t S>
size_t length(T(&)[S]){
	return S;
}

void updatePattern(){
   patternLoop += 1;

  if (patternLoop >= length(patterns))
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

  delay(2000);
}

void alternateLightPairs()
{
  CRGB color1 = randomColor();
  CRGB color2 = randomColor();
  
  int lightPair1Length = length(lightPairs1);
  int lightPair2Length = length(lightPairs2);

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

  delay(1000);
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
    if(!isBell(i)){
      leds[i] = CRGB(0, 0, 0);
      FastLED.show();
      delay(100);
    }
  }
}

CRGB randomColor()
{
  return randomColors[random(0, 7)];
}

bool isBell(int value)
{
  return value == 4 || value == 8 || value == 12 || value == 16 || value == 20 || value == 24;
}