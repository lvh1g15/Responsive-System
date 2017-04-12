#include <Adafruit_MotorShield.h>
#include <Stepper.h>
// running two steppers 

#define IN1  4
#define IN2  5
#define IN3  6
#define IN4  7
#define IN5  8
#define IN6  9
#define IN7  10
#define IN8  11

#define STEPS_PER_ROTATION 4096
int sensorPin = A0;
int sensorValue = 0;
const int pingPin = 7;
long cm;

void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  pinMode(IN5, OUTPUT);
  pinMode(IN6, OUTPUT);
  pinMode(IN7, OUTPUT);
  pinMode(IN8, OUTPUT);
  delay(100);
}

void loop()
{
//  ultraSoundResponse();
//    rotate(10);
//    delay(10);
//    minimaxCalc(100);
      rotate(-10);
      
    delay(50);
    
}

void ultraSoundResponse() {
  long duration;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  if(cm < 30) {
    sensorResponse();
  }
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void rotate(float rotations) {
    rotate_steps(rotations * STEPS_PER_ROTATION);
}

void sensorResponse() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  if(sensorValue > 500) {
    rotate_steps(5);
  } else {
    rotate_steps(0);
  }
}

int phase = 0;
byte phases[] = { 1, 3, 2, 6, 4, 12, 8, 9 };

void rotate_steps(int steps)
{
  int dir = (steps > 0) - (steps < 0);
  steps *= dir;
  long laststep;
  for (int i = 0; i < steps;) {
    
    long now = micros();
    if (now - laststep < 1000) continue;
    laststep = now;
    stepper_writepins(phases[phase]);
    phase = (8 + phase + dir) % 8;
    i++;
  }
  stepper_writepins(0);
}

void stepper_writepins(int bitmap) {
    digitalWrite(IN1, bitmap & 8 ? HIGH : LOW);
    digitalWrite(IN2, bitmap & 4 ? HIGH : LOW);
    digitalWrite(IN3, bitmap & 2 ? HIGH : LOW);
    digitalWrite(IN4, bitmap & 1 ? HIGH : LOW);

    digitalWrite(IN5, bitmap & 8 ? HIGH : LOW);
    digitalWrite(IN6, bitmap & 4 ? HIGH : LOW);
    digitalWrite(IN7, bitmap & 2 ? HIGH : LOW);
    digitalWrite(IN8, bitmap & 1 ? HIGH : LOW);
}


/* these next lines are to find the max and min values from the mircophones anolog output*/
// for this function only call it once the stepper has started moving from the previous sensor. When the smaller stepper has 
// stopped moving then that is when you should take the last sounds reading and use this as the larger stepper value.


void minimaxCalc(int sampleWindow)
{

   unsigned int sample;
    
   unsigned long startMillis= millis();  // Start of sample window
   unsigned int peakToPeak = 0;   // peak-to-peak level
 
   unsigned int signalMax = 0;
   unsigned int signalMin = 1024;
 
   // collect data for 50 mS
   while (millis() - startMillis < sampleWindow)
   {
      sample = analogRead(0);
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
   double volts = (peakToPeak * 5.0) / 1024;  // convert to volts
 
   Serial.println(volts);
}

