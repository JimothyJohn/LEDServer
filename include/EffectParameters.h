// Base framework
#include <Arduino.h>

#define ZERO_INDEX 0
#define PLASMA_INDEX 1
#define LIGHTNING_INDEX 2
#define PACIFICA_INDEX 3
#define BPM_INDEX 4
#define CONFETTI_INDEX 5
#define SAWTOOTH_INDEX 6
#define CHASE_INDEX 7
#define RACER_INDEX 8
#define BOUNCER_INDEX 9
#define COUNTDOWN_INDEX 10

String effectString[] = {"off",
                        "PLASMA",
                        "Lightning",
                        "Pacifica",
                        "BPM",
                        "Confetti",
                        "Sawtooth",
                        "Rainbow Chase",
                        "Neon Racers",
                        "Bouncers",
                        "Countdown"};

String colorParameter = {R"=====(
<form action="/color" method="get">
  <label for="red">Red</label><br>
  <input type="range" id="red" name="red" min="0" max="255"><br>
  <label for="green">Green</label><br>
  <input type="range" id="green" name="green" min="0" max="255"><br>
  <label for="blue">Blue</label><br>
  <input type="range" id="blue" name="blue" min="0" max="255"><br>
</form>
)====="
};

String speedParameter = {R"=====(
<form action="/speed" method="get">
  <label for="speed">Movement Speed</label><br>
  <select name="speed" id="speed" onchange='if(this.value != 0) { this.form.submit(); }'>
    <option value=0>Choose speed...</option>
    <option value=1>Slow</option>
    <option value=2>Medium</option>
    <option value=4>Fast</option>
  </select><br>
</form>
)====="
};

String dirParameter = {R"=====(
<form action="/direction" method="get">
  <input type="checkbox" id="direction" name="direction" checked onchange='if(this.value != 0) { this.form.submit(); }'>
  <label for="direction">Direction</label><br>
</form>
)====="
};

String tempoParameter = {R"=====(
<form action="/tempo" method="get">
  <label for="tempo">BPM:</label><br>
  <input type="text" name="tempo"><br>
  <input type="submit" value="Submit">
</form>
)====="
};

String paletteParameter = {R"=====(
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
)====="
};

String racerParameter = {R"=====(
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
paletteParameter,
// 2 - Lightning
"",
// 3 - Pacifica
"",
// 4 - BPM
tempoParameter+
paletteParameter,
// 5 - Confetti
"",
// 6 - Sawtooth
paletteParameter+
tempoParameter,
// 7 - Chase
speedParameter+
dirParameter,
// 8 - Racers
speedParameter+
dirParameter+
racerParameter,
// 9 - Bouncers
speedParameter,
// 10 - Countdown
""};
