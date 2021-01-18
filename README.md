# FastLED Server

LEDServer makes controlling FastLED's remotely easy.

Quickly drop in effects with parameters and control completely over Wi-Fi! Set and forget.

* PlatformIO-based for instant deployment on ESP32. Total project cost <$50
* OTA for remote updates and scalability
* Wi-Fi login server for automatic configuration and security
* Hostname connection for ease-of-access
* HTML5 AsyncWebServer, powered by Boostrap, for custom UI creation

## Hardware Required

* <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a>
* <a href="https://www.amazon.com/BTF-LIGHTING-Flexible-Individually-Addressable-Non-waterproof/dp/B01CDTEJBG">LEDs</a>

## Software Required

<a href="https://github.com/platformio/platformio-core">PlatformIO</a>

## Installation

See [`QUICKSTART.md`](QUICKSTART.md).

## Core Libraries

* <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ArduinoOTA">Arduino OTA</a>
* <a href="https://github.com/espressif/arduino-esp32/tree/master/libraries/ESPmDNS">ESPmDNS</a>
* <a href="https://github.com/me-no-dev/ESPAsyncWebServer">ESPAsyncWebServer</a>
* <a href="https://github.com/FastLED/FastLED">FastLED</a>
* <a href="https://github.com/tzapu/WiFiManager/">WiFiManager</a>
