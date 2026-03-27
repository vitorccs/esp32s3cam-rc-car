#include <Arduino.h>
#include <esp_http_server.h>
#include <esp_camera.h>
#include <esp_timer.h>
#include <img_converters.h>
#include <fb_gfx.h>
#include <StreamServer.h>
#include <Config.h>
#include <CameraPins.h>

httpd_handle_t camera_httpd = NULL;
httpd_handle_t stream_httpd = NULL;

#define TAG 1
#define PART_BOUNDARY "123456789000000000000987654321"

static const char *_STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char *_STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char *_STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>

<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
<title>JoyStick</title>
<style>
/* global */
*,
::after,
::before {
  box-sizing: content-box;
}

html,
body {
  padding: 0;
  margin: 0;
}

body {
  width: 100%;
  height: 100vh;
  overflow: hidden;
  font-family: 'Roboto', 'Tahoma', 'Arial', sans-serif;
  background: #6c8bee;
}

button {
  background: radial-gradient(ellipse at center, #444 0%, #222 100%);
  border: 1px solid #222;
  color: #fff;
  padding: 1rem clamp(1rem, 6vh, 3rem);
  font-size: clamp(1rem, 2.5vw, 2rem);
  border-radius: .4rem;
  user-select: none;
}

button.pressed {
  background: #444;
  box-shadow: inset 3px 1px 7px 4px #222;
}

/* debug */
.debug {
  position: absolute;
  background-color: rgba(0, 0, 0, .6);
  color: #ccc;
  bottom: 0;
  right: 0;
  margin: 0 auto;
  padding: .4rem .5rem;
}

/* container */
.container {
  display: flex;
  width: 100%;
  height: 100%;
  flex-direction: column;
  align-items: center;
}

/* container elements */
.container .video {
  background: #111;
  width: 100%;
}

.container .video img {
  width: 100%;
  min-width: 350px;
  aspect-ratio: 4 / 3;
}

.container .joystick-container,
.container .keyboard-container {
  display: flex;
  justify-content: center;
  width: 100%;
  border: 0px solid red;
}

.container .buttons {
  display: flex;
  border: 0px solid green;
  flex-direction: row;
  padding: 0 2rem;
  justify-content: end;
  gap: .5rem;
}

.container .joystick-container #joystick-canvas {
  width: 70%;
  aspect-ratio: 1/1;
  cursor: pointer;
}

/* buttons container */
.container .buttons button {
  max-width: 220px;
}

.container .buttons button:hover {
  cursor: pointer;
  background: radial-gradient(ellipse at center, #444 0%, #111 100%);
}

.container .buttons button.pressed {
  color: yellow;
}

/* WASD */
.container .keyboard-container {
  display: none;
  width: auto;
  margin: 4rem;
  flex-direction: column;
  align-items: center;
}

.container .keyboard-container .wasd {
  display: grid;
  grid-template-columns: repeat(3, 70px);
  grid-template-rows: repeat(2, 70px);
  gap: 10px;
  width: max-content;
}

.container .keyboard-container .wasd .key {
  width: 70px;
  height: 70px;
  border: 3px solid #333;
  background: transparent;
  box-sizing: border-box;
  display: flex;
  align-items: center;
  justify-content: center;
  font-family: Arial, sans-serif;
  font-size: 28px;
  font-weight: bold;
  color: #333;
  border-radius: 1rem;
}

.container .keyboard-container .wasd .key.active {
  background-color: #dcdcd6;
}

.container .keyboard-container .max-speed {
  font-size: 1.3rem;
  color: #333;
  font-weight: bold;
  margin-top: 3rem;
  text-align: center;
  text-transform: uppercase;
}

.container .keyboard-container .max-speed b {
  font-size: 1.8rem;
  color: #222;
}

@media (min-height: 750px) {
  .container .buttons {
    flex-direction: column;
  }
}

@media (min-width: 650px) {
  .container {
    align-items: center;
    flex-direction: row;
  }

  .container .buttons {
    flex-direction: column;
    gap: 1rem;
  }

  .container .video {
    order: 2;
  }

  .container .container-joystick {
    order: 1;
  }

  .container .buttons {
    order: 3;
  }

  .container .video img {
    min-width: 300px;
  }

  .container .container-joystick #joystick-canvas {
    width: 100%;
  }
}

@media (min-width: 750px) {
  .container .video img {
    min-width: 350px;
  }
}

@media (min-width: 800px) {
  .container .container-joystick {
    width: 80%;
  }
}

@media (min-width: 1000px) {
  .container .buttons {
    padding: 0 4rem;
  }
}
</style>
<script>
/*
* Name          : joy.js
* @author       : Roberto D'Amico (Bobboteck)
* Last modified : 09.06.2020
* Revision      : 1.1.6
* Source        : https://github.com/bobboteck/JoyStick
* License       : The MIT License (MIT)
*/
let StickStatus = { xPosition: 0, yPosition: 0, x: 0, y: 0, cardinalDirection: "C" }; var JoyStick = function (t, e, i) { var o = void 0 === (e = e || {}).title ? "joystick" : e.title, n = void 0 === e.width ? 0 : e.width, a = void 0 === e.height ? 0 : e.height, r = void 0 === e.internalFillColor ? "#00AA00" : e.internalFillColor, s = void 0 === e.internalLineWidth ? 2 : e.internalLineWidth, c = void 0 === e.internalStrokeColor ? "#003300" : e.internalStrokeColor, $ = void 0 === e.externalLineWidth ? 2 : e.externalLineWidth, d = void 0 === e.externalStrokeColor ? "#008000" : e.externalStrokeColor, u = void 0 === e.autoReturnToCenter || e.autoReturnToCenter; i = i || function (t) { }; var h = document.getElementById(t); h.style.touchAction = "none"; var S = document.createElement("canvas"); S.id = o, 0 === n && (n = h.clientWidth), 0 === a && (a = h.clientHeight), S.width = n, S.height = a, h.appendChild(S); var f = S.getContext("2d"), l = 0, k = 2 * Math.PI, _ = (S.width - (S.width / 2 + 10)) / 2, g = _ + 5, x = _ + .6 * _, v = S.width / 2, P = S.height / 2, C = S.width / 10, p = -1 * C, y = S.height / 10, w = -1 * y, L = v, F = P; function m() { f.beginPath(), f.arc(v, P, x, 0, k, !1), f.lineWidth = $, f.strokeStyle = d, f.stroke() } function E() { f.beginPath(), L < _ && (L = g), L + _ > S.width && (L = S.width - g), F < _ && (F = g), F + _ > S.height && (F = S.height - g), f.arc(L, F, _, 0, k, !1); var t = f.createRadialGradient(v, P, 5, v, P, 200); t.addColorStop(0, r), t.addColorStop(1, c), f.fillStyle = t, f.fill(), f.lineWidth = s, f.strokeStyle = c, f.stroke() } function W(t) { l = 1 } function T(t) { l = 1 } function D() { let t = "", e = L - v, i = F - P; return i >= w && i <= y && (t = "C"), i < w && (t = "N"), i > y && (t = "S"), e < p && ("C" === t ? t = "W" : t += "W"), e > C && ("C" === t ? t = "E" : t += "E"), t } "ontouchstart" in document.documentElement ? (S.addEventListener("touchstart", W, !1), document.addEventListener("touchmove", function t(e) { 1 === l && e.targetTouches[0].target === S && (L = e.targetTouches[0].pageX, F = e.targetTouches[0].pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("touchend", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)) : (S.addEventListener("mousedown", T, !1), document.addEventListener("mousemove", function t(e) { 1 === l && (L = e.pageX, F = e.pageY, "BODY" === S.offsetParent.tagName.toUpperCase() ? (L -= S.offsetLeft, F -= S.offsetTop) : (L -= S.offsetParent.offsetLeft, F -= S.offsetParent.offsetTop), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus)) }, !1), document.addEventListener("mouseup", function t(e) { l = 0, u && (L = v, F = P), f.clearRect(0, 0, S.width, S.height), m(), E(), StickStatus.xPosition = L, StickStatus.yPosition = F, StickStatus.x = (100 * ((L - v) / g)).toFixed(), StickStatus.y = (-(100 * ((F - P) / g) * 1)).toFixed(), StickStatus.cardinalDirection = D(), i(StickStatus) }, !1)), m(), E(), this.GetWidth = function () { return S.width }, this.GetHeight = function () { return S.height }, this.GetPosX = function () { return L }, this.GetPosY = function () { return F }, this.GetX = function () { return (100 * ((L - v) / g)).toFixed() }, this.GetY = function () { return (-(100 * ((F - P) / g) * 1)).toFixed() }, this.GetDir = function () { return D() } };
</script>
</head>

<body>
<section class="debug"></section>

<section class="container">
  <div class="video">
    <img id="stream" src="">
  </div>
  <div class="joystick-container">
    <div id="joystick-canvas"></div>
  </div>
  <div class="keyboard-container">
    <div class="wasd">
      <div></div>
      <div class="key" data-key="up">W</div>
      <div></div>

      <div class="key" data-key="left">A</div>
      <div class="key" data-key="down">S</div>
      <div class="key" data-key="right">D</div>
    </div>
    <div class="max-speed">
      Max Speed <b></b>
    </div>
  </div>
  <div class="buttons">
    <button type="button" id="button-a">FLASH</button>
    <button type="button" id="button-b">LEDs</button>
    <button type="button" id="button-c">PHOTO</button>
  </div>
</section>

<script>
class Logger {
  constructor(display = true) {
    this.element = document.querySelector('.debug');

    if (!display) {
      this.element.style.display = 'none';
    }
  }

  debug(strText) {
    this.element.innerHTML = strText;
    console.log(strText);
  }

  fail(strText) {
    this.debug(strText);
    alert(strText);
  }
}

class VideoStream {
  constructor(
    selector,
    port,
    host = null
  ) {
    this.element = document.querySelector(selector);
    this.port = port;
    this.host = host || window.location.hostname;
    this.url = `http://${this.host}:${this.port}/stream`;
    this.init();
  }

  init() {
    this.element.src = this.url;
  }
}

class SocketClient {
  constructor(
    port,
    logger,
    host = null
  ) {
    this.logger = logger;
    this.port = port;
    this.host = host || window.location.hostname;
    this.url = `ws://${this.host}:${this.port}`;
    this.connection = new WebSocket(this.url);

    this.connection.onopen = () => this.onOpen();
    this.connection.onerror = (error) => this.onError(error);
  }

  send(params) {
    const jsonString = this.isReady()
      ? JSON.stringify(params)
      : null;

    const debugMessage = jsonString || 'Socket is not ready';

    if (jsonString !== null) {
      this.connection.send(jsonString);
    }

    this.logger.debug(debugMessage);
  }

  setOnMessage(handler) {
    this.connection.onmessage = (e) => handler(e);
  }

  onOpen() {
    const message = `WebSocket Connection at ${this.url}`;
    this.logger.debug(message);
  }

  isReady() {
    return this.connection.readyState === WebSocket.OPEN;
  }

  onError(error) {
    const message = `WebSocket Error ${error} at ${this.url}`;
    this.logger.fail(message);
  }
}

class PushButton {
  constructor(
    selector,
    callback
  ) {
    this.element = document.querySelector(selector);
    this.callback = callback;
    this.setValue(false);
    this.bindEvent();
  }

  bindEvent() {
    this.element.addEventListener('click', async() => this.press());
  }

  async press() {
    this.setValue(true);
    this.callback();
    this.setValue(false);
  }

  setValue(value) {
    this.value = value;
    this.element.classList.toggle('pressed', value);
  }
}

class ToggleButton {
  constructor(
    selector,
    callback,
    initialValue = false
  ) {
    this.element = document.querySelector(selector);
    this.callback = callback;
    this.setValue(initialValue);
    this.bindEvent();
  }

  bindEvent() {
    this.element.addEventListener('click', () => this.press());
  }

  press() {
    const newValue = !this.value;
    this.setValue(newValue);
    this.callback(newValue);
  }

  setValue(value) {
    this.value = value;
    this.element.classList.toggle('pressed', value);
  }
}

class MessageSender {
  constructor(
    controllerHandler,
    socketClient
  ) {
    this.controllerHandler = controllerHandler;
    this.socketClient = socketClient;
  }

  sendButtonA(value) {
    this.socketClient.send({ 'button-a': value });
  }

  sendButtonB(value) {
    this.socketClient.send({ 'button-b': value });
  }

  sendJoystick(direction, speed) {
    if (!this.controllerHandler.isJoystick()) return;
    this.sendCoords(direction, speed);
  }

  sendKeyboard(direction, speed) {
    if (!this.controllerHandler.isKeyboard()) return;
    this.sendCoords(direction, speed);
  }

  sendGamepad(direction, speed) {
    if (!this.controllerHandler.isGamepad()) return;
    this.sendCoords(direction, speed);
  }

  sendCoords(direction, speed) {
    speed = Math.round(speed)
    this.socketClient.send({ direction, speed });
  }
}

class JoystickController {
  constructor(
    messageSender,
    controllerHandler,
    canvasJoystick
  ) {
    this.messageSender = messageSender;
    this.controllerHandler = controllerHandler;
    this.canvasJoystick = canvasJoystick;
    this.startLoop();
  }

  startLoop() {
    window.setInterval(() => {
      if (!this.controllerHandler.isJoystick()) return;
      this.sendJoystick();
    }, 50);
  }

  sendJoystick() {
    const xAxis = this.canvasJoystick.GetX();
    const yAxis = this.canvasJoystick.GetY();
    const direction = this.canvasJoystick.GetDir();
    const speed = Math.abs(xAxis) > Math.abs(yAxis)
      ? Math.abs(xAxis)
      : Math.abs(yAxis);

    this.messageSender.sendJoystick(direction, speed);
  }
}

class KeyboardController {
  constructor(
    messageSender,
    controllerHandler,
    buttonA,
    buttonB,
    buttonC,
    acceleration,
    deceleration
  ) {
    this.messageSender = messageSender;
    this.controllerHandler = controllerHandler;
    this.buttonA = buttonA;
    this.buttonB = buttonB;
    this.buttonC = buttonC;
    this.acceleration = acceleration;
    this.deceleration = deceleration;

    this.speed = 0;
    this.maxSpeed = 100;
    this.direction = 'C';
    this.keys = {
      up: false,
      down: false,
      left: false,
      right: false
    };

    this.bindEvents();
    this.startLoop();
    this.setMaxSpeed();
  }

  setMaxSpeed(speed = 100) {
    this.maxSpeed = speed;
    const el = document.querySelector(`.keyboard-container .max-speed b`);
    el.textContent = this.maxSpeed;
  }

  bindEvents() {
    window.addEventListener('keydown', (e) => this.handleKey(e, true));
    window.addEventListener('keyup', (e) => this.handleKey(e, false));
  }

  handleKey(e, pressed) {
    const key = e.key.toLowerCase();
    const repeat = e.repeat;

    if (pressed && key === 'enter') {
      this.controllerHandler.toggleKeyboard();
      return;
    }

    if (pressed && key === 'f') {
      this.buttonA.press();
      return;
    }

    if (pressed && key === 'l') {
      this.buttonB.press();
      return;
    }

    if (pressed && key === 'p') {
      this.buttonC.press();
      return;
    }

    switch (key) {
      case "arrowup":
      case "w":
        this.keys.up = pressed;
        this.highlightKey('up', pressed, repeat);
        break;

      case "arrowdown":
      case "s":
        this.keys.down = pressed;
        this.highlightKey('down', pressed, repeat);
        break;

      case "arrowleft":
      case "a":
        this.keys.left = pressed;
        this.highlightKey('left', pressed, repeat);
        break;

      case "arrowright":
      case "d":
        this.keys.right = pressed;
        this.highlightKey('right', pressed, repeat);
        break;
      case "1":
        this.setMaxSpeed(10);
        break;
      case "2":
        this.setMaxSpeed(20);
        break;
      case "3":
        this.setMaxSpeed(30);
        break;
      case "4":
        this.setMaxSpeed(40);
        break;
      case "5":
        this.setMaxSpeed(50);
        break;
      case "6":
        this.setMaxSpeed(60);
        break;
      case "7":
        this.setMaxSpeed(70);
        break;
      case "8":
        this.setMaxSpeed(80);
        break;
      case "9":
        this.setMaxSpeed(90);
        break;
      case "0":
        this.setMaxSpeed(100);
        break;
    }
  }

  highlightKey(key, pressed, repeat) {
    const el = document.querySelector(`.wasd .key[data-key="${key}"]`);

    if (!el) return;

    if (pressed && !repeat) {
      el.classList.add("active");
    } else if (!pressed) {
      el.classList.remove("active");
    }
  }

  startLoop() {
    window.setInterval(() => {
      if (!this.controllerHandler.isKeyboard()) return;
      this.updateSpeed();
      this.updateDirection();
      this.sendKeyboard();
    }, 50);
  }

  updateSpeed() {
    const { up, down, left, right } = this.keys;

    // accelerate if ANY movement key is pressed
    if (up || down || left || right) {
      this.speed += this.acceleration;

      if (this.speed > this.maxSpeed) {
        this.speed = this.maxSpeed;
      }

    } else {
      // decelerate when no keys are pressed
      this.speed -= this.deceleration;

      if (this.speed < 0) {
        this.speed = 0;
      }
    }
  }

  updateDirection() {
    const { up, down, left, right } = this.keys;

    if (!up && !down && !left && !right) {
      this.direction = "C";
      return;
    }

    if (up && left) this.direction = "NW";
    else if (up && right) this.direction = "NE";
    else if (down && left) this.direction = "SW";
    else if (down && right) this.direction = "SE";
    else if (up) this.direction = "N";
    else if (down) this.direction = "S";
    else if (left) this.direction = "W";
    else if (right) this.direction = "E";
  }

  sendKeyboard() {
    this.messageSender.sendKeyboard(this.direction, this.speed);
  }
}

class GamepadController {
  constructor(
    logger,
    messageSender,
    controllerHandler,
    buttonA,
    buttonB,
    buttonC,
    acceleration,
    deceleration
  ) {
    this.logger = logger;
    this.messageSender = messageSender;
    this.controllerHandler = controllerHandler;
    this.buttonA = buttonA;
    this.buttonB = buttonB;
    this.buttonC = buttonC;

    // custom parameters
    this.acceleration = acceleration;
    this.deceleration = deceleration;

    // static parameters
    this.maxSpeed = 100;
    this.hDeadZone = 0.2;
    this.vDeadZone = 0.1;
    this.diagonalThreshold = 0.5;

    // system parameters
    this.speed = 0;
    this.direction = 'C';
    this.lastDirection = 'C';
    this.prevButtons = [];
    this.gamepad = null;

    // ps3-gamepad buttons
    this.buttons = {
      triangle: 0,
      circle: 1,
      cross: 2,
      square: 3,
      l1: 4,
      r1: 5,
      l2: 6,
      r2: 7,
      select: 8,
      start: 9,
      l3: 10,
      r3: 11
    };

    // map buttons
    this.map = {
      activation: this.buttons.start,
      buttonA: this.buttons.square,
      buttonB: this.buttons.cross,
      buttonC: this.buttons.triangle
    }

    this.bindEvents();
    this.startLoop();
  }

  bindEvents() {
    window.addEventListener("gamepadconnected", (e) => {
      this.logger.debug('Gamepad connected');
      this.controllerHandler.setGamepadIndex(e.gamepad.index);
    });

    window.addEventListener("gamepaddisconnected", () => {
      this.logger.debug('Gamepad disconnected');
      this.controllerHandler.setGamepadIndex(null);
    });
  }

  updateGamepad() {
    const index = this.controllerHandler.gamepadIndex;
    this.gamepad = navigator.getGamepads()[index] || null;
  }

  startLoop() {
    window.setInterval(() => {
      this.updateGamepad();
      this.checkButtonsPressed();
      if (!this.controllerHandler.isGamepad()) {
        return;
      }
      this.updateSpeed();
      this.updateDirection();
      this.sendGamepad();
    }, 50);
  }

  checkButtonsPressed() {
    if (!this.gamepad) return;

    Object.values(this.map).forEach((index) => {
      const pressed = this.gamepad.buttons[index].pressed;

      if (this.prevButtons[index] !== pressed) {
        this.prevButtons[index] = pressed;

        if (pressed && index === this.map.activation) {
          this.controllerHandler.toggleGamepad();
        }

        if (pressed && index === this.map.buttonA) {
          this.buttonA.press();
        }

        if (pressed && index === this.map.buttonB) {
          this.buttonB.press();
        }

        if (pressed && index === this.map.buttonC) {
          this.buttonC.press();
        }
      }
    });
  }

  // acceleration (pedal-like)
  updateSpeed() {
    if (!this.gamepad) {
      this.speed = 0;
      return;
    }

    // L2/R2 values
    const up = this.gamepad.buttons[this.buttons.r2].value;
    const down = this.gamepad.buttons[this.buttons.l2].value;

    const hasUp = up > this.vDeadZone;
    const hasDown = down > this.vDeadZone;

    // accelerate if ANY movement key is pressed
    if (hasUp || hasDown) {
      this.speed += this.acceleration;

      if (this.speed > this.maxSpeed) {
        this.speed = this.maxSpeed;
      }

    } else {
      // decelerate when no keys are pressed
      this.speed -= this.deceleration;

      if (this.speed < 0) {
        this.speed = 0;
      }
    }
  }

  updateDirection()
  {
    this.direction = this.getDirection();
    this.lastDirection = this.direction;
  }

  getDirection() {
    if (!this.gamepad) {
      return 'C';
    }

    // L2/R2 values
    const up = this.gamepad.buttons[this.buttons.r2].value;
    const down = this.gamepad.buttons[this.buttons.l2].value;
    
    // left analog x and y position
    const x = this.gamepad.axes[0];
    const y = this.gamepad.axes[1];

    const hasLeft = x < -this.hDeadZone;
    const hasRight = x > this.hDeadZone;
    const hasUp = up > this.vDeadZone;
    const hasDown = down > this.vDeadZone;
    const hasVerticalStick = Math.abs(y) > this.diagonalThreshold;

    // No input
    if (!hasLeft && !hasRight && !hasUp && !hasDown) {
      return 'C';
    }

    // Only vertical (triggers)
    if (!hasLeft && !hasRight) {
      if (hasUp) return 'N';
      if (hasDown) return 'S';
    }

    // Only horizontal (stick)
    if (!hasUp && !hasDown) {
      if (hasLeft || hasRight) return this.lastDirection;
    }

    // Combined (diagonals logic)
    if (hasLeft || hasRight) {
      const horizontal = hasLeft ? 'W' : 'E';

      // Prioritize downward diagonal
      if (hasDown) {
        return 'S' + horizontal; // SW / SE
      }

      // Upward diagonal only if stick indicates
      if (hasVerticalStick && hasUp) {
        return 'N' + horizontal; // NW / NE
      }

      // Otherwise, prioritize horizontal
      return horizontal;
    }

    return 'C';
  }

  sendGamepad() {
    this.messageSender.sendGamepad(this.direction, this.speed);
  }
}

class ControllerHandler {
  constructor(joyStickSelector, wasdSelector) {
    this.gamepadIndex = null;
    this.controller = 'joystick';
    this.joyStickContainer = document.querySelector(joyStickSelector);
    this.wasdContainer = document.querySelector(wasdSelector);

    this.render();
  }

  isGamepad() {
    return this.controller === 'gamepad';
  }

  isJoystick() {
    return this.controller === 'joystick';
  }

  isKeyboard() {
    return this.controller === 'keyboard';
  }

  setGamepad() {
    this.controller = 'gamepad';
  }

  setJoystick() {
    this.controller = 'joystick';
  }

  setKeyboard() {
    this.controller = 'keyboard';
  }

  toggleGamepad() {
    this.controller = this.controller === 'gamepad'
       ? 'joystick'
       : 'gamepad';
    this.render();
  }

  toggleKeyboard() {
    this.controller = this.controller === 'keyboard'
       ? 'joystick'
       : 'keyboard';
    this.render();
  }

  setGamepadIndex(index) {
    this.gamepadIndex = index;
  }

  render() {
    this.joyStickContainer.style.display = this.isJoystick() ? 'flex' : 'none';
    this.wasdContainer.style.display = this.isKeyboard() ? 'flex' : 'none';
  }
}

class PhotoService {

  constructor(
    port,
    host = null
  ) {
    this.port = port;
    this.host = host || window.location.hostname;
    this.url = `http://${this.host}:${this.port}/capture`;
  }

  async capture() {
    // a simple solution to download the picture in the background
    const a = document.createElement('a');
    a.target = '_blank';
    a.href = this.url;
    a.click();
  }
}

window.addEventListener('load', () => {
  // const host = null; // change for debugging remotely
  const host = '192.168.10.194'; // change for debugging remotely
  const enableLog = true;
  const httpPort = 8000;
  const streamPort = 8001;
  const websocketPort = 8002;
  const keyboardAcceleration = 5;
  const keyboardDeceleration = 5;
  const gamepadAcceleration = 5;
  const gamepadDeceleration = 5;

  const canvasJoystick = new JoyStick('joystick-canvas', {
    'internalFillColor': '#444',
    'internalStrokeColor': '#222',
    'externalStrokeColor': '#222'
  });

  const logger = new Logger(enableLog);
  const socketClient = new SocketClient(websocketPort, logger, host);
  const controllerHandler = new ControllerHandler(
    '.joystick-container',
    '.keyboard-container',
  );
  const messageSender = new MessageSender(controllerHandler, socketClient);
  const photoService = new PhotoService(httpPort, host);
  const buttonA = new ToggleButton('#button-a', (value) => messageSender.sendButtonA(value));
  const buttonB = new ToggleButton('#button-b', (value) => messageSender.sendButtonB(value));
  const buttonC = new PushButton('#button-c', async() => await photoService.capture());

  new JoystickController(
    messageSender,
    controllerHandler,
    canvasJoystick
  );

  new KeyboardController(
    messageSender,
    controllerHandler,
    buttonA,
    buttonB,
    buttonC,
    keyboardAcceleration,
    keyboardDeceleration
  );

  new GamepadController(
    logger,
    messageSender,
    controllerHandler,
    buttonA,
    buttonB,
    buttonC,
    gamepadAcceleration,
    gamepadDeceleration
  );

  new VideoStream(
    '#stream',
    streamPort,
    host
  );
});
</script>

</body>
</html>
)rawliteral";

void StreamServer::init(framesize_t frameSize,
                        int jpegQuality)
{
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    config.fb_location = CAMERA_FB_IN_PSRAM;
    config.frame_size = frameSize;
    config.jpeg_quality = jpegQuality;
    config.fb_count = 1;

    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

esp_err_t StreamServer::index_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");
    return httpd_resp_send(req, (const char *)INDEX_HTML, strlen(INDEX_HTML));
}

esp_err_t StreamServer::stream_handler(httpd_req_t *req)
{
    camera_fb_t * fb = NULL;
    esp_err_t res = ESP_OK;
    size_t jpg_buf_len = 0;
    uint8_t * jpg_buf = NULL;
    char part_buf[64];
    static int64_t last_frame = 0;
    if(!last_frame) {
        last_frame = esp_timer_get_time();
    }

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    while(true){
        fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(TAG, "Camera capture failed");
            res = ESP_FAIL;
            break;
        }
        if(fb->format != PIXFORMAT_JPEG){
            bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
            if(!jpeg_converted){
                ESP_LOGE(TAG, "JPEG compression failed");
                esp_camera_fb_return(fb);
                res = ESP_FAIL;
                break;
            }
        } else {
            jpg_buf_len = fb->len;
            jpg_buf = fb->buf;
        }

        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }
        if(res == ESP_OK){
            int hlen = snprintf(part_buf, sizeof(part_buf), _STREAM_PART, jpg_buf_len);
            if(hlen < 0 || hlen >= sizeof(part_buf)){
                ESP_LOGE(TAG, "Header truncated (%d bytes needed >= %zu buffer)",
                         hlen, sizeof(part_buf));
                res = ESP_FAIL;
            } else {
                res = httpd_resp_send_chunk(req, part_buf, (size_t)hlen);
            }
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_buf_len);
        }
        if(fb->format != PIXFORMAT_JPEG){
            free(jpg_buf);
        }
        esp_camera_fb_return(fb);
        if(res != ESP_OK){
            break;
        }
        int64_t fr_end = esp_timer_get_time();
        int64_t frame_time = fr_end - last_frame;
        last_frame = fr_end;
        frame_time /= 1000;
        float fps = frame_time > 0 ? 1000.0f / (float)frame_time : 0.0f;
        ESP_LOGI(TAG, "MJPG: %uKB %ums (%.1ffps)",
            (uint32_t)(jpg_buf_len/1024),
            (uint32_t)frame_time, fps);

        vTaskDelay(1);
    }

    last_frame = 0;
    return res;
}

esp_err_t StreamServer::capture_handler(httpd_req_t *req)
{
    camera_fb_t *fb = esp_camera_fb_get();

    if (!fb)
    {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    // Set response type
    httpd_resp_set_type(req, "image/jpeg");

    // force download instead of preview
    char filename[40];
    snprintf(filename, sizeof(filename), "attachment; filename=capture_%lu.jpg", millis());
    httpd_resp_set_hdr(req, "Content-Disposition", filename);

    // Send image buffer
    esp_err_t res = httpd_resp_send(req, (const char *)fb->buf, fb->len);

    esp_camera_fb_return(fb);

    return res;
}

void StreamServer::startStream()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.server_port = 8000;

    httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = StreamServer::index_handler,
        .user_ctx = NULL
    };
    
    httpd_uri_t capture_uri = {
        .uri = "/capture",
        .method = HTTP_GET,
        .handler = StreamServer::capture_handler,
        .user_ctx = NULL
    };

    if (httpd_start(&camera_httpd, &config) == ESP_OK)
    {
        httpd_register_uri_handler(camera_httpd, &index_uri);
        httpd_register_uri_handler(camera_httpd, &capture_uri);
    }

    config.server_port = 8001;
    config.ctrl_port += 1;

    httpd_uri_t stream_uri = {
        .uri = "/stream",
        .method = HTTP_GET,
        .handler = StreamServer::stream_handler,
        .user_ctx = NULL};

    if (httpd_start(&stream_httpd, &config) == ESP_OK)
    {
        httpd_register_uri_handler(stream_httpd, &stream_uri);
    }
}
