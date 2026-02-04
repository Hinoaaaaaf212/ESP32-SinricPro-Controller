/*
 * ESP32 + WS2812B + Sinric Pro (Google Home)
 * 
 * DEPENDENCIES (Install via Library Manager):
 * 1. SinricPro by Sinric Pro
 * 2. FastLED by Daniel Garcia
 */

#include <Arduino.h>
#include <WiFi.h> // Changed from ESP8266WiFi.h for ESP32
#include "SinricPro.h"
#include "SinricProLight.h"
#include <FastLED.h>

// -----------------------------------------------------------------------------
// USER CONFIGURATION (EDIT THESE)
// -----------------------------------------------------------------------------

#define WIFI_SSID         "WIFI_SSID"
#define WIFI_PASS         "WIFI_PASS"

#define APP_KEY           "APP_KEY"      // from https://portal.sinric.pro
#define APP_SECRET        "APP_SECRET"   // from https://portal.sinric.pro
#define LIGHT_ID          "LIGHT_ID"               // from https://portal.sinric.pro

// LED CONFIGURATION
#define LED_PIN           4        // GPIO 4. (Check your board! On NodeMCU-32S, D4 might be GPIO 4, or D2 might be GPIO 2)
#define NUM_LEDS          40       // How many LEDs on your strip?
#define LED_TYPE          WS2812B
#define COLOR_ORDER       GRB      // Try RGB if colors are swapped

// -----------------------------------------------------------------------------

#define BAUD_RATE         115200

CRGB leds[NUM_LEDS];
bool powerState = false;
int globalBrightness = 255; // 0-255

// --- Sinric Pro Callbacks ---

bool onPowerState(const String &deviceId, bool &state) {
  powerState = state;
  if (powerState) {
    FastLED.setBrightness(globalBrightness);
  } else {
    FastLED.setBrightness(0);
  }
  FastLED.show();
  Serial.printf("Device %s turned %s\r\n", deviceId.c_str(), state ? "on" : "off");
  return true; // request handled properly
}

bool onBrightness(const String &deviceId, int &brightness) {
  // SinricPro gives 0-100, FastLED needs 0-255
  globalBrightness = map(brightness, 0, 100, 0, 255);
  FastLED.setBrightness(globalBrightness);
  FastLED.show();
  Serial.printf("Device %s brightness level changed to %d\r\n", deviceId.c_str(), brightness);
  return true;
}

bool onAdjustBrightness(const String &deviceId, int brightnessDelta) {
  // brightnessDelta is relative (-30 or +30)
  globalBrightness += map(brightnessDelta, -100, 100, -255, 255);
  globalBrightness = constrain(globalBrightness, 0, 255);
  FastLED.setBrightness(globalBrightness);
  FastLED.show();
  Serial.printf("Device %s brightness changed by %d to %d\r\n", deviceId.c_str(), brightnessDelta, globalBrightness);
  return true;
}

bool onColor(const String &deviceId, byte &r, byte &g, byte &b) {
  fill_solid(leds, NUM_LEDS, CRGB(r, g, b));
  FastLED.show();
  Serial.printf("Device %s color changed to RGB(%d, %d, %d)\r\n", deviceId.c_str(), r, g, b);
  return true;
}

// --- Setup & Loop ---

void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  Serial.printf("connected!\r\n[Wifi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  SinricProLight &myLight = SinricPro[LIGHT_ID];

  // Set callbacks
  myLight.onPowerState(onPowerState);
  myLight.onBrightness(onBrightness);
  myLight.onAdjustBrightness(onAdjustBrightness);
  myLight.onColor(onColor);

  // Setup SinricPro connection
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(BAUD_RATE);

  // Setup LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(globalBrightness);
  
  // --- STARTUP TEST SEQUENCE ---
  // Cycle R-G-B to confirm wiring is correct
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(500);
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(500);
  // -----------------------------

  // Clear LEDs on startup
  fill_solid(leds, NUM_LEDS, CRGB::Black);
  FastLED.show();

  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
