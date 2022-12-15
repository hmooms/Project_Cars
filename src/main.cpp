#include <WebServer.h>
#include <ESPmDNS.h>

#include "../lib/Analog.h"
#include "../lib/MotorDriver.h"

#include <WiFi.h>

const int trigPin = 21;
const int echoPin = 27;
long duration;
float distanceCm;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

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

float distanceDetection()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

float baseHeight;

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
  pinMode(echoPin, INPUT);

  baseHeight = distanceDetection();

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
    analogWrite(linksVoorPWM, 120);
    analogWrite(linksAchterPWM, 120);
    analogWrite(rechtsVoorPWM, 120);
    analogWrite(rechtsAchterPWM, 120);
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
      analogWrite(linksVoorPWM, 120);
      analogWrite(linksAchterPWM, 120);
      analogWrite(rechtsVoorPWM, 120);
      analogWrite(rechtsAchterPWM, 120);
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

int state = 1;

void loop()
{
  static unsigned long startTimee = millis();
  switch (state)
  {
  case 0:
    moveBackward(0);
    if (millis() - startTimee > 3000)
    {
      startTimee = millis();
      state = -1;
    }
    break;
  case 1:
    moveForward(0);
    // if (distanceDetection() > baseHeight + 5)
    // {
    //   startTimee = millis();
    //   state = 0;
    // }
    if (distanceDetection() < 15)
    {
      state = 2;
    }
    break;
  case 2:
    moveDiagonalRight(0);
    if (millis() - startTimee > 2000)
    {
      startTimee = millis();
      state = 1;
    }
  default:
    stop();
  }
}