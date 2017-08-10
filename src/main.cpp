/**
 * Client Widget for NodeMCU 1.0 | Receptor 4
 *
 * This is Receptor-4 (Personal Dashboard Service) client program.
 * Before upload firmware to your device, must configure wifi settings in `settings.h`.
 *
 * If you need detailed information, please refer to the following URL.
 *
 * http://receptor-4.appspot.com/doc/diy-esp8266-widget-guide.en.html
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <SSD1306.h>
#include <user_interface.h>

#include "settings.h"
#include "service.h"
#include "ui.h"

const int PIN_SDA = 4;
const int PIN_SCL = 5;
const int I2C_ADDR = 0x3c;
const int LED_ERR = LED_BUILTIN;

SSD1306 display(I2C_ADDR, PIN_SDA, PIN_SCL);
OLEDDisplayUi ui (&display);
OLEDDisplayUiState* state = ui.getUiState();
Service svc (ENDPOINT_URL);
os_timer_t updateTimer;
volatile bool expired;
int errors;
int blocking;


void render(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) {
  Card c = svc.getItem(state->currentFrame);

  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(x + DISPLAY_WIDTH, y + 0, c.label);

  display->setFont(ArialMT_Plain_24);
  display->drawString(x + DISPLAY_WIDTH, y + 10 + 2, c.value);

  display->setFont(ArialMT_Plain_10);
  display->drawString(x + DISPLAY_WIDTH, y + 34 + 4, c.summary);
}
FrameCallback frames[] = {render};


void connectWiFi() {
  WiFi.printDiag(Serial);
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());

  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);

  if (STATIC_IP) WiFi.config(IP, GATEWAY, SUBNET, DNS);

  WiFi.begin(SSID, PASSWORD);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void configureUi() {
  ui.setTargetFPS(60);
  ui.setActiveSymbol(ANIMATION_activeSymbol);
  ui.setInactiveSymbol(ANIMATION_inactiveSymbol);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.enableSingleFrameFunction(true);
  ui.setFrames(frames, 1);
}


void interrupted(void* arg) {
  expired = true;
}


bool update() {
  Serial.println("");
  Serial.println("Loading data...");

  // network available?
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed: Network connection unavailable.");
    return false;
  }

  int size = svc.getSize();

  // fetch success?
  if (!svc.fetch()) {
    Serial.print("Failed: ");
    Serial.println(svc.getStatusMessage());
    return false;
  }

  Serial.print("Success: ");
  Serial.print(svc.getSize());
  Serial.println(" cards collected.");

  // collection size change?
  if (size != svc.getSize()) {
    ui.setFrames(frames, svc.getSize() ? svc.getSize() : 1);
    Serial.println("Collection update was detected.");
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  pinMode(LED_ERR, OUTPUT);

  connectWiFi();
  configureUi();

  ui.init();
  if (FLIP_SCREEN_VERTICALLY) {
    display.flipScreenVertically();
  }

  // start timer
  os_timer_setfn(&updateTimer, (os_timer_func_t*) &interrupted, NULL);
  os_timer_arm(&updateTimer, UPDATE_INTERVAL_MS, true);

  expired = true;
}

void loop() {

  if (expired && blocking < 1) {
    if (update()) {
      digitalWrite(LED_ERR, HIGH);
      expired = false;
      errors = blocking = 0;
    } else {
      digitalWrite(LED_ERR, LOW);
      blocking = (errors < 10) ? pow(2, errors) * 1000 + random(1000) : 600000;
      errors++;
    }
  }

  int remainingTimeBudget = ui.update();
  if (remainingTimeBudget > 0) {
    delay(remainingTimeBudget);
  }
  delay(10);
  if (blocking > 0) {
    blocking -= remainingTimeBudget + 10;
  }
}
