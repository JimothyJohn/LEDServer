// Base framework
#include <Arduino.h>
// LED library and parameters
#include "LEDEffects.h"
#include <EffectParameters.h>
// Use WifiManager for login and maintenance
#include <WiFiManager.h>
// Load async web library
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
// Enable OTA updates
#include <ArduinoOTA.h>
// Connect by hostname
#include <ESPmDNS.h>
// Enable file system
#include <SPIFFS.h>

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

uint8_t activeEffect = 7;
String processor(const String& var) {
  if(var == "ACTIVE_EFFECT") {return effectString[activeEffect];}
  if(var == "PARAM_STRINGS") {return effectParameters[activeEffect];}
}

// Set up server callback functions
uint8_t masterSpeed = 1;
uint8_t masterTempo = 1;
uint8_t masterPalette = 1;
int8_t masterDir = 1;
uint8_t numRacers = 1;
// https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino
void SetupServer() {
  Serial.print('Configuring webserver...');
  // Index page, defaults to off
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // Route to load script.js file
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js", "text/js");
  });

  // Effect selection action
  server.on("/effect", HTTP_GET, [](AsyncWebServerRequest *request){
    String effect;
    if (request->hasParam("effect")) {
      effect = request->getParam("effect")->value();
      if (effect!=0) {
        activeEffect = effect.toInt() - 1;
        clearLEDs();
      }
      if(activeEffect==10) {
        warpSpeed = 5;
      }
    }
    Serial.print("Running effect: ");
    Serial.println(effectString[activeEffect]);
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // RGB color selection
  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request){
    String color;
    if (request->hasParam("color")) {
      color = request->getParam("color")->value();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Movement speed
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    String speed;
    if (request->hasParam("speed")) {
      speed = request->getParam("speed")->value();
      masterSpeed = speed.toInt();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
    
  // Forwards/backwards
  server.on("/direction", HTTP_GET, [](AsyncWebServerRequest *request){
    String direction;
    if (request->hasParam("direction")) {
      direction = request->getParam("direction")->value();
      Serial.print("Direction: ");
      Serial.println(direction);
      masterDir = (direction.toInt()-1)*2-1;
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Tempo of effect in BPM
  server.on("/tempo", HTTP_GET, [](AsyncWebServerRequest *request){
    String beats;
    if (request->hasParam("tempo")) {
      beats = request->getParam("tempo")->value();
      masterTempo = beats.toInt();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // Color palette selector
  server.on("/palette", HTTP_GET, [](AsyncWebServerRequest *request){
    String palette;
    if (request->hasParam("palette")) {
      palette = request->getParam("palette")->value();
      masterPalette = palette.toInt() - 1;
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Custom - Number of racers
  server.on("/racers", HTTP_GET, [](AsyncWebServerRequest *request){
    String number;
    if (request->hasParam("racers")) {
      number = request->getParam("racers")->value();
      numRacers = number.toInt();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Custom - Number of rainbow bands
  server.on("/bands", HTTP_GET, [](AsyncWebServerRequest *request){
    String bands;
    if (request->hasParam("bands")) {
      bands = request->getParam("bands")->value();
      numBands = bands.toInt();
    }
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  server.onNotFound(notFound);
  server.begin();
}

// Enable OTA updates
// https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino
void SetupOTA() {
  Serial.print('Configuring OTA...');
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

// Initialize Wi-Fi manager and connect to Wi-Fi
// https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino
void SetupWiFi() {
  Serial.print('Configuring WiFi...');
  WiFi.mode(WIFI_STA); // make sure your code sets wifi mode

  WiFiManager wm;

  bool res = wm.autoConnect("uServer"); // anonymous ap

  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      Serial.println("Connected to WiFi!");
  }
}

// Setup sequence
void setup() {
  delay(1000); // pwrLevel-up safety delay
  
  // Start serial server and connect to WiFi
  Serial.begin(115200);
  while (!Serial);
  Serial.print('Serial alive!');

  // Uses soft AP to connect to Wi-Fi (if saved credentials aren't valid)
  SetupWiFi();
  // Configures Async web server
  SetupServer();
  // Configures over-the-air updates
  SetupOTA();
  // Allows for connection at http://ledcontrol.local/
  // https://github.com/espressif/arduino-esp32/blob/master/libraries/ESPmDNS/examples/mDNS_Web_Server/mDNS_Web_Server.ino
  if(!MDNS.begin("ledcontrol")) {
    Serial.println("Error starting mDNS!");
    ESP.restart();
  }
  // Initialize SPIFFS
  // https://docs.platformio.org/en/latest/platforms/espressif8266.html#using-filesystem
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  SetupLEDs();
}

// Run chosen effect
void handleEffect(uint8_t effect) {
  switch (effect) {
    case ZERO_INDEX:
      break;
    case PLASMA_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {plasma(masterPalette, 0, strandNumber*strandLength);}
      break;
    case LIGHTNING_INDEX:
      lightning();
      break;
    case PACIFICA_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {pacifica_loop(0, strandNumber*strandLength);}
      break;
    case BPM_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {gHue++;} 
      bpm(masterPalette, masterTempo);
      break;
    case CONFETTI_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {gHue++;}
      confetti();
      break;
    case SAWTOOTH_INDEX:
      sawtooth(masterPalette, masterTempo);
      break;
    case CHASE_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {RainbowChase(masterSpeed, masterDir, numBands);}
      break;
    case RACER_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {NeonRacers(masterSpeed, numRacers, masterDir);}
      break;
    case BOUNCER_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {NeonRacers(masterSpeed, numRacers, masterDir);}
      break;
    case COUNTDOWN_INDEX:
      EVERY_N_MILLISECONDS(UPDATE_RATE) {Countdown();}
      break;
  }
  FastLED.show();
}

// Boilerplate loop
void loop() {
  ArduinoOTA.handle();
  handleEffect(activeEffect);
}
