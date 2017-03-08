#include <Servo.h>

Servo servoPin;

int soundSensor = A2;
int pos = 0;
int range[] = {};

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
    servoPin.attach(9);
}

void loop() {
  
  int soundValue = analogRead(soundSensor);
  delay(100);
  int minVal = range[0];
  int maxVal = range[1];
  String newMin = String(minVal);
  String newMax = String(maxVal);
  
  if(minVal > value) {
    int value = range[0];
    String newMax = String(value);
  } else if(minVal < value && maxVal < value) {
    maxVal = value;
    String newMax = String(maxVal);
  } 
  
  Serial.println(newMin);
  Serial.println(newMax);
}



void sweep(int number) {
   for(pos=0;pos < number; pos++){
      servoPin.write(pos);
      delay(1000);
   }
  for(pos=number;pos>=1;pos--){
      servoPin.write(pos);
      delay(100);
  }
    
}
