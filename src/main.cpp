#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "../lib/server.cpp"

#define leftFrontIR 36
#define rightFrontIR 39
#define rightBackIR 5
#define leftBackIR 27
#define trigPin 21
#define echoForward 34
#define echoDown 13

#define sensorStatusPin 12
#define SOUND_SPEED 0.034

// Motor Pins
int motorPins[][3] = {
    {32, 33, 16}, // left front
    {25, 26, 15}, // left rear
    {23, 22, 4},  // right front
    {19, 18, 17}  // right rear
};

long duration;
float distanceCm;
unsigned long leftFrontIRTriggeredTime = 0;
unsigned long rightFrontIRTriggeredTime = 0;
int leftFrontIRCounter = 0;
int rightFrontIRCounter = 0;
const unsigned long triggerInterval = 5000; // 5 seconds in milliseconds
unsigned long lastSensorStatusChangeTime = 0;

float distanceDetection(uint8_t pin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(pin, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

bool isStuckInCorner()
{
  unsigned long currentTime = millis();

  if (digitalRead(leftFrontIR))
  {
    if (currentTime - leftFrontIRTriggeredTime > triggerInterval)
    {
      leftFrontIRCounter = 0;
    }
    leftFrontIRTriggeredTime = currentTime;
    leftFrontIRCounter++;
  }

  if (digitalRead(rightFrontIR))
  {
    if (currentTime - rightFrontIRTriggeredTime > triggerInterval)
    {
      rightFrontIRCounter = 0;
    }
    rightFrontIRTriggeredTime = currentTime;
    rightFrontIRCounter++;
  }

  if (leftFrontIRCounter >= 3 && rightFrontIRCounter >= 3)
  {
    // Reset counters to avoid repeated corner detection
    leftFrontIRCounter = 0;
    rightFrontIRCounter = 0;
    return true;
  }
  return false;
}

void setup()
{
  Serial.begin(9600);
  static unsigned long startTimer = millis();
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
  pinMode(echoForward, INPUT);
  pinMode(echoDown, INPUT);
  uint8_t setupHeight = distanceDetection(echoDown);
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
    analogWrite(motorPins[i][2], 150);
  }
}

void moveForward(int time) { move(0b1010, time); }
void moveBackward(int time) { move(0b0101, time); }
void moveLeft(int time) { move(0b0011, time); }
void moveRight(int time) { move(0b1100, time); }
void turnLeft(int time) { move(0b1001, time); }
void turnRight(int time) { move(0b0110, time); }
void rampCalibration(int time) { move(0b1101, time); }

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

void handleObstacle()
{
  if (isStuckInCorner())
  {
    stop();
    moveBackward(1000);
    turnLeft(1000);
  }
  else
  {
    if (digitalRead(rightFrontIR))
    {
      moveBackward(0);
      delay(500);
      turnLeft(0);
      delay(200);
      digitalWrite(sensorStatusPin, HIGH);
    }
    if (digitalRead(leftFrontIR))
    {
      moveBackward(0);
      delay(500);
      turnRight(0);
      delay(200);
      digitalWrite(sensorStatusPin, HIGH);
    }
    if (digitalRead(rightBackIR))
    {
      moveForward(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    if (digitalRead(leftBackIR))
    {
      moveForward(0);
      delay(500);
      digitalWrite(sensorStatusPin, HIGH);
    }
    if (distanceDetection(echoDown) >= 5)
    {
      stop();
      delay(500);
      moveBackward(0);
      delay(500);
      turnRight(0);
      delay(1000);
    }
    if (distanceDetection(echoForward) <= 15)
    {
      moveRight(0);
      delay(500);
    }
  }
}

void loop()
{
  bool ACMStatus = digitalRead(ACMStatusPin);
  digitalWrite(sensorStatusPin, ACMStatus ? HIGH : LOW);
  if (ACMStatus)
  {
    float distanceForward = distanceDetection(echoForward);
    float distanceDown = distanceDetection(echoDown);

    if (digitalRead(rightFrontIR) || digitalRead(leftFrontIR) || digitalRead(rightBackIR) || digitalRead(leftBackIR) || distanceForward <= 15 || distanceDown >= 5)
    {
      handleObstacle();
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
