#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H
#include <Arduino.h>
#include <WebSocketsServer.h>
#include <JoyCoords.h>

typedef std::function<void(JoyCoords coords)> CoordsHandlerFunction;
typedef std::function<void(uint8_t buttonState)> ButtonStateHandlerFunction;

class SocketServer
{
public:
    SocketServer();

    void init(CoordsHandlerFunction coordsHandler,
              ButtonStateHandlerFunction btnAHandler);

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
    const char *ALIAS_DIRECTION;
    const char *ALIAS_SPEED;
    const char *ALIAS_BUTTON_A;
};

#endif