#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "../lib/server.cpp"

#define leftFrontIR 36
#define rightFrontIR 39
#define rightBackIR 5
#define leftBackIR 27
#define trigPin 21
#define USEcho 34

#define sensorStatusPin 12
#define SOUND_SPEED 0.034

// Motor Pins
int motorPins[][3] = {
    {32, 33, 16}, // linksvoor
    {25, 26, 15}, // linksachter
    {23, 22, 4},  // rechtsvoor
    {19, 18, 17}  // rechtsachter
};

long duration;
float distanceCm;
int state = 1;

unsigned long lastSensorStatusChangeTime = 0;

float distanceDetection()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(USEcho, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

void setup()
{
  Serial.begin(9600);
  pinMode(ACMStatusPin, OUTPUT);
  pinMode(sensorStatusPin, OUTPUT);
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 3; ++j)
    {
      pinMode(motorPins[i][j], OUTPUT);
    }
  }
  pinMode(leftFrontIR, INPUT);
  pinMode(rightFrontIR, INPUT);
  pinMode(rightBackIR, INPUT);
  pinMode(leftBackIR, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(USEcho, INPUT);
  setupServer();
}

void move(int mode, int time)
{
  static unsigned long startTime = millis();

  if (time != 0 && millis() - startTime >= time)
  {
    return;
  }

  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(motorPins[i][0], mode & (1 << i) ? HIGH : LOW);
    digitalWrite(motorPins[i][1], mode & (1 << i) ? LOW : HIGH);
    analogWrite(motorPins[i][2], 125);
  }
}

void moveForward(int time) { move(0b1010, time); }
void moveBackward(int time) { move(0b0101, time); }
void moveLeft(int time) { move(0b0011, time); }
void moveRight(int time) { move(0b1100, time); }
void turnLeft(int time) { move(0b1001, time); }
void turnRight(int time) { move(0b0110, time); }

void stop()
{
  for (int i = 0; i < 4; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      digitalWrite(motorPins[i][j], LOW);
    }
    analogWrite(motorPins[i][2], LOW);
  }
}

void loop()
{
  float distance = distanceDetection();

  if (digitalRead(ACMStatusPin))
  {
    digitalWrite(sensorStatusPin, HIGH);
    if (digitalRead(rightFrontIR))
    {
      stop();
      turnLeft(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    else if (digitalRead(leftFrontIR))
    {
      stop();
      turnRight(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    else if (digitalRead(rightBackIR))
    {
      stop();
      turnLeft(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    else if (digitalRead(leftBackIR))
    {
      stop();
      turnRight(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    else if (distance <= 20)
    {
      stop();
      turnLeft(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    else
    {
      moveForward(0);
      digitalWrite(sensorStatusPin, LOW);
    }
  }
  else
  {
    stop();
    digitalWrite(sensorStatusPin, LOW);
  }
}
