# Quick Start Guide

Start by cloning the repository into your desired directory:

```sh 
user@host:~$ git clone https://github.com/JimothyJohn/LEDServer.git
```

Update [`src/main.cpp`](src/main.cpp) LED parameters

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

Update [`platform.ini`](platform.ini) configuration

```ini 
upload_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_port = /dev/ttyUSB0 # CHANGE THIS IF NEEDED
monitor_speed = 115200
# We'll add these later
# upload_protocol = espota 
# upload_port = ledcontrol.local
```

Move to root of repository and upload

```sh 
user@host:~$ cd <LEDServer>
user@host:<LEDServer>$ pio run -t upload
```

Go to your phone/PC's Wi-Fi settings and use access point "LEDSetup" to connect the ESP32 to your Wi-Fi, then finalize your [`platform.ini`](platform.ini) configuration

```ini
# Leave as comments in case you need to troubleshoot
# upload_port = /dev/ttyUSB0
# monitor_port = /dev/ttyUSB0
monitor_speed = 115200
upload_protocol = espota
upload_port = ledcontrol.local
```

All future uploads will now be over-the-air and your device is permanently configured to connect to your Wi-Fi on bootup!
