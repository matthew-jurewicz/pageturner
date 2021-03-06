#define SENDER  1

int targetFreq[2] = {100, 200};

#ifdef SENDER
#include <I2Cdev.h>
#include <MPU6050.h>

MPU6050 gyro;
int rot1[3], rot2[3], ledPin = 9;
float mag1, thresh = PI / 4.0;

void setup() {
  pinMode(ledPin, OUTPUT);
  
  gyro.initialize();
  gyro.getRotation(&rot1[0], &rot1[1], &rot1[2]);
  mag1 = mag3(rot1);
}

void loop() {
  gyro.getRotation(&rot2[0], &rot2[1], &rot2[2]);

  int prod = dot3(rot1, rot2);
  float angle = acos((float)prod / (float)(mag1 * mag3(rot2)));
  if(angle >= thresh) {
    if(prod > 0) {
      //tone(ledPin, targetFreq[0]);
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else {
      //tone(ledPin, targetFreq[1]);
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
  else {
    //noTone(ledPin);
    digitalWrite(LED_BUILTIN, LOW);
  }
}
#else
#include <Servo.h>

Servo ds3218;
int servoPin = 9, adjRest = 94, irPin = 3;
int targetMs[2] = {1000 / targetFreq[0], 1000 / targetFreq[1]}; //1000 ms / hz
unsigned long pulse[2] = {-1, -1};

void setup() {
  Serial.begin(9600);
  ds3218.attach(servoPin);
  ds3218.write(adjRest);
}

void loop() {
  if(digitalRead(irPin) == HIGH) {
    pulse[0] = millis();
    if(pulse[1] != -1) {
      int elapsed = pulse[0] - pulse[1];
      if(elapsed == targetMs[0]) {
        ds3219.write(0);
      }
      else if(elapsed == targetMs[1]) {
        ds3219.write(180);
      }
      else {
        ds3219.write(adjRest);
      }
    }
    pulse[1] = pulse[0];
  }
}
#endif

float mag3(int *vec) {
  return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

int dot3(int *v1, int *v2) {
  int sum = 0;
  for(int i = 0; i < 3; i++)
    sum += v1[i] * v2[i];

  return sum;
}

