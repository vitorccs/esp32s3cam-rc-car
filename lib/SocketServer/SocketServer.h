#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <JoyCoords.h>

typedef std::function<void(const JoyCoords &coords)> CoordsHandlerFunction;
typedef std::function<void(uint8_t buttonState)> ButtonStateHandlerFunction;
typedef std::function<void()> StopHandlerFunction;

class SocketServer
{
public:
    SocketServer();

    /**
     * stopHandler is the failsafe: it is invoked when the client disconnects
     * and when no valid command arrives for commandTimeoutMs. Without it, the
     * last PWM duty would persist and the car would keep driving after a
     * WiFi or browser drop.
     */
    void init(CoordsHandlerFunction coordsHandler,
              ButtonStateHandlerFunction btnAHandler,
              StopHandlerFunction stopHandler,
              uint32_t commandTimeoutMs);

    void onEvent(uint8_t num,
                 WStype_t type,
                 uint8_t *payload,
                 size_t length);

    void handleWebSocketMessage(char *dataChar);

    void loop();

private:
    WebSocketsServer webSocket;
    CoordsHandlerFunction _coordsHandler;
    ButtonStateHandlerFunction buttonAHandler;
    StopHandlerFunction _stopHandler;
    uint32_t _commandTimeoutMs = 500;
    uint32_t _lastCommandMs = 0;
    bool _stopped = true;
    const char *ALIAS_DIRECTION;
    const char *ALIAS_SPEED;
    const char *ALIAS_BUTTON_A;

    void failsafeStop();
};

#endif
