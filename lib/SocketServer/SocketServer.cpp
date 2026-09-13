#include <Arduino.h>
#include <SocketServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

SocketServer::SocketServer() : webSocket(8002)
{
    ALIAS_BUTTON_A = "button-a";
    ALIAS_DIRECTION = "direction";
    ALIAS_SPEED = "speed";

    auto handler = [&](uint8_t num,
                       WStype_t type,
                       uint8_t *payload,
                       size_t length)
    {
        return onEvent(num, type, payload, length);
    };

    webSocket.onEvent(handler);
}

void SocketServer::onEvent(uint8_t num,
                           WStype_t type,
                           uint8_t *payload,
                           size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        failsafeStop();
        break;
    case WStype_CONNECTED:
        Serial.printf("[%u] Connected!\n", num);
        break;
    case WStype_TEXT:
        handleWebSocketMessage((char *)payload);
        break;
    case WStype_ERROR:
        Serial.printf("[%u] Error!\n", num);
        failsafeStop();
        break;
    case WStype_BIN:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void SocketServer::init(CoordsHandlerFunction coordsHandler,
                        ButtonStateHandlerFunction btnAHandler,
                        StopHandlerFunction stopHandler,
                        uint32_t commandTimeoutMs)
{
    _coordsHandler = coordsHandler;
    buttonAHandler = btnAHandler;
    _stopHandler = stopHandler;
    _commandTimeoutMs = commandTimeoutMs;
    _lastCommandMs = millis();

    webSocket.begin();
}

void SocketServer::handleWebSocketMessage(char *dataChar)
{
    const bool isMovCoords = strstr(dataChar, ALIAS_SPEED) != nullptr &&
                             strstr(dataChar, ALIAS_DIRECTION) != nullptr;
    const bool isButtonA = strstr(dataChar, ALIAS_BUTTON_A) != nullptr;

    if (isMovCoords)
    {
        // parse string as a two-members JSON
        const int jsonSize = JSON_OBJECT_SIZE(2);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, dataChar);

        if (error)
        {
            Serial.printf("Invalid coords payload: %s\n", error.c_str());
            return;
        }

        const char *direction = json[ALIAS_DIRECTION].as<const char *>();

        if (direction == nullptr)
        {
            return;
        }

        JoyCoords coords;
        coords.speed = json[ALIAS_SPEED];
        strlcpy(coords.direction, direction, sizeof(coords.direction));

        _lastCommandMs = millis();
        _stopped = false;

        if (_coordsHandler)
        {
            _coordsHandler(coords);
        }

        return;
    }

    if (isButtonA)
    {
        const int jsonSize = JSON_OBJECT_SIZE(1);
        StaticJsonDocument<jsonSize> json;
        DeserializationError error = deserializeJson(json, dataChar);

        if (error)
        {
            Serial.printf("Invalid button payload: %s\n", error.c_str());
            return;
        }

        const uint8_t state = json[ALIAS_BUTTON_A];

        if (buttonAHandler)
        {
            buttonAHandler(state);
        }

        return;
    }
}

void SocketServer::loop()
{
    webSocket.loop();

    // Failsafe: the UI sends a command every 50 ms, so going quiet for
    // _commandTimeoutMs means we lost the client (WiFi drop, tab frozen).
    // Unsigned arithmetic keeps this correct across the millis() rollover.
    if (!_stopped && (millis() - _lastCommandMs) > _commandTimeoutMs)
    {
        Serial.println("Command timeout - stopping the car");
        failsafeStop();
    }
}

void SocketServer::failsafeStop()
{
    if (_stopped)
    {
        return;
    }

    _stopped = true;

    if (_stopHandler)
    {
        _stopHandler();
    }
}
