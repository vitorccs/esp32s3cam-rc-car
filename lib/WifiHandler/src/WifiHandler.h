#ifndef WIFI_HANDLER_H
#define WIFI_HANDLER_H
#include <WiFi.h>

class WifiHandler
{
public:
    bool connect(const char *ssid, const char *pwd, uint32_t timeoutMs = 20000);
    bool apMode(const char *ssid, const char *pwd, int channel = 1);

private:
    void debug(const char *ssid, bool apMode);
};
#endif
