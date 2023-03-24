// Include necessary libraries
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "../lib/server.cpp"

// Define pins for various sensors and actuators
#define leftFrontIR 36
#define rightFrontIR 39
#define rightBackIR 2
#define leftBackIR 27
#define trigPinForward 13
#define trigPinDown 21
#define echoForward 34
#define echoDown 35
#define magSensor 5

#define HEIGHT 2.63
#define sensorStatusPin 12
#define SOUND_SPEED 0.034

// Motor Pins configuration
int motorPins[][3] = {
    {32, 33, 16}, // left front
    {25, 26, 15}, // left rear
    {23, 22, 4},  // right front
    {19, 18, 17}  // right rear
};

// Variables for measuring time intervals, distances and keeping track of states
long duration;
float distanceCm;
unsigned long leftFrontIRTriggeredTime = 0;
unsigned long rightFrontIRTriggeredTime = 0;
int leftFrontIRCounter = 0;
int rightFrontIRCounter = 0;
const unsigned long triggerInterval = 5000; // 5 seconds in milliseconds
unsigned long lastSensorStatusChangeTime = 0;

// Function to stop the robot
void stop();

// Function to calculate distance using ultrasonic sensor
float distanceDetection(uint8_t pin)
{
  byte trigPin = (pin == echoForward) ? trigPinForward : trigPinDown;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(pin, HIGH);
  float distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

// Function to check if the robot is stuck in a corner
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

// Main setup function
void setup()
{
  // Initialize Serial communication
  Serial.begin(9600);
  static unsigned long startTimer = millis();

  // Set pin modes for all sensors and actuators
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
  pinMode(trigPinForward, OUTPUT);
  pinMode(trigPinDown, OUTPUT);
  pinMode(echoForward, INPUT);
  pinMode(echoDown, INPUT);

  // Set up base height
  uint8_t setupHeight = distanceDetection(echoDown);

  // Set up the server
  setupServer(leftFrontIR, rightFrontIR, leftBackIR, rightBackIR, magSensor);
}

// Function to move the robot in a specific mode for a specific time
void move(int mode, int time)
{
  // Set motor directions and speeds according to mode
  for (int i = 0; i < 4; ++i)
  {
    digitalWrite(motorPins[i][0], mode & (1 << i) ? HIGH : LOW);
    digitalWrite(motorPins[i][1], mode & (1 << i) ? LOW : HIGH);
    analogWrite(motorPins[i][2], (mode == 0b1010) ? 170 : 200);
  }

  // Delay for the specified time (if any)
  if (time != 0)
  {
    delay(time);
  }
}

// Movement functions for the robot
void moveForward(int time) { move(0b1010, time); }
void moveBackward(int time) { move(0b0101, time); }
void moveLeft(int time) { move(0b0011, time); }
void moveRight(int time) { move(0b1100, time); }
void turnLeft(int time) { move(0b1001, time); }
void turnRight(int time) { move(0b0110, time); }
void rampCalibration(int time) { move(0b1101, time); }

// Function to stop the robot
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

// Function to handle obstacles and adjust robot's movement accordingly
void handleObstacle()
{
  float distanceFront = distanceDetection(echoForward);
  float distanceDown = distanceDetection(echoDown);
  digitalWrite(sensorStatusPin, HIGH);
  // Check for corner and react accordingly
  if (isStuckInCorner())
  {
    stop();
    moveBackward(1000);
    turnLeft(1000);
  }
  else
  {
    // React to different sensor inputs and adjust movement
    if (distanceDown >= (5 + HEIGHT))
    {
      moveBackward(500);
      turnRight(500);
    }
    else if (distanceDown > (HEIGHT + 1))
    {
      moveForward(2000);
    }
    else if (digitalRead(rightFrontIR) && digitalRead(leftFrontIR))
    {
      moveBackward(500);
      turnRight(500);
    }
    else if (digitalRead(rightFrontIR) && digitalRead(rightBackIR))
    {
      moveLeft(200);
    }
    else if (digitalRead(leftFrontIR) && digitalRead(leftBackIR))
    {
      moveRight(200);
    }
    else if (digitalRead(rightFrontIR))
    {
      moveBackward(500);
      turnLeft(200);
    }
    else if (digitalRead(leftFrontIR))
    {
      moveBackward(500);
      turnRight(200);
    }
    else if (digitalRead(rightBackIR) || digitalRead(leftBackIR))
    {
      moveForward(100);
    }
    else if (distanceFront <= 15 && distanceFront >= 5)
    {
      moveLeft(500);
    }

    else
    {
      // Serial.println("hello");
      moveForward(0);
      digitalWrite(sensorStatusPin, LOW);
    }
  }
}

// Main loop function
void loop()
{
  ACMStatus = digitalRead(ACMStatusPin);

  // Check if the robot is allowed to move based on the magnetic sensor
  if (digitalRead(magSensor))
  {
    ACMStatus = false;
    digitalWrite(ACMStatusPin, LOW);
  }

  // If ACMStatus is true, handle obstacles and move accordingly
  if (ACMStatus)
  {
    handleObstacle();
  }
  else
  {
    // If ACMStatus is false, stop the robot and turn off the sensor status LED
    stop();
    digitalWrite(sensorStatusPin, LOW);
  }
}
