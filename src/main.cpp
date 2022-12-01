#include <Arduino.h>
// L293D
// Motor A rechtsvoor
const int rechtsVoor1 = 23; // Pin
const int rechtsVoor2 = 22; // Pin

// Motor B linksvoor
const int linksVoor1 = 5;  // Pin
const int linksVoor2 = 17; // Pin

// Motor C linksachter
const int linksAchter1 = 16; // Pin
const int linksAchter2 = 4;  // Pin

// Motor D rechtsachter
const int rechtsAchter1 = 19; // Pin
const int rechtsAchter2 = 18; // Pin

// This will run only one time.
void setup()
{
  // Set pins as outputs
  pinMode(rechtsVoor1, OUTPUT);
  pinMode(rechtsVoor2, OUTPUT);
  pinMode(linksVoor1, OUTPUT);
  pinMode(linksVoor2, OUTPUT);
  pinMode(linksAchter1, OUTPUT);
  pinMode(linksAchter2, OUTPUT);
  pinMode(rechtsAchter1, OUTPUT);
  pinMode(rechtsAchter2, OUTPUT);
}
void moveForward()
{
  // Move forwards
  digitalWrite(rechtsVoor1, HIGH);
  digitalWrite(rechtsVoor2, LOW);
  digitalWrite(linksVoor1, HIGH);
  digitalWrite(linksVoor2, LOW);
  digitalWrite(linksAchter2, HIGH);
  digitalWrite(linksAchter1, LOW);
  digitalWrite(rechtsAchter2, HIGH);
  digitalWrite(rechtsAchter1, LOW);
}
void moveBackward()
{
  // Move backwards
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, HIGH);
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, HIGH);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, HIGH);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, HIGH);
}
void moveLeft()
{
  // Move left
  digitalWrite(rechtsVoor1, HIGH);
  digitalWrite(rechtsVoor2, LOW);
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, HIGH);
  digitalWrite(linksAchter2, HIGH);
  digitalWrite(linksAchter1, LOW);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, HIGH);
}
void moveRight()
{
  // Move right
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, HIGH);
  digitalWrite(linksVoor1, HIGH);
  digitalWrite(linksVoor2, LOW);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, HIGH);
  digitalWrite(rechtsAchter2, HIGH);
  digitalWrite(rechtsAchter1, LOW);
}
void stop()
{
  // Move right
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, LOW);
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, LOW);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, LOW);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, LOW);
}
void turnLeft()
{
  // Turn left
  digitalWrite(rechtsVoor1, HIGH);
  digitalWrite(rechtsVoor2, LOW);
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, HIGH);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, HIGH);
  digitalWrite(rechtsAchter2, HIGH);
  digitalWrite(rechtsAchter1, LOW);
}
void turnRight()
{
  // Turn right
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, HIGH);
  digitalWrite(linksVoor1, HIGH);
  digitalWrite(linksVoor2, LOW);
  digitalWrite(linksAchter2, HIGH);
  digitalWrite(linksAchter1, LOW);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, HIGH);
}

void loop()
{
  turnRight();
  delay(2000);
  turnLeft();
  delay(2000);
  moveRight();
  delay(2000);
  moveLeft();
  delay(2000);
  moveForward();
  delay(2000);
  moveBackward();
  delay(2000);
}