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
//int encoderPosCount = 0;
int lastEncoderPinAValue;
int encoderPinAValue;
//boolean bCW;

#define LED_DATA_PIN 8
#define LED_BRIGHTNESS 100
#define NUM_LEDS 3
#define LED_TYPE    WS2811
#define LED_COLOR_ORDER GRB
#define LED_FPS 30
CRGB leds[NUM_LEDS];

unsigned long handwashingStarted = 0;
int secondsElapsed = 0;
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
  // Grab pin A value
  encoderPinAValue = digitalRead(ENCODER_PIN_A);
  
  if (encoderPinAValue != lastEncoderPinAValue) { // Knob has rotated
    washingHands = true;
    
    // Log time handwashing started
    if (handwashingStarted == 0) {
      handwashingStarted = millis();
    }
    
//    // Determine direction by grabbing pin B value
//    if (digitalRead(ENCODER_PIN_B) != aVal) { // Pin A changed first, so we're rotating clockwise
//      encoderPosCount++;
//      bCW = true;
//    } else { // Pin B changed first, so we're rotating counterclockwise
//      bCW = false;
//      encoderPosCount--;
//    }

//    // Print data for diagnostics
//    Serial.print("Rotated: ");
//    if (bCW) {
//      Serial.println("Clockwise");
//    } else {
//      Serial.println("Counterclockwise");
//    }
//    Serial.print("Encoder Position: ");
//    Serial.println(encoderPosCount);
  }
  lastEncoderPinAValue = encoderPinAValue; // Update last value for pin A

  if (washingHands) {
    if (secondsElapsed < (millis() - handwashingStarted) / 1000) { // Second counter is off
      secondsElapsed = (millis() - handwashingStarted) / 1000; // Update second counter
      
      // Print data for diagnostics
      Serial.print(secondsElapsed);
      Serial.println(" seconds elapsed");
    }

    // Reset timer after 20 seconds elapsed
    if (secondsElapsed == 20) {
      washingHands = false;
      secondsElapsed = 0;
      handwashingStarted = 0;

      leds[0] = CHSV(0, 0, 0);
      leds[1] = CHSV(0, 0, 0);
      leds[2] = CHSV(0, 0, 0);
    }

    if (secondsElapsed == 1) {
      leds[0] = CRGB::White;
    } else if (secondsElapsed == 2) {
      leds[1] = CRGB::White;
    } else if (secondsElapsed == 3) {
      leds[2] = CRGB::White;
    }
  }

  FastLED.show();
  FastLED.delay(1000 / LED_FPS);
}
