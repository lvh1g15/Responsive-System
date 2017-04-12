#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(32, 1);


const int pingPin = 3;
long cm;
long db;
long finalUltraSoundValue = 0;
int sensorPin = A0;
long arraySensorValues[1] = {};

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

//  AFMS.begin();  // create with the default frequency 1.6KHz
  AFMS.begin();  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(100);  // 10 rpm   
  myMotor2->setSpeed(200);
}

void ultraSoundResponse() {
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(1);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(1);
  
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  arraySensorValues[0] = cm;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void loop() {
runmotorAndReadSensor(100);
delay(1000);

}

void microphone() {
  db = analogRead(sensorPin);
  arraySensorValues[1] = db;
}

void runmotorAndReadSensor(int input) { 
  int i = 0;
  int m = 0;
    for(int i; i < input; i++) {
      do {
         myMotor2->step(input, FORWARD, DOUBLE);
         ultraSoundResponse();
         microphone();
      }
      while (i >= input);
        myMotor2->release();
        mappingStepperData(arraySensorValues[m]);
     } 
}

void mappingStepperData(int sensorValue) {
  // ultrasound comes in cm but we turn into how far stepper moves between 0 and 2000
  if(sensorValue > 315) {
    int mappedAudio = map(sensorValue, 450, 800, 0, 2000);
    myMotor->step(mappedAudio, FORWARD, DOUBLE);
  }
  else {
    int mappedStepper = map(sensorValue, 0, 315, 0, 2000);
    myMotor->step(mappedStepper, FORWARD, DOUBLE);
    Serial.print(" cm: ");
    Serial.print(cm);
    Serial.print(", db: ");
    Serial.print(db);
    Serial.println("");
  }
}



