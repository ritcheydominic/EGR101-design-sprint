/*
 * Arduino Code for Design Sprint
 * Written by: Trey Howell and Dominic Ritchey
 * 
 * This program is based on the Arduino sketch example provided at https://www.epitran.it/ebayDrive/datasheet/25.pdf
 */

int pinA = 3; // Connected to CLK on KY-040
int pinB = 4; // Connected to DT on KY-040
//int encoderPosCount = 0;
int pinALast;
int aVal;
//boolean bCW;

unsigned long handwashingStarted = 0;
int secondsElapsed = 0;
boolean washingHands;

void setup() {
  // Set up pins
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  
  // Read pin A (whatever state it's in will reflect the last position)
  pinALast = digitalRead(pinA);

  // Set up logging
  Serial.begin(9600);
}

void loop() {
  // Grab pin A value
  aVal = digitalRead(pinA);
  
  if (aVal != pinALast) { // Knob has rotated
    washingHands = true;
    
    // Log time handwashing started
    if (handwashingStarted == 0) {
      handwashingStarted = millis();
    }
    
//    // Determine direction by grabbing pin B value
//    if (digitalRead(pinB) != aVal) { // Pin A changed first, so we're rotating clockwise
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
  pinALast = aVal; // Update last value for pin A

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
    }
  }  
}
