# ESP32S3-CAM RC Car
A remote-controlled car built on an ESP32-S3-based platform that can be controlled by mouse, keyboard or a gamepad!

<img width="380"  src="https://github.com/user-attachments/assets/9c87027a-5927-40b4-9232-bff47f240cf2" />
<img width="380" src="https://github.com/user-attachments/assets/b1e3027e-ce6b-45fb-86c2-441278e4adde" />
<img width="380" src="https://github.com/user-attachments/assets/1b35ae77-7d77-4a7a-8023-47e0785e5f5a" />
<img width="380"  src="https://github.com/user-attachments/assets/a186ef4f-2b01-4508-85e9-974050c4f88f" />


## Recording
Note: enable the sound by clicking on the speaker icon from the video toolbar.

https://github.com/user-attachments/assets/d405cac1-93c0-4656-b3de-b336b7a0a3e5

https://github.com/user-attachments/assets/688e1cb7-99dc-43c0-b13c-eb8923ff54a2


## Description
This project is the successor to the [ESP32CAM-RC-Car project](https://github.com/vitorccs/esp32cam-rc-car), which was based on a previous-generation ESP32 board.

The ESP32-S3 is a more powerful microcontroller that delivers smooth, low-latency video streaming.

Also, this project uses the DRV8833 as a motor driver, improving power efficiency and reaching higher speeds.

## Control methods
### Mouse
By default, the car is controlled through a web page featuring a virtual joystick.

<img width="350" src="https://github.com/user-attachments/assets/90edd39e-dc98-4863-8b63-a6506ae2d469" />

<img src="https://github.com/user-attachments/assets/54f7b778-2dab-4138-b2e3-d2f6ab629eb7" />

### Keyboard
To use the keyboard, press the `Enter` key at any time. Press it again to switch back to the virtual joystick. 

+ Enable/Disable: `Enter`
+ Left/Right: `A`/`D`, or `←`/`→` 
+ Up: `W` or `↑`
+ Reverse: `S` or `↓`
+ Camera: `P` or `C`
+ Front LEDs: `L` or `F`

<img src="https://github.com/user-attachments/assets/50c9a7cd-da9c-40cc-8e1c-d85b99e771db" />

### Gamepad
To use a USB PlayStation 3 compatible gamepad, press the `Start` button at any time. Press it again to switch back to the virtual joystick. 

+ Enable/Disable: `Start`
+ Left/Right: `left analog`
+ Up: `R2`
+ Reverse: `L2`
+ Camera: `△`
+ Front LEDs: `□`

<img src="https://github.com/user-attachments/assets/7077b9c3-e8f3-4759-9333-1216187b7d69" />

## Remote control
If you are familiar with configuring internet routers and setting port forwarding, you can also control your car remotely!
1) Ensure you assign a static IP.
2) Set up the following port forwarding rules:
* 8000 (web page)
* 8001 (video streaming)
* 8002 (WebSocket / joystick commands).
3) If your ISP changes your public IP frequently, set up a DDNS service (e.g., No-IP)

## Compatible boards
### Freenove ESP32S3-CAM
This board is more affordable than "XIAO ESP32-S3" board and has higher-quality assembly and soldering than "AI Thinker style" board below.

Also, it has a built-in USB programmer so you don't need an external USB programmer to flash it.

In the PIO Arduino, use the env "esp32s3cam".

<img src="https://github.com/user-attachments/assets/40393a1c-cbb9-4f6d-8d1a-0267f5819d62" width="330"/>


### ESP32S3-CAM ("AI Thinker" style)
The "ESP32S3-CAM" board succeeds the "AI Thinker" board, which was based on the older ESP32. This board can be found at [AliExpress](http://aliexpress.com/w/wholesale-esp32s3cam.html). 

NOTE: there are slightly different models and they may have different GPIO numbers. The project was based on this model. There is also an RGB LED in the position which used to be a bright FLASH LED.

In the PIO Arduino, use the env "esp32s3cam". If you want to use the previous model "AI Thinker" (ESP32-S), use "ai_tinker".

<img src="https://github.com/user-attachments/assets/d590d683-830d-4006-8bae-eb9d859a37c2" width="330"/>

### Seeed Studio XIAO ESP32-S3 Series
It works perfectly in this project. It is more expensive but it is very compact and an externall antenna can be easily attached. 

In the PIO Arduino, use the env "xiaoS3cam".

<img src="https://github.com/user-attachments/assets/08a81881-be41-45f0-be26-7417dd8f78cb" width="330"/>

## Schematics

### Option 1 - Easier to build (uses L298N bridge)
I recommend starting with this project, as it is easier and faster to build while still being enough to get the car working perfectly.

<img src="https://github.com/user-attachments/assets/a36c4f15-d758-4922-ada2-4171e4fb5c58" />

### Option 2 - More complex to build (uses DRV8833 bridge)
If you want to upgrade to a more energy-efficient project with faster motors, and you have time and experience with soldering components, this is a great option.

The DRV8833 uses modern MOSFET transistor and drops around **0.2-0.4v**, while the L298N is based on older bipolar transistor technology and drops around **2-4 V**. This means more voltage reaches the motor, resulting in better performance, less heat, and improved battery efficiency.

<img src="https://github.com/user-attachments/assets/0e749e5d-7006-4e99-8de4-eb949d3b2de9" />

I could not find any motor shield so I had to design my own PCB. [Download the Gerber file](https://github.com/user-attachments/files/26162524/Gerber_DRV8833-Shield_PCB_DRV8833-Shield_2026-03-21.zip). 


The cost was about 2 USD on [EasyEDA](https://easyeda.com/), with shipping costs varying by country.

<img src="https://github.com/user-attachments/assets/139d6590-9a00-429c-81e6-d10aa59a7e32" width="430">


## Components
* 01 - ESP32S3-CAM board
* 01 - Car Chassis (2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 02 - [High Brightness White Leds (5v, 1 Watt)](https://pt.aliexpress.com/w/wholesale-led-5v-1w-star.html)
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support
* 01 - [MOSFET module (LR7843 or AOD4184)](https://pt.aliexpress.com/w/wholesale-mosfet-module.html)
* 01 - (Optional) OV2640 camera (120 or 160 degree lens) - wide-angle capture
* 02 - (Optional) 1000 μF capacitor
* 02 - (Optional) 100 nF capacitor

### Option 1 (L298N)
* 01 - L298N Dual H-Bridge board

### Option 2 (DRV8833)
* 01 - DRV8833 Dual H-Bridge board
* 01 - L2596 Bulk Converter

## About "PIO Arduino" IDE
<img width="200" height="200" alt="Image" src="https://github.com/user-attachments/assets/e0c681bf-b1d1-4018-8d4e-9fa2566e4ebc" />

This project was created on "Visual Studio Code" with "PIO Arduino" plugin.

Compared to the official Arduino IDE, this setup offers better dependency management, multi-environment (board) support, and a project structure that promotes modular and object-oriented code.

## About the code
The parameters and PINs can be set in the file `Config/src/Config.h`
```c++
// WiFi credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PASSWORD"

// Access Point mode 
// + true = creates a WiFi network without internet connection
// + false = connect to an existing WiFi with internet connection
#define WIFI_AP_MODE false

// Set camera model
#define CAMERA_FREENOVE_ESP32S3_CAM
// #define CAMERA_MODEL_XIAO_ESP32S3
// #define CAMERA_MODEL_AI_THINKER

// Set JPEG quality (0 to 63 - lower means higher quality)
// NOTE: Higher image quality reduces CPU usage for compression.
#define JPEG_QUALITY 12

// Customize PINS
#if defined(CAMERA_FREENOVE_ESP32S3_CAM)
#define PIN_M1_IN1 42
#define PIN_M1_IN2 41
#define PIN_M2_IN1 40
#define PIN_M2_IN2 39
#define PIN_FRONT_LED 3
#define PIN_RGB_LED 48
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_VGA

#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
#define PIN_M1_IN1 1
#define PIN_M1_IN2 2
#define PIN_M2_IN1 3
#define PIN_M2_IN2 4
#define PIN_FRONT_LED 9
#define PIN_BOARD_LED 7
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_VGA

#elif defined(CAMERA_MODEL_AI_THINKER)
#define PIN_M1_IN1 14
#define PIN_M1_IN2 15
#define PIN_M2_IN1 12
#define PIN_M2_IN2 13
#define PIN_FRONT_LED 3
#define PIN_BOARD_LED 33
// NOTE: higher resolutions affect framerate
#define FRAME_SIZE FRAMESIZE_VGA

#else
#error "Camera model not selected"

#endif
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
* [esp32s3cam-rc-car.zip](https://github.com/user-attachments/files/27463789/esp32s3cam-rc-car.zip)