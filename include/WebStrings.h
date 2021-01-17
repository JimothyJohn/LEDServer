#include "Arduino.h"

// Add Bootstrap fluid container to 
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

String headerString = {R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no">
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<title>LED Control</title>
</head>
<body>
)====="};

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

String effectSelect() {
  // Title
  String ptrString = "<h1>LED Control</h1>";
  String ptr = containerify(ptrString);
  // Selection box
  ptrString = {R"=====(
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
)====="};
  ptr += containerify(ptrString);
  return ptr;
}

String colorFormn = {R"=====(
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
  <input type="checkbox" id="direction" name="direction" checked onchange='if(this.value != 0) { this.form.submit(); }'>
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

String effectParameters[] = {
// 0 - Zero
"",
// 1 - Plasma
palletForm,
// 2 - Lightning
"",
// 3 - Pacifica
"",
// 4 - Racers
moveForm+dirForm+racerForm,
// 5 - Bouncers
moveForm,
// 6 - BPM
tempoForm+palletForm,
// 7 - Confetti
"",
// 8 - Sawtooth
palletForm+tempoForm,
// 9 - Countdown
"",
// 10 - Chase
moveForm+dirForm};

String footerString = {R"=====(
  </body>
  </html>
  <footer class="bottom">
		<p style="font-size: 15px">Copyright © 2020 advin.io. All rights reserved.</p>
  </footer>
  )====="};

String navHTML(uint8_t effect) {
  String ptr = headerString;
  ptr += effectString[effect];
  ptr += effectSelect();
  ptr += effectParameters[effect];
  ptr += footerString;
  return ptr;
}
