#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(32, 1);
const int pingPin = 3;
long cm;
long finalUltraSoundValue = 0;

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
//  if(cm < 30) {
//    sensorResponse();
//  }
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void loop() {
runmotorAndReadSensor(100);
delay(1000);

}

void runmotorAndReadSensor(int input) { 
  int i = 0;
  int steps = 0;
  for(int i; i < input; i++) {
    steps = steps + i;
  }

  do {
     myMotor2->step(input, FORWARD, DOUBLE);
     ultraSoundResponse();
  }
  while (steps >= input);
    Serial.println("stopped");
    myMotor2->release();
    mappingStepperData(finalUltraSoundValue);
} 

void mappingStepperData(int sensorValue) {
  Serial.println(cm);
  // ultrasound comes in cm but we turn into how far stepper moves between 0 and 2000
  int mappedStepper = map(cm, 0, 315, 0, 2000);
  myMotor->step(mappedStepper, FORWARD, DOUBLE);
  Serial.println("motor1moving");
}



