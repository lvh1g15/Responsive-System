#define IN1  8
#define IN2  9
#define IN3  10
#define IN4  11
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
  delay(100);
}

void loop()
{
  ultraSoundResponse();
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
}
