
#include <Arduino.h>
#include <WifiHandler.h>
#include <StreamServer.h>
#include <SocketServer.h>
#include <DCMotor.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>
#include <Car.h>
#include <PwmLed.h>
#include <AdafruitLed.h>
#include <DigitalLed.h>
#include <sensor.h>
#include <Config.h>

// Car components
DCMotor motor1(PIN_M1_IN1, PIN_M1_IN2);
DCMotor motor2(PIN_M2_IN1, PIN_M2_IN2);
PwmLed frontLed(PIN_FRONT_LED);

#if defined(PIN_RGB_LED)
AdafruitLed boardLed(PIN_RGB_LED);
#elif defined(PIN_BOARD_LED)
DigitalLed boardLed(PIN_BOARD_LED);
#endif

Car car(motor1, motor2, frontLed);

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

// RGB LED customization
#if defined(PIN_RGB_LED)
  boardLed.init(); // requires initialization
  boardLed.setGreen();
  boardLed.setLowBrightness();
#elif defined(PIN_BOARD_LED)
  boardLed.setInverted(true); // built-in board LED is inverted
#endif

  // Camera first: esp_camera_init() claims LEDC channel 0 / timer 0 for the
  // XCLK, so the motor and front LED channels must only be set up afterwards.
  const bool cameraReady = streamServer.init(FRAME_SIZE,
                                             JPEG_QUALITY,
                                             INCREASE_FPS);

#if defined(PIN_RGB_LED)
  if (!cameraReady)
  {
    boardLed.setColor(255, 0, 0); // red signals a camera failure
  }
#endif

  // Set up the front LED PWM channel and start stopped
  car.init();
  car.setMinAbsSpeed(MIN_MOTOR_SPEED);
  car.stop();

  // Wi-Fi connection
  if (WIFI_AP_MODE)
  {
    wifiHandler.apMode(WIFI_SSID, WIFI_PWD);
  }
  else if (!wifiHandler.connect(WIFI_SSID, WIFI_PWD))
  {
    // fall back to the access point so the car stays reachable
    Serial.println("Falling back to AP mode");
    wifiHandler.apMode(WIFI_SSID, WIFI_PWD);
  }

  // Start streaming web server
  streamServer.startStream();

  // Set Web Joystick (Web sockets)
  webJoystickHandler.setDebug(JOYSTICK_DEBUG);

  // Start Web Sockets
  CoordsHandlerFunction coordsHandler = [](const JoyCoords &coords)
  {
    webJoystickHandler.handle(coords);
  };

  ButtonStateHandlerFunction buttonAHandler = [](uint8_t state)
  {
    if (state == 2)
    {
      webJoystickHandler.frontLightsHigh();
    }
    else if (state == 1)
    {
      webJoystickHandler.frontLightsLow();
    }
    else
    {
      webJoystickHandler.frontLightsOff();
    }
  };

  // Failsafe: stop the motors when the client disconnects or goes quiet
  StopHandlerFunction stopHandler = []()
  {
    car.stop();
  };

  socketServer.init(coordsHandler,
                    buttonAHandler,
                    stopHandler,
                    COMMAND_TIMEOUT_MS);

  // turn on built-in board led to indicate the car is ready
  boardLed.turnOn();
}

void loop()
{
  socketServer.loop();

  // yield to the httpd / LwIP tasks instead of spinning a core at 100%
  delay(1);
}
