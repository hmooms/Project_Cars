#include <Arduino.h>
#include <WiFi.h>
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
    {19, 13, 17}  // rechtsachter
};

long duration;
float distanceCm;
int state = 1;

String sensorStatus = "off";

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
    analogWrite(motorPins[i][2], 250);
  }
}

void moveForward(int time) { move(0b1010, time); }
void moveBackward(int time) { move(0b0101, time); }
void moveLeft(int time) { move(0b1001, time); }
void moveRight(int time) { move(0b0110, time); }
void turnLeft(int time) { move(0b1100, time); }
void turnRight(int time) { move(0b0011, time); }

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
  // checkForClients();
  // float distance = distanceDetection();

  // if (ACMStatusPinState == "on")
  // {
  //   digitalWrite(ACMStatusPin, HIGH);

  //   if (digitalRead(leftFrontIR) || digitalRead(rightFrontIR) || digitalRead(leftBackIR) || digitalRead(rightBackIR))
  //   {
  //     sensorStatus = "IR";
  //   }
  //   else if (distance < 5)
  //   {
  //     sensorStatus = "alert";
  //   }
  //   else if (distance < 20)
  //   {
  //     sensorStatus = "US";
  //   }
  //   else
  //   {
  //     sensorStatus = "off";
  //   }
  // moveForward(0);
  analogWrite(19, HIGH); // Set pin 19 to HIGH
  analogWrite(18, HIGH); // Set pin 18 to LOW
  analogWrite(17, 250);  // Set PWM to pin 17

  delay(2000); // Wait for 2 seconds

  analogWrite(19, HIGH); // Set pin 19 to LOW
  analogWrite(18, HIGH); // Set pin 18 to HIGH
  analogWrite(17, 250);  // Set PWM to pin 17

  delay(2000); // Wait for 2 seconds
  // }
  // else
  // {
  //   stop();
  //   digitalWrite(ACMStatusPin, LOW);
  // }

  // if (sensorStatus == "off")
  // {
  //   digitalWrite(sensorStatusPin, LOW);
  // }
  // else if (sensorStatus == "IR")
  // {
  //   digitalWrite(sensorStatusPin, HIGH);
  // }
  // else if (sensorStatus == "US")
  // {
  //   digitalWrite(sensorStatusPin, HIGH);
  //   delay(500);
  //   digitalWrite(sensorStatusPin, LOW);
  //   delay(500);
  // }
  // else if (sensorStatus == "alert")
  // {
  //   digitalWrite(sensorStatusPin, HIGH);
  //   delay(250);
  //   digitalWrite(sensorStatusPin, LOW);
  //   delay(250);
  // }
}
