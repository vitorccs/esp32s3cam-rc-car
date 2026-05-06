# ESP32S3-CAM RC Car
A remote-controlled car built on an ESP32-S3-based platform, featuring a more powerful microcontroller that delivers smooth, low-latency video streaming.

This project is the successor to the [ESP32CAM-RC-Car project](https://github.com/vitorccs/esp32cam-rc-car), which was based on a previous-generation ESP32 board.

It also uses the DRV8833 as a motor driver, improving power efficiency and reaching higher speeds.

<img src="https://github.com/user-attachments/assets/ce574718-f88c-4e82-9be9-0e0c2ca61ab5" width="380">
<img src="https://github.com/user-attachments/assets/1f5df0d0-f162-40a3-9eee-81be8f22557f" width="380">
<img src="https://github.com/user-attachments/assets/5294549f-c83e-4bb5-92a6-7fd01303fca6" width="380">
<img src="https://github.com/user-attachments/assets/0c9c748f-d447-4257-a3c9-4555bdf3c234" width="380">


## Description
Build a Wi-Fi-controlled surveillance car using an ESP32S3-CAM board. 

The car is controlled by a Web page containing a virtual joystick.

<img src="https://github.com/user-attachments/assets/d3b44d02-4885-400b-9ad1-8a20e1871b38" width="300">

You can also control the car with a Keyboard. Activate it by pressing the Enter key at any time, and press it again to switch back to the virtual joystick. Use the `W`, `A`, `S`, `D` or `arrow` keys, along with (`L`)ed and (`F`)lash.
<img src="https://github.com/user-attachments/assets/3313343c-4895-4c40-9076-929d0d6fde80" />

If you are familiar with configuring internet routers and setting port forwarding, you can also control your car remotely!
1) Ensure you assign a static IP.
2) Set up the following port forwarding rules:
* 8000 (web page)
* 8001 (video streaming)
* 8002 (WebSocket / joystick commands).
3) If your ISP changes your public IP frequently, set up a DDNS service (e.g., No-IP)

## Recording
Note: enable the sound by clicking on the speaker icon from the video toolbar.

https://github.com/user-attachments/assets/d405cac1-93c0-4656-b3de-b336b7a0a3e5

https://github.com/user-attachments/assets/688e1cb7-99dc-43c0-b13c-eb8923ff54a2

## Compatible boards
### ESP32S3-CAM
The "ESP32S3-CAM" board succeeds the "AI Thinker" board, which was based on the older ESP32. This board can be found at [AliExpress](http://aliexpress.com/w/wholesale-esp32s3cam.html). 

NOTE: there are slightly different models and they may have different GPIO numbers. The project was based on this model. There is also an RGB LED in the position which used to be a bright FLASH LED.

<img src="https://github.com/user-attachments/assets/d590d683-830d-4006-8bae-eb9d859a37c2" width="330">

### Seeed Studio XIAO ESP32-S3 Series
It works perfectly in this project. When building and uploading the project, remember to change the env to "xiaoS3cam". You will have to change the PINS configuration at `Config.h`.

<img src="https://github.com/user-attachments/assets/08a81881-be41-45f0-be26-7417dd8f78cb" width="330">

## Schematics

### Option 1 - Easier to build (uses L298N bridge)
I recommend starting with this project, as it is easier and faster to build while still being enough to get the car working perfectly.

<img src="https://github.com/user-attachments/assets/c935c23b-afe2-46e4-af1d-441b9cdcd834" />

### Option 2 - More complex to build (uses DRV8833 bridge)
If you want to upgrade to a more energy-efficient project with faster motors, and you have time and experience with soldering components, this is a great option.

The DRV8833 uses modern MOSFET transistor and drops around **0.2-0.4v**, while the L298N is based on older bipolar transistor technology and drops around **2-4 V**. This means more voltage reaches the motor, resulting in better performance, less heat, and improved battery efficiency.

<img src="https://github.com/user-attachments/assets/82d02a84-c2a8-43f8-bacd-7ff76472e0b8" />

I could not find any motor shield so I had to design my own PCB. [Download the Gerber file](https://github.com/user-attachments/files/26162524/Gerber_DRV8833-Shield_PCB_DRV8833-Shield_2026-03-21.zip). 


The cost was about 2 USD on [EasyEDA](https://easyeda.com/), with shipping costs varying by country.

<img src="https://github.com/user-attachments/assets/139d6590-9a00-429c-81e6-d10aa59a7e32" width="430">


## Components
* 01 - ESP32S3-CAM board
* 01 - Car Chassis (2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 02 - White Leds
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support
* 01 - (Optional) Antenna for ESP32-CAM board - improves video streaming and prevents lags
* 01 - (Optional) OV2640 ESP32 CAM (120 or 160 degree lens) - wide-angle capture

### Option 1 (L298N)
* 01 - L298N Dual H-Bridge board
* 01 - (Optional) 1000 μF capacitor

### Option 2 (DRV8833)
* 01 - DRV8833 Dual H-Bridge board
* 01 - L2596 Bulk Converter
* 02 - (Optional) 1000 μF capacitor
* 01 - (Optional) 100 nF capacitor

_Note: in order to enable the external antenna, it is required to change resistor position in ESP32S3-CAM board. [See tutorial](https://randomnerdtutorials.com/esp32-cam-connect-external-antenna/)._

## About "PIO Arduino" IDE
<img width="200" height="200" alt="Image" src="https://github.com/user-attachments/assets/e0c681bf-b1d1-4018-8d4e-9fa2566e4ebc" />

This project was created on "Visual Studio Code" with "PIO Arduino" plugin.

Compared to the official Arduino IDE, this setup offers better dependency management, multi-environment (board) support, and a project structure that promotes modular and object-oriented code.

## About the code
The PINs can be customized in the `Config/src/Config.h`
```c++
// WiFi credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PASSWORD"

// Access Point mode 
// + true = creates a WiFi network without internet connection
// + false = connect to an existing WiFi with internet connection
#define WIFI_AP_MODE false

// Customize PINS
#define PIN_M1_IN1 14
#define PIN_M1_IN2 47
#define PIN_M2_IN1 38
#define PIN_M2_IN2 39
#define PIN_FRONT_LED_1 41
#define PIN_FRONT_LED_2 40
#define PIN_CAMERA_LED 48
#define PIN_BOARD_LED 2

// Set minimum motor speed (0 to 255)
#define MIN_MOTOR_SPEED 80

// Set JPEG resolution 
// NOTE: higher resolutions affect stream FPS
#define FRAME_SIZE FRAMESIZE_SVGA

// Set JPEG quality (0 to 63 - lower means higher quality)
// NOTE: Higher quality affect stream FPS
#define JPEG_QUALITY 25

// Enable debug (prints car speed and direction in the serial)
#define JOYSTICK_DEBUG false

// Set camera model
// #define CAMERA_MODEL_AI_THINKER
#define CAMERA_FREENOVE_ESP32S3_CAM
// #define CAMERA_MODEL_XIAO_ESP32S3
```

Fine-tuning customization can be done in the individual files like `DCMotor.h` for changing speed parameters
```c++
#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>

class DCMotor
{
public:
    DCMotor(uint8_t pinIn1, uint8_t pinIn2);
    void backward(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();

private:
    uint8_t pinIn1;
    uint8_t pinIn2;
    uint8_t absSpeed = 0;
    uint8_t maxAbsSpeed = 255;
    uint8_t minAbsSpeed = 50;
    uint8_t ignoreAbsSpeed = 30;

    void setSpeed(uint8_t speed);
};
#endif
```
## About Car Chassis
This project can work with a 2WD or 4WD car chassis like these ones:

### Reference 1 (Brazil):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/a79de4b0-ea64-42f6-b933-a801766820ab" width="300">

https://www.robocore.net/robotica-robocore/plataforma-robotica-rocket-tank

### Reference 2 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/6e92b8e4-c452-4b1f-b8ce-b5e51e591bed" width="300">

https://www.aliexpress.us/item/3256801542172576.html

### Reference 3 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/ab262906-3d15-41b7-8fab-b9664d6263a4" width="300">

https://www.aliexpress.us/item/3256805855273192.html

### About the Power Supply
I recommend to use high quality 18650 batteries (3.7v - 4.2v, 2200mAh, at least 2C of discharge rate).

Most people prefer to use different power sources for ESP32-CAM (3.3v or 5v) and Bridge driver (7.4 - 8.4v).

I prefer to have a single power source and thus a single power switch. However, it is recommended to use capacitor to filter the electrical noise created by the bridge.

## Fritzing file
The eletronic schematic was created in the [Fritzing](https://fritzing.org/) software and can be downloaded at
* [esp32cam-rc-car-v4.zip](https://github.com/user-attachments/files/26162509/esp32cam-rc-car-v4.zip)