#include "Arduino.h"
#include "effects.h"

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
  String ptr = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<title>LED Control</title>
)=====";
  ptr += cssString();
  ptr += "</head>";
  return ptr;
}

String effectSelect() {
  String ptr = R"=====(
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
  </select>
</form><br>
)=====";
  return ptr;
}

String parameterString[] = {
// Blank off string
"",
// Plasma parameters
R"=====(
<form action="/plasma" method="get">
  <select name="color" id="color" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose color...</option>
    <option value=1>Blue</option>
    <option value=2>Red</option>
    <option value=3>Green</option>
  </select>
</form>
)=====",
// Lightning parameters
R"=====(
<form action="/lightning" method="get">
  <label for="frequency">Frequency:</label>
  <input type="text" name="frequency"><br>
  <label for="flashes">Flashes:</label>
  <input type="text" name="flashes"><br>
  <input type="submit" value="Submit">
</form>
)=====",
// Blank Pacifica string
"",
// Racer parameters
R"=====(
<form action="/racers" method="get">
  <select name="speed" id="speed" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose speed...</option>
    <option value=1>Slow</option>
    <option value=2>Medium</option>
    <option value=4>Fast</option>
  </select><br>
  <select name="number" id="number" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>How many racers...</option>
    <option value=1>1</option>
    <option value=2>2</option>
    <option value=3>3</option>
    <option value=4>4</option>
  </select><br>
  <select name="direction" id="direction" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose direction...</option>
    <option value=-1>Backward</option>
    <option value=1>Forward</option>
  </select><br>
</form>
)=====",
// Bouncer parameters
R"=====(
<form action="/bouncers" method="get">
  <select name="speed" id="speed" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose speed...</option>
    <option value=1>Slow</option>
    <option value=2>Medium</option>
    <option value=4>Fast</option>
  </select>
</form>
)=====",
// BPM parameters
R"=====(
<form action="/bpm" method="get">
  <select name="color" id="color" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose color...</option>
    <option value=1>Blue</option>
    <option value=2>Red</option>
    <option value=3>Green</option>
    <option value=4>Rainbow</option>
    <option value=5>Party</option>
  </select><br>
  <label for="beats">BPM:</label>
  <input type="text" name="beats"><br>
  <input type="submit" value="Submit">
</form>
)=====",
// Blank confetti string
"",
// BPM parameters
R"=====(
<form action="/sawtooth" method="get">
  <select name="color" id="color" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose color...</option>
    <option value=1>Blue</option>
    <option value=2>Red</option>
    <option value=3>Green</option>
    <option value=4>Rainbow</option>
    <option value=5>Party</option>
  </select>
</form>
)====="};

String effectString[] = {"<p>off</p>",
                        "<p>PLASMA</p>",
                        "<p>Lightning</p>",
                        "<p>Pacifica</p>",
                        "<p>Racers</p>",
                        "<p>Bouncers</p>",
                        "<p>BPM</p>",
                        "<p>Confetti</p>",
                        "<p>Sawtooth</p>"};

String footerString() {
  String ptr = R"=====(
  </body>
  </html>
  <footer class="bottom">
		<p style="font-size: 15px">Copyright © 2020 advin.io. All rights reserved.</p>
  </footer>
  )=====";
  return ptr;
}

String bodyStyle[] = {"<body>",
                      "<body style=\"background: linear-gradient(160deg, #4f55dd, #191b81, #c7d8fe) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(150deg, red, white, orange, black) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(140deg, #468c55, white, green, black) no-repeat center center fixed;\">",
                      "<body style=\"background: linear-gradient(150deg, red, orange, yellow, green, blue, indigo, violet) no-repeat center center fixed;\">",};

String navHTML(uint8_t effect) {
  String ptr = headerString();
  ptr += bodyStyle[0];
  ptr += effectSelect();
  ptr += parameterString[effect];
  ptr += effectString[effect];
  ptr += footerString();
  return ptr;
}
