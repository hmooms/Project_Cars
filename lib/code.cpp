#include <WebServer.h>
#include <ESPmDNS.h>

#include "../lib/Analog.h"
#include "../lib/MotorDriver.h"

#include <WiFi.h>

const char *ssid = "Hugo's Galaxy A52s 5G";
const char *password = "blpv8004";

WebServer server(80);

void onMovementDataReceive();

void setup()
{
    // MotorDriver::defineMotor()

    Serial.begin(9600);

    MotorDriver::defineMotor(32, 33, 16, direction::left);
    MotorDriver::defineMotor(26, 25, 15, direction::left); // achter

    MotorDriver::defineMotor(23, 22, 4, direction::right);
    MotorDriver::defineMotor(19, 18, 17, direction::right); // achter

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp32"))
    {
        Serial.println("MDNS responder started");
    }

    server.on("/api", HTTP_POST, onMovementDataReceive);

    server.begin();
    Serial.println("HTTP server started");
}

void onMovementDataReceive()
{
    float tl = server.arg("tl").toFloat();
    float bl = server.arg("bl").toFloat();
    float tr = server.arg("tr").toFloat();
    float br = server.arg("br").toFloat();

    Serial.println(tl);
    Serial.println(bl);
    Serial.println(tr);
    Serial.println(br);

    MotorDriver::setMotorDirect(0, tl);
    MotorDriver::setMotorDirect(1, bl);
    MotorDriver::setMotorDirect(2, tr);
    MotorDriver::setMotorDirect(3, br);

    server.send(200);
}

void loop()
{
    server.handleClient();
}