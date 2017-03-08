#include <Servo.h>
Servo servoPin;

boolean sensorTakingValues = false;

int soundSensor = A2;
const int pingPinUltra = 7;
int pos = 0;
long cm;
int counter = 0;

int sensorValueArray[] = {};
int lastValueInUltraArray = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoPin.attach(9);
  delay(10);

  //maybe place ultrasoundsensor code in here so we are reading values from
  //setup so we can initialise the array will values

}

void loop() {

  servo();

}


void ultraSoundResponse() {
  sensorTakingValues = true;
  long duration;
  
  pinMode(pingPinUltra, OUTPUT);
  digitalWrite(pingPinUltra, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPinUltra, HIGH);
  delayMicroseconds(5);

  pinMode(pingPinUltra, INPUT);
  duration = pulseIn(pingPinUltra, HIGH);
  cm = microsecondsToCentimeters(duration);
  sensorValueArray[cm];
  Serial.println(cm);
  delay(10);
}

void readingUltraResponse() {
  int i = 0;
    for(i = 0; sensorValueArray[i] != '\0'; i++) {
    int lastValueInUltraArray = sensorValueArray[--i];
    }
}

void fakeStepper() {
  int i =0;
  for(i=0;i<10000; i++) {
  counter = counter + i;
  if (counter = lastValueInUltraArray) {
    servo();
    counter = 0;
    }
  }
}

void servo() {
 
  for(pos=0; pos < 180; pos++) {
    servoPin.write(pos);
    if (pos = 30) {
      servoPin.write(30);
      fakeStepper();
       
    }
  }
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
