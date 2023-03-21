#include "WiFi.h"

#define WIFI_NETWORK "Network name"
#define WIFI_PASSWORD "Password"
#define WIFI_TIMEOUT_MS 20000

void connectToWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS)
    {
        delay(100);
    }

    if (WiFi.status() != WL_CONNECTED)
    {
        sleep(30);
    }
}