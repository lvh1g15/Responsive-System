#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(32, 1);

int counter = 0;
const int pingPin = 3;
long cm;
long db;
long finalUltraSoundValue = 0;
int sensorPin = A0;
long arraySensorValues[] = {cm, db};

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
    runmotorAndReaddata(100);
}

void microphone() {
  db = analogRead(sensorPin);
  arraySensorValues[1] = db;
}

void runmotorAndReaddata(int input) { 
  for(int m=0; m< 2; m++ ) {
    myMotor2->step(input, FORWARD, DOUBLE);
    ultraSoundResponse();
    delay(100);
    microphone();
    delay(100);
    myMotor2->release();
    mappingStepperData(arraySensorValues[m]);
  }
}

void mappingStepperData(long sensorValue) {
  // ultrasound comes in cm but we turn into how far stepper moves between 0 and 2000
  if(sensorValue > 315) {
    long mappedAudio = map(sensorValue, 450, 800, 0, 1300);
    myMotor->step(mappedAudio, FORWARD, DOUBLE);
    myMotor->release();
    Serial.print(" AUDIO...");
    Serial.print(" cm: ");
    Serial.print(arraySensorValues[0]);
    Serial.print(" db: ");
    Serial.print(arraySensorValues[1]);
    Serial.println("");
  }
  else {
    long mappedStepper = map(sensorValue, 0, 315, 0, 1300);
    myMotor->step(mappedStepper, FORWARD, DOUBLE);
    myMotor->release();
    Serial.print(" ULTRASOUND... ");
    Serial.print(" cm: ");
    Serial.print(arraySensorValues[0]);
    Serial.print(" db: ");
    Serial.print(arraySensorValues[1]);
    Serial.println("");
  }
}



