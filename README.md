# FastLED Server

LEDServer makes controlling LED's remotely easy.

Quickly drop in effects with parameters and control completely over Wi-Fi! Set and forget.

* PlatformIO-based for instant deployment on ESP32. Total project cost <$30
* OTA for remote updates and scalability
* Wi-Fi login server for automatic configuration and security
* Hostname connection for ease-of-access
* HTML5 AsyncWebServer, powered by Boostrap, for custom UI creation

## Setup

### Hardware

* <a href="https://www.mouser.com/ProductDetail/Espressif-Systems/ESP32-DevKitC-32UE/?qs=GedFDFLaBXFguOYDKoZ3jA%3D%3D">ESP32</a>
* <a href="https://www.amazon.com/BTF-LIGHTING-Flexible-Individually-Addressable-Non-waterproof/dp/B01CDTEJBG">LEDs</a>

### Prereqs

<a href="https://github.com/platformio/platformio-core">PlatformIO</a>

### Installation

Start by cloning the repository

```sh 
git clone ttps://github.com/JimothyJohn/LEDServer.git
```

Update src/main.cpp LED parameters

```cpp
// Line 19
// Change to your ESP pin number
#define LED_PIN     12
// Line 25
// 60LEDs/m = 300 x strands
const uint16_t strandLength = 300;
// Rows/cols in serpentine grid
const uint8_t strandNumber = 1;
```

Update platform.ini configuration

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = ledcontrol.local
```

Upload via USB and use access point LEDSetup to connect the ESP32 to your Wi-Fi.

Finalize your platform.ini configuration

```ini 
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = ledcontrol.local
```

All future uploads will now be over-the-air and your device is permanently configured to connect to your Wi-Fi on bootup!
