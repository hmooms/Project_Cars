#include <WebServer.h>
#include <ESPmDNS.h>
#include <Arduino.h>

#include "../lib/Analog.h"
#include "../lib/MotorDriver.h"

#include <WiFi.h>

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const int trigPin = 21;
const int echoFront = 34;
const int echoBack = 35;
const int echoRight = 14;
const int echoLeft = 12;
const int leftFrontIR = 36;
const int rightFrontIR = 39;
const int rightBackIR = 5;
const int leftBackIR = 27;

// Motor B linksvoor
const int linksVoor1 = 32;   // Pin
const int linksVoor2 = 33;   // Pin
const int linksVoorPWM = 16; // Pin

// Motor C linksachter
const int linksAchter1 = 25;   // Pin
const int linksAchter2 = 26;   // Pin
const int linksAchterPWM = 15; // Pin

// rechtsvoor
const int rechtsVoor1 = 23;  // Pin
const int rechtsVoor2 = 22;  // Pin
const int rechtsVoorPWM = 4; // Pin

// Motor D rechtsachter
const int rechtsAchter1 = 18;   // Pin
const int rechtsAchter2 = 19;   // Pin
const int rechtsAchterPWM = 17; // Pin

long duration;
float distanceCm;
int state = 1;

float distanceDetection(int pin)
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(pin, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

void setup()
{
  // Set pins as outputs
  pinMode(rechtsVoor1, OUTPUT);
  pinMode(rechtsVoor2, OUTPUT);
  pinMode(rechtsVoorPWM, OUTPUT);
  pinMode(linksVoor1, OUTPUT);
  pinMode(linksVoor2, OUTPUT);
  pinMode(linksVoorPWM, OUTPUT);
  pinMode(linksAchter1, OUTPUT);
  pinMode(linksAchter2, OUTPUT);
  pinMode(linksAchterPWM, OUTPUT);
  pinMode(rechtsAchter1, OUTPUT);
  pinMode(rechtsAchter2, OUTPUT);
  pinMode(rechtsAchterPWM, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoFront, INPUT);
  pinMode(echoBack, INPUT);
  pinMode(leftFrontIR, INPUT);
  pinMode(rightFrontIR, INPUT);
  Serial.begin(9600);
}
void stop()
{
  // Move forwards
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, LOW);
  digitalWrite(linksVoorPWM, LOW);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, LOW);
  digitalWrite(linksAchterPWM, LOW);
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, LOW);
  digitalWrite(rechtsVoorPWM, LOW);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, LOW);
  digitalWrite(rechtsAchterPWM, LOW); // set LED brightness as PWM signal
}
void moveForward(int x)
{
  if (x == 0)
  {
    digitalWrite(linksVoor1, HIGH);
    digitalWrite(linksVoor2, LOW);
    digitalWrite(linksAchter2, HIGH);
    digitalWrite(linksAchter1, LOW);
    digitalWrite(rechtsVoor1, HIGH);
    digitalWrite(rechtsVoor2, LOW);
    digitalWrite(rechtsAchter2, HIGH);
    digitalWrite(rechtsAchter1, LOW);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(linksVoor1, HIGH);
      digitalWrite(linksVoor2, LOW);
      digitalWrite(linksAchter2, HIGH);
      digitalWrite(linksAchter1, LOW);
      digitalWrite(rechtsVoor1, HIGH);
      digitalWrite(rechtsVoor2, LOW);
      digitalWrite(rechtsAchter2, HIGH);
      digitalWrite(rechtsAchter1, LOW);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}
void moveBackward(int x)
{
  if (x == 0)
  {
    digitalWrite(linksVoor1, LOW);
    digitalWrite(linksVoor2, HIGH);
    digitalWrite(linksAchter2, LOW);
    digitalWrite(linksAchter1, HIGH);
    digitalWrite(rechtsVoor1, LOW);
    digitalWrite(rechtsVoor2, HIGH);
    digitalWrite(rechtsAchter2, LOW);
    digitalWrite(rechtsAchter1, HIGH);
    analogWrite(linksVoorPWM, 100);
    analogWrite(linksAchterPWM, 100);
    analogWrite(rechtsVoorPWM, 100);
    analogWrite(rechtsAchterPWM, 100);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(linksVoor1, LOW);
      digitalWrite(linksVoor2, HIGH);
      digitalWrite(linksAchter2, LOW);
      digitalWrite(linksAchter1, HIGH);
      digitalWrite(rechtsVoor1, LOW);
      digitalWrite(rechtsVoor2, HIGH);
      digitalWrite(rechtsAchter2, LOW);
      digitalWrite(rechtsAchter1, HIGH);
      analogWrite(linksVoorPWM, 100);
      analogWrite(linksAchterPWM, 100);
      analogWrite(rechtsVoorPWM, 100);
      analogWrite(rechtsAchterPWM, 100);
    }
  }
}
void moveDiagonalRight(int x)
{
  if (x == 0)
  {
    digitalWrite(linksVoor1, HIGH);
    digitalWrite(linksVoor2, LOW);
    digitalWrite(linksAchter2, LOW);
    digitalWrite(linksAchter1, LOW);
    digitalWrite(rechtsVoor1, LOW);
    digitalWrite(rechtsVoor2, LOW);
    digitalWrite(rechtsAchter2, HIGH);
    digitalWrite(rechtsAchter1, LOW);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 0);
    analogWrite(rechtsVoorPWM, 0);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(linksVoor1, HIGH);
      digitalWrite(linksVoor2, LOW);
      digitalWrite(linksAchter2, LOW);
      digitalWrite(linksAchter1, LOW);
      digitalWrite(rechtsVoor1, LOW);
      digitalWrite(rechtsVoor2, LOW);
      digitalWrite(rechtsAchter2, HIGH);
      digitalWrite(rechtsAchter1, LOW);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 0);
      analogWrite(rechtsVoorPWM, 0);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}
void moveDiagonalLeft(int x)
{
  if (x == 0)
  {
    digitalWrite(linksVoor1, LOW);
    digitalWrite(linksVoor2, LOW);
    digitalWrite(linksAchter2, HIGH);
    digitalWrite(linksAchter1, LOW);
    digitalWrite(rechtsVoor1, HIGH);
    digitalWrite(rechtsVoor2, LOW);
    digitalWrite(rechtsAchter2, LOW);
    digitalWrite(rechtsAchter1, LOW);
    analogWrite(linksVoorPWM, 0);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 0);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(linksVoor1, LOW);
      digitalWrite(linksVoor2, LOW);
      digitalWrite(linksAchter2, HIGH);
      digitalWrite(linksAchter1, LOW);
      digitalWrite(rechtsVoor1, HIGH);
      digitalWrite(rechtsVoor2, LOW);
      digitalWrite(rechtsAchter2, LOW);
      digitalWrite(rechtsAchter1, LOW);
      analogWrite(linksVoorPWM, 0);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 0);
    }
  }
}
void moveLeft(int x)
{
  if (x == 0)
  {
    digitalWrite(rechtsVoor1, HIGH);
    digitalWrite(rechtsVoor2, LOW);
    digitalWrite(linksVoor1, LOW);
    digitalWrite(linksVoor2, HIGH);
    digitalWrite(linksAchter2, HIGH);
    digitalWrite(linksAchter1, LOW);
    digitalWrite(rechtsAchter2, LOW);
    digitalWrite(rechtsAchter1, HIGH);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(rechtsVoor1, HIGH);
      digitalWrite(rechtsVoor2, LOW);
      digitalWrite(linksVoor1, LOW);
      digitalWrite(linksVoor2, HIGH);
      digitalWrite(linksAchter2, HIGH);
      digitalWrite(linksAchter1, LOW);
      digitalWrite(rechtsAchter2, LOW);
      digitalWrite(rechtsAchter1, HIGH);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}
void moveRight(int x)
{
  if (x == 0)
  {
    digitalWrite(rechtsVoor1, LOW);
    digitalWrite(rechtsVoor2, HIGH);
    digitalWrite(linksVoor1, HIGH);
    digitalWrite(linksVoor2, LOW);
    digitalWrite(linksAchter2, LOW);
    digitalWrite(linksAchter1, HIGH);
    digitalWrite(rechtsAchter2, HIGH);
    digitalWrite(rechtsAchter1, LOW);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(rechtsVoor1, LOW);
      digitalWrite(rechtsVoor2, HIGH);
      digitalWrite(linksVoor1, HIGH);
      digitalWrite(linksVoor2, LOW);
      digitalWrite(linksAchter2, LOW);
      digitalWrite(linksAchter1, HIGH);
      digitalWrite(rechtsAchter2, HIGH);
      digitalWrite(rechtsAchter1, LOW);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}
void turnLeft(int x)
{
  if (x == 0)
  {
    digitalWrite(rechtsVoor1, HIGH);
    digitalWrite(rechtsVoor2, LOW);
    digitalWrite(linksVoor1, LOW);
    digitalWrite(linksVoor2, HIGH);
    digitalWrite(linksAchter2, LOW);
    digitalWrite(linksAchter1, HIGH);
    digitalWrite(rechtsAchter2, HIGH);
    digitalWrite(rechtsAchter1, LOW);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(rechtsVoor1, HIGH);
      digitalWrite(rechtsVoor2, LOW);
      digitalWrite(linksVoor1, LOW);
      digitalWrite(linksVoor2, HIGH);
      digitalWrite(linksAchter2, LOW);
      digitalWrite(linksAchter1, HIGH);
      digitalWrite(rechtsAchter2, HIGH);
      digitalWrite(rechtsAchter1, LOW);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}
void turnRight(int x)
{
  if (x == 0)
  {
    digitalWrite(rechtsVoor1, LOW);
    digitalWrite(rechtsVoor2, HIGH);
    digitalWrite(linksVoor1, HIGH);
    digitalWrite(linksVoor2, LOW);
    digitalWrite(linksAchter2, HIGH);
    digitalWrite(linksAchter1, LOW);
    digitalWrite(rechtsAchter2, LOW);
    digitalWrite(rechtsAchter1, HIGH);
    analogWrite(linksVoorPWM, 250);
    analogWrite(linksAchterPWM, 250);
    analogWrite(rechtsVoorPWM, 250);
    analogWrite(rechtsAchterPWM, 250);
  }
  else
  {
    static unsigned long startTime = millis();
    if (millis() - startTime < x)
    {
      digitalWrite(rechtsVoor1, LOW);
      digitalWrite(rechtsVoor2, HIGH);
      digitalWrite(linksVoor1, HIGH);
      digitalWrite(linksVoor2, LOW);
      digitalWrite(linksAchter2, HIGH);
      digitalWrite(linksAchter1, LOW);
      digitalWrite(rechtsAchter2, LOW);
      digitalWrite(rechtsAchter1, HIGH);
      analogWrite(linksVoorPWM, 250);
      analogWrite(linksAchterPWM, 250);
      analogWrite(rechtsVoorPWM, 250);
      analogWrite(rechtsAchterPWM, 250);
    }
  }
}

void hugRightLine()
{
  while (digitalRead(rightFrontIR) && digitalRead(rightBackIR))
  {
    moveRight(0);
  }
  while (!digitalRead(rightFrontIR) && digitalRead(rightBackIR))
  {
    turnLeft(0);
  }
  while (digitalRead(rightFrontIR) && !digitalRead(rightBackIR))
  {
    turnRight(0);
  }
  stop();
}

void hugLeftLine()
{
  while (digitalRead(leftBackIR) && digitalRead(leftFrontIR))
  {
    moveLeft(0);
  }
  while (!digitalRead(leftBackIR) && digitalRead(leftFrontIR))
  {
    turnLeft(0);
  }
  while (digitalRead(leftBackIR) && !digitalRead(leftFrontIR))
  {
    turnRight(0);
  }
}

void spaceWalk()
{
}

void loop()
{
  switch (state)
  {

  case 0: // Backward
    moveBackward(0);
    break;

  case 1: // Forward
    moveForward(0);
    break;

  case 2: // Diagonal right
    moveDiagonalRight(0);
    break;

  case 3: // Diagonal left
    moveDiagonalLeft(0);
    break;

  case 4: // Left
    moveLeft(0);
    break;

  case 5: // Right
    moveRight(0);
    break;

  case 6: // Turn left
    turnLeft(0);
    break;

  case 7: // Turn right
    turnRight(0);
    break;

  default: // Stop
    stop();
  }
}