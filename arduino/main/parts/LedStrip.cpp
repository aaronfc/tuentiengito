#include <FastLED.h>

// LED Strip config and init
#define LED_PIN     4
#define NUM_LEDS    30
#define BRIGHTNESS  128
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

void initLedStrip() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}


//#define UPDATES_PER_SECOND 100
#define UPDATES_PER_SECOND 1024  // 5 * 64

unsigned tick = 0;

int getNewLedsMode(char* readLine)
{
    char* commandWord;
    commandWord = strtok(readLine, " \n;");

    if (strcmp(commandWord, "LEDS_OFF") == 0) {
        return 0;
    } else if (strcmp(commandWord, "LEDS_KNIGHTRIDER") == 0) {
        return 1;
    } else if (strcmp(commandWord, "LEDS_RAINBOW") == 0) {
        return 2;
    } else if (strcmp(commandWord, "LEDS_ALARM") == 0) {
        return 3;
    } else {
        return -1;
    }
}

int ledsMode = 1;

#define HUE_GAP 16
#define DISTANCE 16
#define TRAIL_LEDS 9

void setKnightRiderColors(unsigned tick0) {
    int tick = tick0 / DISTANCE;
    int unit = (256 / (TRAIL_LEDS-1));
    int brightTick = (tick % DISTANCE) * (unit / DISTANCE);

    int hueIndex = tick / (NUM_LEDS * 2);
    int hue = (HUE_RED + (hueIndex * HUE_GAP)) % 256;

    int mainIndex = (tick % (NUM_LEDS * 2)) % (NUM_LEDS*2);
    int contraIndex = ((NUM_LEDS*2) - mainIndex) % (NUM_LEDS*2);
    boolean plus = (tick / NUM_LEDS) % 2 > 0;
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
        for (int j = TRAIL_LEDS-1; j>=0; j--) {
          if ((i == mainIndex + j) || (i == contraIndex - j)) {
            int ledBright = ((j * unit) + brightTick) % 256;
            CHSV newColor = CHSV(hue, 255, ledBright);

//            if (leds[i].getAverageLight() < ledBright) {
              leds[i] = newColor;
//            }
          }
        }
    }
}

void setRainbowColors(unsigned tick0) {
    int tick = tick0 / DISTANCE;
    int unit = (256 / (TRAIL_LEDS-1));
    int brightTick = (tick % DISTANCE) * (unit / DISTANCE);

    for( int i = 0; i < NUM_LEDS; i++) {
      int hue = (tick0 + i) % 256;
      leds[i] = CHSV(hue, 255, 255);
    }
}

void setConstantAll(CRGB color) {
    for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
}

void setAlarmMode(unsigned tick) {
    CRGB color = CRGB::Black;
    if (((tick / 128) % 2) == 1) {
      color = CRGB::Red;
    }
    for( int i = 0; i < NUM_LEDS; i++) {
      leds[i] = color;
    }
}


void refreshLeds(char* command)
{
    int newLedsMode = getNewLedsMode(command);
    if (newLedsMode != -1) {
        ledsMode = newLedsMode;
    }

    if (ledsMode == 0) {
        setConstantAll(CRGB::Black);
    } else if (ledsMode == 1) {
        setKnightRiderColors(tick);
    } else if (ledsMode == 2) {
        setRainbowColors(tick);
    } else if (ledsMode == 3) {
        setAlarmMode(tick);
    }

    FastLED.show();
    //FastLED.delay(1000 / UPDATES_PER_SECOND);
    tick++;

    // Tacky control trying to avoid negative values
    //if (tick == UPDATES_PER_SECOND*DISTANCE)  tick = 0;
}
