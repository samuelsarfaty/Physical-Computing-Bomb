#include "SevSeg.h"

SevSeg sevseg; //Instantiate a seven segment controller object

static unsigned long millisPerMinute = 60000;
unsigned long secondsTimer = millisPerMinute;

unsigned long previousTime = 0;
int minutesCounter = 0;

boolean keepCounting = true;
boolean disconnected = false; //TODO Timer gets stopped when true. Should be changed when disconnection is detected

int secondToDisplay = 60; //TODO Do not change.
int totalMinutes = 9;

 void setup() {

  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);

  byte numDigits = 4;
  byte digitPins[] = {13, 12, 11, 10};
  byte segmentPins[] = {2, 3, 4, 5, 6, 7, 8, 9};

  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(90);
  Serial.begin(9600);
}

void loop() {

  while (disconnected == false){

  if (digitalRead(14) == 1){
    disconnected = true;
  }

  if (millisPerMinute - (millis() - previousTime) <= secondsTimer && keepCounting == true){
    secondToDisplay--;
    secondsTimer -= 1000;
    DisplayNumber();
    //Serial.println(secondsTimer);
  }

  if (secondsTimer <= 0 && totalMinutes > 0){
    minutesCounter ++;
    previousTime = millisPerMinute * minutesCounter;
    secondsTimer = millisPerMinute;
    secondToDisplay = 60;
    totalMinutes --;
  } else if (secondsTimer <= 0 && totalMinutes <= 0){
    keepCounting = false;
    sevseg.setNumber(8888, 2);
    Serial.println("Done");
  }

  sevseg.refreshDisplay();
  }

  DisplayNumber();
  sevseg.refreshDisplay();

}

void DisplayNumber(){
  int minuteToDisplay = totalMinutes * 100;
  int currentTime = minuteToDisplay + secondToDisplay;
  sevseg.setNumber(currentTime, 2);
}
