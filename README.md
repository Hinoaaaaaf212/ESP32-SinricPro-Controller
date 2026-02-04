# ESP32 Sinric Pro WS2812B Controller

This repository contains firmware for controlling WS2812B addressable LED strips using an ESP32 microcontroller. It utilizes the Sinric Pro IoT cloud platform to enable integration with smart home ecosystems such as Google Home and Amazon Alexa.

## Features

*   **Smart Home Integration**: Native support for Google Home via Sinric Pro.
*   **Power Control**: Toggle LEDs on and off remotely.
*   **Brightness Control**: Adjust brightness levels through the app or voice commands.
*   **Color Control**: Change LED colors using the standard color picker or voice commands.
*   **Visual Startup Test**: Cycles Red, Green, and Blue on boot to verify hardware connections.
*   **Wi-Fi Connectivity**: Connects directly to standard 2.4GHz Wi-Fi networks.

## Hardware Requirements

*   **Microcontroller**: ESP32 Development Board (e.g., ESP32 DevKit V1, NodeMCU-32S).
*   **LED Strip**: WS2812B (Neopixel) addressable RGB LEDs.
*   **Power Supply**:
    *   For short strips (<30 LEDs), USB power via the ESP32 is generally sufficient.
    *   For longer strips, an external 5V power supply is required, with shared ground.
*   **Connections**:
    *   Strip VCC -> 5V / VIN
    *   Strip GND -> GND
    *   Strip Data -> GPIO 4 (D4 on some boards, but verify pinout)

## Software Dependencies

This project is built using the Arduino IDE. The following libraries must be installed via the Library Manager:

1.  **SinricPro** by Sinric Pro
2.  **FastLED** by Daniel Garcia
3.  **ArduinoJson** (Installed automatically with SinricPro)
4.  **WebSockets** (Installed automatically with SinricPro)

## Configuration

Before flashing, the `SinricPro_WS2812B.ino` file requires configuration. Open the file and modify the following defines:

```cpp
#define WIFI_SSID         "YOUR_WIFI_SSID"
#define WIFI_PASS         "YOUR_WIFI_PASSWORD"

#define APP_KEY           "YOUR_SINRIC_PRO_APP_KEY"
#define APP_SECRET        "YOUR_SINRIC_PRO_APP_SECRET"
#define LIGHT_ID          "YOUR_DEVICE_ID"

#define LED_PIN           4
#define NUM_LEDS          40
```

Credentials can be obtained from the [Sinric Pro Portal](https://portal.sinric.pro).

## Installation

1.  Clone this repository or download the source code.
2.  Open `SinricPro_WS2812B.ino` in the Arduino IDE.
3.  Select the correct board (e.g., "ESP32 Dev Module") under **Tools > Board**.
4.  Connect the ESP32 via USB.
5.  Select the correct COM port.
6.  Click **Upload**.

*Note: Some ESP32 boards require holding the BOOT button when the "Connecting..." message appears during upload.*

## License

This project is open source and available under the MIT License.
