#ifndef STREAMSERVER_H
#define STREAMSERVER_H
#include <Arduino.h>
#include <esp_http_server.h>
#include <sensor.h>

class StreamServer
{
public:
    // Returns false when esp_camera_init() fails, so the caller can report it
    // instead of serving a stream that can never produce a frame.
    bool init(framesize_t frameSize,
              int jpegQuality,
              bool increaseFps);

    // The /stream and /capture endpoints are only registered when init()
    // succeeded.
    void startStream();

protected:
    static esp_err_t index_handler(httpd_req_t *req);
    static esp_err_t stream_handler(httpd_req_t *req);
    static esp_err_t capture_handler(httpd_req_t *req);

private:
    bool cameraReady = false;
};

#endif
