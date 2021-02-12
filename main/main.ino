/*
 * Arduino Code for Design Sprint
 * Written by: Trey Howell and Dominic Ritchey
 * 
 * Encoder code is based on the Arduino sketch example provided at https://www.epitran.it/ebayDrive/datasheet/25.pdf
 * LED code is based on the Arduino sketch example DemoReel100 provided by the FastLED library written by Daniel Garcia
 */

#include <FastLED.h>

FASTLED_USING_NAMESPACE

#define ENCODER_PIN_A 3 // Connected to CLK on KY-040
#define ENCODER_PIN_B 4 // Connected to DT on KY-040
int lastEncoderPinAValue;
int encoderPinAValue;

#define LED_DATA_PIN 8
#define LED_BRIGHTNESS 100
#define NUM_LEDS 19
#define LED_TYPE    WS2811
#define LED_COLOR_ORDER GRB
#define LED_FPS 30
CRGB leds[NUM_LEDS];

int secondsElapsed = -1;
boolean washingHands;

void setup() {
  // Set up encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);

  // Set up LEDs
  FastLED.addLeds<LED_TYPE, LED_DATA_PIN, LED_COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(LED_BRIGHTNESS);
  
  // Read pin A (whatever state it's in will reflect the last position)
  lastEncoderPinAValue = digitalRead(ENCODER_PIN_A);

  // Set up logging
  Serial.begin(9600);
}

void loop() {
  encoderPinAValue = digitalRead(ENCODER_PIN_A); // Grab pin A value
  
  if (encoderPinAValue != lastEncoderPinAValue) { // Knob has rotated
    washingHands = true;
  }
  
  lastEncoderPinAValue = encoderPinAValue; // Update last value for pin A

  if (washingHands) {
    secondsElapsed++;
    
    // Print data for diagnostics
    Serial.print(secondsElapsed);
    Serial.println(" seconds elapsed");

    if (secondsElapsed == 23) { // Reset timer after 23 seconds elapsed (20 seconds handwashing + 3 seconds reset)
      washingHands = false;
      secondsElapsed = -1;

      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(0, 0, 0);
      }
    } else if (secondsElapsed == 20) { // Switch LEDs to green after 20 seconds signalling completion
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Green;
      }
    } else { // Illuminate LEDs one by one as counter goes up
      leds[secondsElapsed - 1] = CRGB::Red;
    }

    // Push updates to LEDs
    FastLED.show();
    FastLED.delay(1000 / LED_FPS);

    delay(1000); // Wait for 1 second
  }
}
