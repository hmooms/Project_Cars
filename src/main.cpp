#include <WebServer.h>
#include <ESPmDNS.h>

#include "../lib/Analog.h"
#include "../lib/MotorDriver.h"

#include <WiFi.h>

const char *ssid = "Hugo's Galaxy A52s 5G";
const char *password = "blpv8004";

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

int distanceDetection()
{
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distanceCm = duration * SOUND_SPEED / 2;
  return distanceCm;
}

// WebServer server(80);

// void onMovementDataReceive();

int baseHeight;

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

  // MotorDriver::defineMotor(32, 33, 16, direction::left);
  // MotorDriver::defineMotor(26, 25, 15, direction::left); // achter

  // MotorDriver::defineMotor(23, 22, 4, direction::right);
  // MotorDriver::defineMotor(19, 18, 17, direction::right); // achter

  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }

  // Serial.println("");
  // Serial.print("Connected to ");
  // Serial.println(ssid);
  // Serial.print("IP address: ");
  // Serial.println(WiFi.localIP());

  // if (MDNS.begin("esp32"))
  // {
  //   Serial.println("MDNS responder started");
  // }

  // server.on("/api", HTTP_POST, onMovementDataReceive);

  // server.begin();
  // Serial.println("HTTP server started");
}

void moveForward()
{
  // Move forwards
  digitalWrite(linksVoor1, HIGH);
  digitalWrite(linksVoor2, LOW);
  analogWrite(linksVoorPWM, 120);
  digitalWrite(linksAchter2, HIGH);
  digitalWrite(linksAchter1, LOW);
  analogWrite(linksAchterPWM, 120);
  digitalWrite(rechtsVoor1, HIGH);
  digitalWrite(rechtsVoor2, LOW);
  analogWrite(rechtsVoorPWM, 120);
  digitalWrite(rechtsAchter2, HIGH);
  digitalWrite(rechtsAchter1, LOW);
  analogWrite(rechtsAchterPWM, 120); // set LED brightness as PWM signal
}
void moveBackward()
{
  // Move forwards
  digitalWrite(linksVoor1, LOW);
  digitalWrite(linksVoor2, HIGH);
  digitalWrite(linksVoorPWM, HIGH);
  digitalWrite(linksAchter2, LOW);
  digitalWrite(linksAchter1, HIGH);
  digitalWrite(linksAchterPWM, HIGH);
  digitalWrite(rechtsVoor1, LOW);
  digitalWrite(rechtsVoor2, HIGH);
  digitalWrite(rechtsVoorPWM, HIGH);
  digitalWrite(rechtsAchter2, LOW);
  digitalWrite(rechtsAchter1, HIGH);
  digitalWrite(rechtsAchterPWM, HIGH);
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
  digitalWrite(rechtsAchterPWM, LOW);
}

// void onMovementDataReceive()
// {
//   float tl = server.arg("tl").toFloat();
//   float bl = server.arg("bl").toFloat();
//   float tr = server.arg("tr").toFloat();
//   float br = server.arg("br").toFloat();

//   Serial.println(tl);
//   Serial.println(bl);
//   Serial.println(tr);
//   Serial.println(br);

//   MotorDriver::setMotorDirect(0, tl);
//   MotorDriver::setMotorDirect(1, bl);
//   MotorDriver::setMotorDirect(2, tr);
//   MotorDriver::setMotorDirect(3, br);

//   server.send(200);
// }

void loop()
{
  if (distanceDetection() > baseHeight + 5)
  {
    stop();
  }
  else
  {
    moveForward();
  }
}