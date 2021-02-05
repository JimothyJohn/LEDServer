#include "Arduino.h"

// Effect indexes
#define ZERO_INDEX 0
#define PLASMA_INDEX 1
#define LIGHTNING_INDEX 2
#define PACIFICA_INDEX 3
#define BPM_INDEX 4
#define CONFETTI_INDEX 5
#define SAWTOOTH_INDEX 6
#define CHASE_INDEX 7
#define RACER_INDEX 8

// Pallette options
CRGBPalette16 PALETTE_SELECT[] = {OceanColors_p,
                                  LavaColors_p,
                                  ForestColors_p,
                                  RainbowColors_p,
                                  PartyColors_p,
                                  HeatColors_p};

// Pacifica palettes
CRGBPalette16 pacifica_palette_1 = {
  0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = {
  0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
  0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = {
  0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
  0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// Rotating "base color" used by many of the patterns
uint8_t gHue = 0; 

// Add Bootstrap fluid container wrapper
String containerify(String body) {
  String ptr = R"=====(
<div style="padding: 30px" class="container-fluid">
    <div class="row">
        <div class="col text-center">
)=====";
  ptr += body;
  ptr += R"=====(
    </div>
  </div>
</div>
)=====";
  return ptr;
}

String headerString() = {
  String ptr = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no" charset="UTF-8">
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<title>LED Control</title>
</head>
<body>
)====="
  String ptrString = R"=====(

)====="
};

String effectString[] = {"off",
                        "Plasma",
                        "Lightning",
                        "Pacifica",
                        "BPM",
                        "Confetti",
                        "Sawtooth",
                        "Rainbow Chase",
                        "Neon Racers"};

String effectSelect(uint8_t effect) {
  // Title
  String ptrString = "<h1>LED Control</h1>";
  String ptr = containerify(ptrString);
  ptr += containerify("<h3>"+effectString[effect]+"</h3>");
  // Selection box
  ptrString = R"=====(
<form action="/effect" method="get">
  <select name="effect" id="effect" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose effect...</option>
    <option value=1>off</option>
    <option value=2>Plasma</option>
    <option value=3>Lightning</option>
    <option value=4>Pacifica</option>
    <option value=5>BPM</option>
    <option value=6>Confetti</option>
    <option value=7>Sawtooth</option>
    <option value=8>Rainbow Chase</option>
    <option value=9>Neon Racers</option>
  </select>
</form>
)=====";
  ptr += containerify(ptrString);
  return ptr;
}

String colorForm = {R"=====(
<form action="/color" method="get">
  <label for="red">Red</label><br>
  <input type="range" id="red" name="red" min="0" max="255"><br>
  <label for="green">Green</label><br>
  <input type="range" id="green" name="green" min="0" max="255"><br>
  <label for="blue">Blue</label><br>
  <input type="range" id="blue" name="blue" min="0" max="255"><br>
</form>
)====="};
String moveForm = {R"=====(
<form action="/speed" method="get">
  <label for="speed">Movement Speed</label><br>
  <select name="speed" id="speed" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose speed...</option>
    <option value=1>Slow</option>
    <option value=2>Medium</option>
    <option value=4>Fast</option>
  </select><br>
</form>
)====="};
String dirForm = {R"=====(
<form action="/direction" method="get">
  <input type="checkbox" id="direction" name="direction" checked onchange='this.form.submit()}'>
  <label for="direction">Direction</label><br>
</form>
)====="};
String tempoForm = {R"=====(
<form action="/tempo" method="get">
  <label for="tempo">BPM:</label><br>
  <input type="text" name="tempo"><br>
  <input type="submit" value="Submit">
</form>
)====="};
String palletForm = {R"=====(
<form action="/palette" method="get">
  <label for="palette">Palette Selector</label><br>
  <select name="palette" id="palette" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose palette...</option>
    <option value=1>Ocean</option>
    <option value=2>Lava</option>
    <option value=3>Forest</option>
    <option value=4>Rainbow</option>
    <option value=5>Party</option>
    <option value=6>Heat</option>
  </select><br>
</form>
)====="};
String racerForm = {R"=====(
<h3>Racers</h3>
<form action="/racers" method="get">
  <select name="racers" id="racers" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>How many racers...</option>
    <option value=1>1</option>
    <option value=2>2</option>
    <option value=3>3</option>
    <option value=4>4</option>
  </select><br>
</form>
)====="};
String bandForm = {R"=====(
<h3>Colors</h3>
<form action="/bands" method="get">
  <select name="bands" id="bands" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>How many bands...</option>
    <option value=4>4</option>
    <option value=6>6</option>
    <option value=8>8</option>
    <option value=10>10</option>
  </select><br>
</form>
)====="};

String effectParameters[] = {
// 0 - Zero
"",
// 1 - Plasma
palletForm,
// 2 - Lightning
"",
// 3 - Pacifica
"",
// 4 - BPM
tempoForm+palletForm,
// 5 - Confetti
"",
// 6 - Sawtooth
palletForm+tempoForm,
// 7 - Chase
moveForm+dirForm+bandForm,
// 8 - Racers
moveForm+racerForm
};

String footerString = {R"=====(
  </body>
  </html>
  <footer class="fixed-bottom">
        <div class="container-fluid">
            <div class="row">
              <div class="col text-center">
                <p style="font-size: 15px">Copyright © 2020 advin.io. All rights reserved.</p>
              </div>
            </div>
        </div>
  </footer>
  )====="};

String navHTML(uint8_t effect) {
  String ptr = headerString;
  ptr += effectSelect(effect);
  ptr += containerify(effectParameters[effect]);
  ptr += footerString;
  return ptr;
}
