#include <Arduino.h>
#include <WiFi.h>
#include <WifiHandler.h>

bool WifiHandler::connect(const char *ssid,
                          const char *pwd,
                          uint32_t timeoutMs)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pwd);

    const uint32_t startedAt = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        if ((millis() - startedAt) > timeoutMs)
        {
            Serial.printf("\nWi-Fi connection to %s timed out\n", ssid);
            return false;
        }

        delay(500);
        Serial.print(".");
    }

    WiFi.setSleep(false);

    debug(ssid, false);

    return true;
}

bool WifiHandler::apMode(const char *ssid,
                         const char *pwd,
                         int channel)
{
    WiFi.mode(WIFI_AP);

    if (!WiFi.softAP(ssid, pwd, channel))
    {
        Serial.printf("Failed to start the access point %s\n", ssid);
        return false;
    }

    WiFi.setSleep(false);

    debug(ssid, true);

    return true;
}

void WifiHandler::debug(const char *ssid,
                        bool apMode)
{
    Serial.println("");

    if (apMode)
    {
        Serial.print("AP Mode connected at ");
    }
    else
    {
        Serial.print("Wi-Fi connected at ");
    }

    Serial.println(ssid);
    Serial.print("http://");

    if (apMode)
    {
        Serial.println(WiFi.softAPIP());
    }
    else
    {
        Serial.println(WiFi.localIP());
    }
}
