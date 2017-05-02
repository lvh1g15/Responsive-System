#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);
Adafruit_StepperMotor *myMotor2 = AFMS.getStepper(200, 1);

int counter = 0;
const int pingPin = 3;
long cm;
long db;
long lux;
long finalUltraSoundValue = 0;
int sensorPin = A0;
int ldrpin = A2;
long arraySensorValues[] = {cm, db, lux};

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Stepper test!");

//  AFMS.begin();  // create with the default frequency 1.6KHz
  AFMS.begin();  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(100);  // 10 rpm   
  myMotor2->setSpeed(100);
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
//  for(int i = 0l; i < sizeof(arraySensorValues)) {
    runmotorAndReaddata(120);
//  }
}

void microphone() {
  db = analogRead(sensorPin);
  arraySensorValues[1] = db;
  Serial.println(db);
}

void ldr() {
  lux = analogRead(ldrpin);
  arraySensorValues[2] = lux;
}

void runmotorAndReaddata(int input) { 
  int x = 1;
  for(int m=0; m > -1; m = m + x) {
    Serial.println(m);
    
    if(m==2) x = -1;
    if(m==0) x = 1;
    if(x > 0) {
      myMotor2->step(input, FORWARD, DOUBLE);
      ultraSoundResponse();
      delay(100);
      microphone();
      delay(100);
      ldr();
      delay(100);
      myMotor2->release();
      mappingStepperData(arraySensorValues[m], m);
    }
    if(x < 0) {
      myMotor2->step(input, BACKWARD, DOUBLE);
      ultraSoundResponse();
      delay(100);
      microphone();
      delay(100);
      ldr();
      delay(100);
      myMotor2->release();
      mappingStepperData(arraySensorValues[m], m);
    }
  }
}

void mappingStepperData(long sensorValue, int arrayIndex) {
// ultrasound comes in cm but we turn into how far stepper moves between 0 and 2000

if(arrayIndex == 0) {
    Serial.println(" ULTRASOUND... ");
    Serial.println(arraySensorValues[0]);
    long mappedStepper = map(sensorValue, 0, 315, 0, 500);
    myMotor->step(mappedStepper, FORWARD, DOUBLE);
    myMotor->release();
} else if(arrayIndex == 1) {
    Serial.println(" AUDIO...");
    Serial.println(arraySensorValues[1]);
    long mappedAudio = map(sensorValue, 250, 800, 0, 500);
    myMotor->step(mappedAudio, FORWARD, DOUBLE);
    myMotor->release();
} else {
    Serial.println(" LIGHT... ");
    Serial.println(arraySensorValues[2]);
    long mappedLuxreading = map(sensorValue, 100, 1100, 500, 0);
    myMotor->step(mappedLuxreading, FORWARD, DOUBLE);
    myMotor->release();
}


//  if(sensorValue > 315) {
//    long mappedAudio = map(sensorValue, 450, 800, 0, 1300);
//    myMotorstep(mappedAudio, FORWARD, DOUBLE);
//    myMotor->release();
//    Serial.print(" AUDIO...");
//    Serial.print(" cm: ");
//    Serial.print(arraySensorValues[0]);
//    Serial.print(" db: ");
//    Serial.print(arraySensorValues[1]);
//    Serial.println("");
//  }
//  else {
//    long mappedStepper = map(sensorValue, 0, 315, 0, 1300);
//    myMotor->step(mappedStepper, FORWARD, DOUBLE);
//    myMotor->release();
//    Serial.print(" ULTRASOUND... ");
//    Serial.print(" cm: ");
//    Serial.print(arraySensorValues[0]);
//    Serial.print(" db: ");
//    Serial.print(arraySensorValues[1]);
//    Serial.println("");
//  }
}



