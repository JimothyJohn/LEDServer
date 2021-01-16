String headerDoc =  R"=====(
<!DOCTYPE html>
<html>
)=====";

String cssDoc =  R"=====(

<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
.slidecontainer {
  width: 100%;
  text-align:center;
}

.slider {
  -webkit-appearance: none;
  width: 90%;
  height: 35px;
  //background: #d3d3d3;
  background: var(--SliderColor);
  outline: none;
  opacity: 0.8;
  -webkit-transition: 0.7s;
  transition: opacity .2s;
  margin-top: 5px;
  margin-bottom: 5px
  
}

.slider:hover {
  opacity: 1;  
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 35px;
  //background: var(--SliderThumbColor);
  background: #FFFFFF;
  cursor: pointer;
  border: 1px solid #F0F0F0;
  border-radius: 5px;
  box-shadow: 1px 1px 1px #000000, 0px 0px 1px #0d0d0d;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  background: #4CAF50;
  cursor: pointer;
}
</style>
</head>
)=====";

String htmlDoc = R"=====(
<body>

<h1 style="text-align:center">Color Selector</h1>
<p>Drag the sliders to display the desired color. Click Set Color to set LEDs.</p>

<div class="slidecontainer">
  <input type="range" min="0" max="360" value="200" class="slider" id="sldrHue">
  <input type="range" min="0" max="100" value="100" class="slider" id="sldrSaturation">
  <input type="range" min="0" max="100" value="50" class="slider" id="sldrLightness">
</div>
  <p>RGB: <span id="rgbValue"></span></p>
  <label for="valueinput">Hue:</label> <input type="text" id="valueinput" name="valueinput">
  <button type="button" onclick="sendPost()">Set Color</button>
  <p>RGB: <span id="demo0"></span></p>

)=====";

String scriptDoc =  R"=====(
<script>

var sliderHue = document.getElementById("sldrHue");
var sliderSaturation = document.getElementById("sldrSaturation");
var sliderLightness = document.getElementById("sldrLightness");
var output = document.getElementById("rgbValue");
var valinput = document.getElementById("valueinput");
output.innerHTML = sliderHue.value;
valinput.value = sliderHue.value;
sliderHue.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
sliderSaturation.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
sliderLightness.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)

function sendPost() {
  var xhttp = new XMLHttpRequest();
  var rgbValue = hslToRgb(sliderHue.value*0.002778, sliderSaturation.value*0.01, sliderLightness.value*0.01);
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("demo0").innerHTML = this.responseText;
    }
  };
  xhttp.open("POST", "post_RGB("+rgbValue+")", true);
  xhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhttp.send(null);
}

sliderHue.oninput = function() {
  //output.innerHTML = this.value;
  output.innerHTML = hslToRgb(sliderHue.value*0.002778, sliderSaturation.value*0.01, sliderLightness.value*0.01)
  valinput.value = this.value;
  sliderHue.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
  sliderSaturation.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
  sliderLightness.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
}
sliderSaturation.oninput = function() {
  output.innerHTML = hslToRgb(sliderHue.value*0.002778, sliderSaturation.value*0.01, sliderLightness.value*0.01)
    sliderHue.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
    sliderSaturation.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
    sliderLightness.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
}
sliderLightness.oninput = function() {
  output.innerHTML = hslToRgb(sliderHue.value*0.002778, sliderSaturation.value*0.01, sliderLightness.value*0.01)
  sliderHue.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
  sliderSaturation.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
  sliderLightness.style.setProperty('--SliderColor', `hsl(${sliderHue.value}, ${sliderSaturation.value}%, ${sliderLightness.value}%)`)
}
valinput.oninput = function() {
  output.innerHTML = this.value;
  sliderHue.value = this.value;
}
function hslToRgb(h, s, l){
    var r, g, b;

    if(s == 0){
        r = g = b = l;
    }else{
        var hue2rgb = function hue2rgb(p, q, t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(t < 1/6) return p + (q - p) * 6 * t;
            if(t < 1/2) return q;
            if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
            return p;
        }

        var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        var p = 2 * l - q;
        r = hue2rgb(p, q, h + 1/3);
        g = hue2rgb(p, q, h);
        b = hue2rgb(p, q, h - 1/3);
    }

    return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
}
</script>

)=====";

String footerDoc = R"=====(
</body>
</html>
)=====";
