/*
Adafruit Arduino - Lesson 16. Stepper
*/

#include <Unistep2.h>
#include "SevSeg.h"
SevSeg sevseg; //Initiate a seven segment controller object

/** PINS **/
const int buttonPin = 13;

int motorIn1Pin = 5;
int motorIn2Pin = 4;
int motorIn3Pin = 3;
int motorIn4Pin = 2;
Unistep2 stepper(motorIn1Pin, motorIn2Pin, motorIn3Pin, motorIn4Pin, 4096, 5000);
int motor_delay = 250;

unsigned long curr_time = 0;
int buttonState = 0;  // variable for reading the pushbutton status

const int pResistorLeft = A0; // Photoresistor at Arduino analog pin A0
const int pResistorRight = A1;
int valueLeft;				  // Store value from photoresistor (0-1023)
int valueRight;
int startingLeft;
int startingRight;
bool calibrated = false;

int numDigits = 4;
int score = 0;
int score_time = 0;
int poll_time = 0;

void setup()
{
  pinMode(buttonPin, INPUT);

  pinMode(motorIn1Pin, OUTPUT);
  pinMode(motorIn2Pin, OUTPUT);
  pinMode(motorIn3Pin, OUTPUT);
  pinMode(motorIn4Pin, OUTPUT);
  
  stepper.run();

  pinMode(pResistorLeft, INPUT);
  pinMode(pResistorRight, INPUT);
  
  byte numDigits = 4;
  byte digitPins[] = {A2, A3, A4, A5};
  byte segmentPins[] = {6, 7, 8, 9, 10, 12, 11};
  bool resistorsOnSegments = 0; 
  // variable above indicates that 4 resistors were placed on the digit pins.
  // set variable to 1 if you want to use 8 resistors on the segment pins.
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegments);
  sevseg.setBrightness(90);

  Serial.begin(9600); 
}


void loop()
{
  sevseg.setNumber(score);
  sevseg.refreshDisplay();
  curr_time = millis();
  stepper.run();

  if (!calibrated && curr_time > 200) {
    startingLeft = analogRead(pResistorLeft);
    startingRight = analogRead(pResistorRight);
    calibrated = true;
    Serial.println("sLeft:");
    Serial.println(startingLeft);
    Serial.println("sRight:");
    Serial.println(startingRight);
    stepper.move(9999);
  }
  
  stepper.move(100);
  valueLeft = analogRead(pResistorLeft);
  valueRight = analogRead(pResistorRight);
    if (curr_time > score_time + 400) {
      
      if (valueLeft < startingLeft - 20 || valueRight < startingRight - 20){
      score += 10;
      score_time = curr_time;
    }
  }


  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    Serial.println("pressed");
    score = 0;
    score_time = 0;
    delay(1000);
  }
}