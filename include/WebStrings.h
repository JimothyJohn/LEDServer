#include "Arduino.h"
// #include "effects.h"

// String declarations
String cssString() {
  return R"=====(
<style>
  html {
    font-family: Helvetica;
    display: inline-block;
    margin: 0px auto;
    text-align: center;

    -webkit-background-size: cover;
    -moz-background-size: cover;
    -o-background-size: cover;
    background-size: cover;
  }
  body {
    margin-top: 50px;
  }
  h1 {
    color: #444444;
    margin: 50px auto 30px;
  }
  h3 {
    color: #444444;
    margin-bottom: 50px;
  }
  .button {
    display: block;
    width: 80px;
    border: none;
    color: white;
    padding: 13px 30px;
    text-decoration: none;
    font-size: 25px;
    margin: 0px auto 35px;
    cursor: pointer;
    border-radius: 12px;
  }
  .button-on {
    background-color: #3498db;
  }
  .button-on:active {
    background-color: #2980b9;
  }
  .button-off {
    background-color: #34495e;
  }
  .button-off:active {
    background-color: #2c3e50;
  }
  p {
    font-size: 14px;
    color: #888;
    margin-bottom: 10px;
  }
  .plasma-button {
    background-image: linear-gradient(150deg, red, yellow, orange);
  }
  .bottom {
    text-align: center;
    left: 50%;
    transform: translate(-50%);
    position: absolute;
    bottom: 0px;
    font-size: 15px;
  }
</style>
  )=====";
}

String headerString() {
  return R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<title>LED Control</title>
</head>
)=====";
}

String effectSelect() {
  return R"=====(
<div style="padding: 30px" class="container-fluid">
    <div class="row">
        <div class="col text-center">
            <h1>LED Control</h1>
        </div>
    </div>
</div>

<div style="padding: 30px" class="container-fluid">
    <div class="row">
        <div class="col text-center">
        <form action="/effect" method="get">
          <select name="effect" id="effect" onchange='if(this.value != 0) { this.form.submit(); }'>
            <option value=0>Choose effect...</option>
            <option value=1>off</option>
            <option value=2>Plasma</option>
            <option value=3>Lightning</option>
            <option value=4>Pacifica</option>
            <option value=5>Neon Racers</option>
            <option value=6>RGB Bouncers</option>
            <option value=7>BPM</option>
            <option value=8>Confetti</option>
            <option value=9>Sawtooth</option>
            <option value=10>Countdown</option>
            <option value=11>Rainbow Chase</option>
          </select>
        </form>
      </div>
    </div>
</div>
)=====";
}

String effectString[] = {"<p>off</p>",
                        "<p>PLASMA</p>",
                        "<p>Lightning</p>",
                        "<p>Pacifica</p>",
                        "<p>Racers</p>",
                        "<p>Bouncers</p>",
                        "<p>BPM</p>",
                        "<p>Confetti</p>",
                        "<p>Sawtooth</p>",
                        "<p>Countdown</p>",
                        "<p>Rainbow Chase</p>"};

#define COLOR_PARAM 0
#define MOVE_PARAM 1
#define DIR_PARAM 2
#define TEMPO_PARAM 3
#define PALETTE_PARAM 4
String parameterStrings[] = {
// 0 - Color
R"=====(
<form action="/color" method="get">
  <label for="red">Red</label><br>
  <input type="range" id="red" name="red" min="0" max="255"><br>
  <label for="green">Green</label><br>
  <input type="range" id="green" name="green" min="0" max="255"><br>
  <label for="blue">Blue</label><br>
  <input type="range" id="blue" name="blue" min="0" max="255"><br>
</form>
)=====",
// 1 - Movement Speed
R"=====(
<form action="/speed" method="get">
  <label for="speed">Movement Speed</label><br>
  <select name="speed" id="speed" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose speed...</option>
    <option value=1>Slow</option>
    <option value=2>Medium</option>
    <option value=4>Fast</option>
  </select><br>
</form>
)=====",
// 2 - Direction
R"=====(
<form action="/direction" method="get">
  <input type="checkbox" id="direction" name="direction" checked onchange='if(this.value != 0) { this.form.submit(); }'>
  <label for="direction">Direction</label><br>
</form>
)=====",
// 3 - Cycle Tempo
R"=====(
<form action="/tempo" method="get">
  <label for="tempo">BPM:</label><br>
  <input type="text" name="tempo"><br>
  <input type="submit" value="Submit">
</form>
)=====",
// 4 - Palettes
R"=====(
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

#define RACER_PARAM 0
String customParameters[] = {
// 0 - Racers
R"=====(
<form action="/racers" method="get">
  <select name="racers" id="racers" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>How many racers...</option>
    <option value=1>1</option>
    <option value=2>2</option>
    <option value=3>3</option>
    <option value=4>4</option>
  </select><br>
</form>
)=====",
""};

String effectParameters[] = {
// 0 - Zero
"",
// 1 - Plasma
parameterStrings[PALETTE_PARAM],
// 2 - Lightning
"",
// 3 - Pacifica
"",
// 4 - Racers
parameterStrings[MOVE_PARAM]+
parameterStrings[DIR_PARAM]+
customParameters[RACER_PARAM],
// 5 - Bouncers
parameterStrings[MOVE_PARAM],
// 6 - BPM
parameterStrings[TEMPO_PARAM]+
parameterStrings[PALETTE_PARAM],
// 7 - Confetti
"",
// 8 - Sawtooth
parameterStrings[PALETTE_PARAM]+
parameterStrings[TEMPO_PARAM],
// 9 - Countdown
"",
// 10 - Chase
parameterStrings[MOVE_PARAM]+
parameterStrings[DIR_PARAM]};

String footerString() {
  return R"=====(
  </body>
  </html>
  <footer class="bottom">
		<p style="font-size: 15px">Copyright © 2020 advin.io. All rights reserved.</p>
  </footer>
  )=====";
}

String bodyStyle[] = {"<body>",
                      "<body style=\"background: linear-gradient(160deg, #4f55dd, #191b81, #c7d8fe) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(150deg, red, white, orange, black) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(140deg, #468c55, white, green, black) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(150deg, red, orange, yellow, green, blue, indigo, violet) no-repeat center center fixed;\">",};

String navHTML(uint8_t effect) {
  String ptr = headerString();
  ptr += bodyStyle[0];
  ptr += effectString[effect];
  ptr += effectSelect();
  ptr += effectParameters[effect];
  ptr += footerString();
  return ptr;
}
