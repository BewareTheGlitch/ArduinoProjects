// --------------------------------
// -- VU Meter - Scott's version --
// -- Modified, chopped & channeled by Jim --
// --------------------------------

#include <FastLED.h>
#include <JC_Button.h>

# define LEFT_OUT_PIN 6             // Left channel data out pin to LEDs [6]
# define RIGHT_OUT_PIN 5            // Right channel data out pin to LEDs [5]
# define LEFT_IN_PIN A5             // Left aux in signal [A5]
# define RIGHT_IN_PIN A4            // Right aux in signal [A4]
# define WHITE 10                   // White LED control 
# define BTN_PIN 3                  // Push button on this pin [3]
# define DEBOUNCE_MS 20             // Number of ms to debounce the button [20]
# define LONG_PRESS 500             // Number of ms to hold the button to count as long press [500]
# define N_PIXELS 15                 // Number of pixels in each string [24]
# define MAX_MILLIAMPS 100          // Maximum current to draw [500]
# define COLOR_ORDER GRB            // Colour order of LED strip [GRB]
# define LED_TYPE WS2812B           // LED string type [WS2812B]
# define DC_OFFSET 0                // DC offset in aux signal [0]
# define NOISE 35                  // Noise/hum/interference in aux signal [10]
# define SAMPLES 60                 // Length of buffer for dynamic level adjustment [60]
# define TOP (N_PIXELS + 2)         // Allow dot to go slightly off scale [(N_PIXELS + 2)]
# define PEAK_FALL 20               // Rate of peak falling dot [20]
# define N_PIXELS_HALF (N_PIXELS / 2)
# define PATTERN_TIME 10            // Seconds to show eaach pattern on auto [10]
# define STEREO true                // If true, L&R channels are independent. If false, both L&R outputs display same data from L audio channel [false]


uint8_t volCountLeft = 0;           // Frame counter for storing past volume data
int volLeft[SAMPLES];               // Collection of prior volume samples
int lvlLeft = 0;                    // Current "dampened" audio level
int minLvlAvgLeft = 0;              // For dynamic adjustment of graph low & high
int maxLvlAvgLeft = 512;

uint8_t volCountRight = 0;          // Frame counter for storing past volume data
int volRight[SAMPLES];              // Collection of prior volume samples
int lvlRight = 0;                   // Current "dampened" audio level
int minLvlAvgRight = 0;             // For dynamic adjustment of graph low & high
int maxLvlAvgRight = 512;
int patternCounter = 0;             // Pattern state counter
uint8_t lampMode = 0;               // Toggles lantern or VU mode

CRGB ledsLeft[N_PIXELS];
CRGB ledsRight[N_PIXELS];

uint8_t myhue = 0;

const int brightWhite[5] = {0, 1, 64, 128, 255};

void vu4(bool is_centered, uint8_t channel);
void vu5(bool is_centered, uint8_t channel);
void vu6(bool is_centered, uint8_t channel);
void vu7(bool show_background);
void vu8();
void vu9();
void vu10();
void balls();
void fire();
void juggle();
void ripple(boolean show_background);
void sinelon();
void twinkle();
void rainbow(uint8_t rate);


uint8_t state = 0;
int buttonPushCounter = 0; //Tracks the current visualizer when in VU mode
int lampStateCounter = 0; //Tracks the LED brightness & state when in lantern mode

Button modeBtn(BTN_PIN, DEBOUNCE_MS);

void incrementButtonPushCounter() {  //counter to step through the visualizer modes
  buttonPushCounter = ++buttonPushCounter % 18;
}

void incrementLampStateCounter() {  //counter to step through the lantern modes
  lampStateCounter = ++lampStateCounter % 7;
}

void randomCounter() {    //Function to generate random counter values
  patternCounter = rand() % 17;
}

void blankLEDs() {   //Blanks the RGB strips
  for (uint8_t i = 0; i < N_PIXELS; i++) {
    ledsLeft[i] = CRGB::Black;
    FastLED.show();
    ledsRight[i] = CRGB::Black;
    FastLED.show();

  }
}


void lanternMode() {           //Sets the state of the LEDs in lantern mode
  switch (lampStateCounter) {
    case 0:
      analogWrite(WHITE, 0);
      for (uint8_t i = 0; i < N_PIXELS; i++) {
        ledsLeft[i] = CRGB::Red;
        FastLED.show();
        ledsRight[i] = CRGB::Red;
        FastLED.show();
      }
      break;

    case 1:
      blankLEDs();
      analogWrite(WHITE, 1);
      break;

    case 2:
      blankLEDs();
      analogWrite(WHITE, 8);
      break;

    case 3:
      blankLEDs();
      analogWrite(WHITE, 64);
      break;


    case 4:
      blankLEDs();
      analogWrite(WHITE, 128);
      break;


    case 5:
      blankLEDs();
      analogWrite(WHITE, 255);
      break;

    case 6:
      blankLEDs();
      analogWrite(WHITE, 0);
  }
}
void setup() {
  delay(1000);           // power-up safety delay

  FastLED.addLeds < LED_TYPE, LEFT_OUT_PIN, COLOR_ORDER > (ledsLeft, N_PIXELS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds < LED_TYPE, RIGHT_OUT_PIN, COLOR_ORDER > (ledsRight, N_PIXELS).setCorrection(TypicalLEDStrip);
  //FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_MILLIAMPS);

  modeBtn.begin();
  Serial.begin(57600);

  buttonPushCounter = 0;
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("Starting pattern ");
  Serial.println(buttonPushCounter);

}






void loop() {

  modeBtn.read();                  //Read the state of the button & sets lantern or VU mode
  switch (state) {
    case 0:
      if (modeBtn.wasReleased()) {
        Serial.print("Short press. Lamp mode: ");
        Serial.print(lampMode);
        Serial.print(" VU mode counter: ");
        Serial.print(buttonPushCounter);
        Serial.print(" Lantern mode counter: ");
        Serial.println(lampStateCounter);
        incrementButtonPushCounter();
        incrementLampStateCounter();

      }
      else if (modeBtn.pressedFor(LONG_PRESS))
        state = 1;
      break;

    case 1:
      if (modeBtn.wasReleased()) {
        state = 0;
        if (lampMode == 0) {
          lampMode = 1;
          buttonPushCounter = 0;
          lampStateCounter = 0;
          blankLEDs();

        }
        else if (lampMode == 1) {
          lampMode = 0;
          buttonPushCounter = 0;
          lampStateCounter = 0;
          analogWrite(WHITE, 0);

        }
        //incrementButtonPushCounter();
        //incrementLampStateCounter();
        Serial.print("Long press. Lamp mode: ");
        Serial.print(lampMode);
        Serial.print(" VU mode counter: ");
        Serial.print(buttonPushCounter);
        Serial.print(" Lantern mode counter: ");
        Serial.println(lampStateCounter);

      }
      break;

  }


  if (lampMode == 0) {             // Run selected pattern in VU mode
   
    switch (buttonPushCounter) {
      case 0:
        vu4(false, 0);
        if (STEREO) vu4(false, 1);
        else copyLeftToRight();
        break;

      case 1:
        vu4(true, 0);
        if (STEREO) vu4(true, 1);
        else copyLeftToRight();
        break;

      case 2:
        vu5(false, 0);
        if (STEREO) vu5(false, 1);
        else copyLeftToRight();
        break;

      case 3:
        vu5(true, 0);
        if (STEREO) vu5(true, 1);
        else copyLeftToRight();
        break;

      case 4:
        vu6(false, 0);
        if (STEREO) vu6(false, 1);
        else copyLeftToRight();
        break;

      case 5:
        vu7(true);
        copyLeftToRight();
        break;

      case 6:
        vu8();
        copyLeftToRight();
        break;

      case 7:
        vu9();
        break;

      case 8:
        vu10();
        break;

      case 9:
        vu7(false);
        copyLeftToRight();
        break;

      case 10:
        twinkle();
        break;

      case 11:
        sinelon();
        break;

      case 12:
        balls();
        break;

      case 13:
        juggle();
        break;

      case 14:
        fire();
        break;

      case 15:
        ripple(false);
        break;

      case 16:
        rainbow(10);
        break;

      case 17:
        randomVisual();
        break;


    }
  }
  if (lampMode == 1) {
    lanternMode();
  }
}


void randomVisual() {      //Picks & displays a random visualizer
  EVERY_N_SECONDS(PATTERN_TIME) {
    randomCounter();
  }
  switch (patternCounter) {
    case 0:
      vu4(false, 0);
      if (STEREO) vu4(false, 1);
      else copyLeftToRight();
      break;

    case 1:
      vu4(true, 0);
      if (STEREO) vu4(true, 1);
      else copyLeftToRight();
      break;

    case 2:
      vu5(false, 0);
      if (STEREO) vu5(false, 1);
      else copyLeftToRight();
      break;

    case 3:
      vu5(true, 0);
      if (STEREO) vu5(true, 1);
      else copyLeftToRight();
      break;

    case 4:
      vu6(false, 0);
      if (STEREO) vu6(false, 1);
      else copyLeftToRight();
      break;

    case 5:
      vu7(true);
      copyLeftToRight();
      break;

    case 6:
      vu8();
      copyLeftToRight();
      break;

    case 7:
      vu9();
      break;

    case 8:
      vu10();
      break;

    case 9:
      vu7(false);
      copyLeftToRight();
      break;

    case 10:
      twinkle();
      break;

    case 11:
      sinelon();
      break;

    case 12:
      balls();
      break;

    case 13:
      juggle();
      break;

    case 14:
      fire();
      break;

    case 15:
      ripple(false);
      break;

    case 16:
      rainbow(10);
      break;
  }
}


// ------------------
// -- VU functions --
// ------------------

uint16_t auxReading(uint8_t channel) {

  int n = 0;
  uint16_t height = 0;

  if (channel == 0) {
    int n = analogRead(LEFT_IN_PIN); // Raw reading from left line in
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlLeft = ((lvlLeft * 7) + n) >> 3; // "Dampened" reading else looks twitchy (>>3 is divide by 8)
    volLeft[volCountLeft] = n; // Save sample for dynamic leveling
    volCountLeft = ++volCountLeft % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlLeft - minLvlAvgLeft) / (long)(maxLvlAvgLeft - minLvlAvgLeft);
  }

  else {
    int n = analogRead(RIGHT_IN_PIN); // Raw reading from mic
    n = abs(n - 512 - DC_OFFSET); // Center on zero
    n = (n <= NOISE) ? 0 : (n - NOISE); // Remove noise/hum
    lvlRight = ((lvlRight * 7) + n) >> 3; // "Dampened" reading (else looks twitchy)
    volRight[volCountRight] = n; // Save sample for dynamic leveling
    volCountRight = ++volCountRight % SAMPLES;
    // Calculate bar height based on dynamic min/max levels (fixed point):
    height = TOP * (lvlRight - minLvlAvgRight) / (long)(maxLvlAvgRight - minLvlAvgRight);
  }

  // Calculate bar height based on dynamic min/max levels (fixed point):
  height = constrain(height, 0, TOP);
  return height;
}

void copyLeftToRight() {
  for (uint8_t i = 0; i < N_PIXELS; i++) {
    ledsRight[i] = ledsLeft[i];
  }
}

/*
   Function for dropping the peak
*/
uint8_t peakLeft, peakRight;
void dropPeak(uint8_t channel) {

  static uint8_t dotCountLeft, dotCountRight;

  if (channel == 0) {
    if (++dotCountLeft >= PEAK_FALL) { //fall rate
      if (peakLeft > 0) peakLeft--;
      dotCountLeft = 0;
    }
  } else {
    if (++dotCountRight >= PEAK_FALL) { //fall rate
      if (peakRight > 0) peakRight--;
      dotCountRight = 0;
    }
  }
}

/*
   Function for averaging the sample readings
*/
void averageReadings(uint8_t channel) {

  uint16_t minLvl, maxLvl;

  // minLvl and maxLvl indicate the volume range over prior frames, used
  // for vertically scaling the output graph (so it looks interesting
  // regardless of volume level).  If they're too close together though
  // (e.g. at very low volume levels) the graph becomes super coarse
  // and 'jumpy'...so keep some minimum distance between them (this
  // also lets the graph go to zero when no sound is playing):
  if (channel == 0) {
    minLvl = maxLvl = volLeft[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volLeft[i] < minLvl) minLvl = volLeft[i];
      else if (volLeft[i] > maxLvl) maxLvl = volLeft[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;

    minLvlAvgLeft = (minLvlAvgLeft * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgLeft = (maxLvlAvgLeft * 63 + maxLvl) >> 6; // (fake rolling average)
  }

  else {
    minLvl = maxLvl = volRight[0];
    for (int i = 1; i < SAMPLES; i++) {
      if (volRight[i] < minLvl) minLvl = volRight[i];
      else if (volRight[i] > maxLvl) maxLvl = volRight[i];
    }
    if ((maxLvl - minLvl) < TOP) maxLvl = minLvl + TOP;
    minLvlAvgRight = (minLvlAvgRight * 63 + minLvl) >> 6; // Dampen min/max levels
    maxLvlAvgRight = (maxLvlAvgRight * 63 + maxLvl) >> 6; // (fake rolling average)
  }
}
