// WiFi credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PASSWORD"

// Access Point mode 
// + true = creates a WiFi network without internet connection
// + false = connect to an existing WiFi with internet connection
#define WIFI_AP_MODE false

// Set minimum motor speed (0 to 255)
#define MIN_MOTOR_SPEED 80

// Enable debug (prints car speed and direction in the serial)
#define JOYSTICK_DEBUG false

// Set camera model
//#define CAMERA_MODEL_AI_THINKER
#define CAMERA_MODEL_ESP32S3_CAM_BOARD
// #define CAMERA_MODEL_XIAO_ESP32S3

// Set JPEG quality (0 to 63 - lower means higher quality)
// NOTE: Higher image quality reduces CPU usage for compression.
#define JPEG_QUALITY 20

// Customize PINS
#if defined(CAMERA_MODEL_ESP32S3_CAM_BOARD)
#define PIN_M1_IN1 14
#define PIN_M1_IN2 47
#define PIN_M2_IN1 38
#define PIN_M2_IN2 39
#define PIN_FRONT_LED_1 41
#define PIN_FRONT_LED_2 40
#define PIN_CAMERA_LED 48
#define PIN_BOARD_LED 2
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_SVGA

#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
#define PIN_M1_IN1 1
#define PIN_M1_IN2 2
#define PIN_M2_IN1 3
#define PIN_M2_IN2 4
#define PIN_FRONT_LED_1 9
#define PIN_FRONT_LED_2 8
#define PIN_CAMERA_LED 7
#define PIN_BOARD_LED 21
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_SVGA

#elif defined(CAMERA_MODEL_AI_THINKER)
#define PIN_M1_IN1 14
#define PIN_M1_IN2 15
#define PIN_M2_IN1 12
#define PIN_M2_IN2 13
#define PIN_FRONT_LED_1 2
#define PIN_FRONT_LED_2 3
#define PIN_CAMERA_LED 4
#define PIN_BOARD_LED 33
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_VGA

#endif
