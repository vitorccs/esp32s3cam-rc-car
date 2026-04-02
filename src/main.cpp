
#include <Arduino.h>
#include <WifiHandler.h>
#include <StreamServer.h>
#include <SocketServer.h>
#include <DCMotor.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>
#include <Car.h>
#include <DigitalLed.h>
#include <AdafruitLed.h>
#include <sensor.h>
#include <Config.h>

// Car components
DCMotor motor1(PIN_M1_IN1, PIN_M1_IN2);
DCMotor motor2(PIN_M2_IN1, PIN_M2_IN2);
DigitalLed frontLed1(PIN_FRONT_LED_1);
DigitalLed frontLed2(PIN_FRONT_LED_2);

#if defined(CAMERA_MODEL_ESP32S3_CAM_BOARD)
  AdafruitLed camLed(PIN_CAMERA_LED); 
#else 
  DigitalLed camLed(PIN_CAMERA_LED);
#endif 

Car car(motor1, motor2, frontLed1, frontLed2, camLed);
DigitalLed boardLed(PIN_BOARD_LED);

// Handlers
WifiHandler wifiHandler;
WebJoystickHandler webJoystickHandler(car);

// Stream and Socket servers
StreamServer streamServer = StreamServer();
SocketServer socketServer = SocketServer();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  // Adafruit RGB LED requires initialization
  #if defined(CAMERA_MODEL_ESP32S3_CAM_BOARD)
    camLed.init();
  #endif

  // initialize car stopped
  car.stop();
  car.setMinAbsSpeed(MIN_MOTOR_SPEED);

  streamServer.init(FRAME_SIZE, JPEG_QUALITY);

  // Wi-Fi connection
  if (WIFI_AP_MODE)
  {
    wifiHandler.apMode(WIFI_SSID, WIFI_PWD);
  }
  else
  {
    wifiHandler.connect(WIFI_SSID, WIFI_PWD);
  }

  // Start streaming web server
  streamServer.startStream();

  // Set Web Joystick (Web sockets)
  webJoystickHandler.setDebug(JOYSTICK_DEBUG);

  // Start Web Sockets
  CoordsHandlerFunction coordsHandler = [&](JoyCoords coords)
  {
    webJoystickHandler.handle(coords);
  };

  ButtonToggleHandlerFunction buttonAHandler = [&](bool toggle)
  {
    webJoystickHandler.toggleCamLed(toggle);
  };

  ButtonToggleHandlerFunction buttonBHandler = [&](bool toggle)
  {
    webJoystickHandler.toggleFrontLights(toggle);
  };

  socketServer.init(coordsHandler,
                    buttonAHandler,
                    buttonBHandler);

  // turn on built-in board led to indicate the car is ready
  boardLed.turnOn();
}

void loop()
{
  socketServer.loop();
}
