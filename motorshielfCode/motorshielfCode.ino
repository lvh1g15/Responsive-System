#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(32, 1);
const int pingPin = 3;
long cm;
long finalUltraSoundValue;

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
//  myMotor->step(1000, FORWARD, DOUBLE); 
//  myMotor2->step(100, FORWARD, DOUBLE);
//  delay(100);
//  myMotor2->step(100, BACKWARD, DOUBLE);
//  delay(100);
runmotorAndReadSensor(100);
delay(1000);
//    
//    ultraSoundResponse();
//
//  Serial.println("Double coil steps");
//  myMotor->step(100, FORWARD, DOUBLE); 
//  myMotor->step(100, BACKWARD, DOUBLE);
//  
//  Serial.println("Interleave coil steps");
//  myMotor->step(100, FORWARD, INTERLEAVE); 
//  myMotor->step(100, BACKWARD, INTERLEAVE); 
//  
//  Serial.println("Microstep steps");
//  myMotor->step(50, FORWARD, MICROSTEP); 
//  myMotor->step(50, BACKWARD, MICROSTEP);
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
     finalUltraSoundValue = Serial.println(cm);
  }
  while (steps >= input);
    Serial.println("stopped");
    myMotor2->release();
} 

void mappingStepperData(int sensorValue) {

  int mappedStepper = map(sensorValue, 0, );
  
}



