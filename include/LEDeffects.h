/*
  LEDeffects.h - Functions and parameters using FastLED library.
  Created by Nick Armenta, Feburary 4, 2021.
  Released into the public domain.
*/

#ifndef LEDeffects_h
#define LEDeffects_h
#include <FastLED.h>

extern uint8_t gHue;
extern const uint16_t strandLength;
extern const uint8_t strandNumber;
// extern CRGB leds;
extern CRGBPalette16 PALETTE_SELECT[];
extern CRGBPalette16 pacifica_palette_1, pacifica_palette_2, pacifica_palette_3;

void SetupLEDs();

// XY Matrix calculator
uint16_t XY(uint16_t x, uint16_t y);

// Changes an RGB pixel on all strands
void setPixelRGB(uint16_t dot, uint8_t r, uint8_t g, uint8_t b);

// Changes an HSV pixel on all strands
void setPixelHSV(uint16_t dot, uint8_t h, uint8_t s, uint8_t v);

// Changes a section of pixels to a solid RGB color
void rgbZone(uint16_t led_start, uint16_t led_end, uint8_t r, uint8_t g, uint8_t b);

// Changes a section of pixels to a solid HSV color
void hsvZone(uint16_t led_start, uint16_t led_end, uint8_t h, uint8_t s, uint8_t v);

// Modifies a section of pixels by a color offset
void rgbOverlay(uint16_t led_start, uint16_t led_end, int r_offset, int g_offset, int b_offset);

// Turns off all LED's
void clearLEDs();

// Plasma glow effect with adjustable pallet
void plasma(uint8_t palette, uint16_t led_start, uint16_t led_end);

// Lightning Strike Effect
void lightning();

// Modified Pacifica code to segment effect
// Add one layer of waves into the led array
void pacifica_one_layer(uint16_t ledStart,
                        uint16_t ledEnd,
                        CRGBPalette16& p,
                        uint16_t cistart,
                        uint16_t wavescale,
                        uint8_t bri,
                        uint16_t ioff);

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps(uint16_t ledStart, uint16_t ledEnd);

// Deepen the blues and greens
void pacifica_deepen_colors(uint16_t ledStart, uint16_t ledEnd);

// End of Pacifica code
void pacifica_loop(uint16_t ledStart, uint16_t ledEnd);

// Shaking effect at tempo
void bpm(uint8_t masterPalette, uint8_t BeatsPerMinute);

// Sparkling effect
void confetti();

// Sawtooth effect
void sawtooth(uint8_t masterPalette, uint8_t BeatsPerMinute);

// Rainbow chase effect
extern uint16_t bandLoc;
extern uint8_t numBands;
extern int8_t rbDir;
void RainbowChase(uint8_t speed, int dir, uint8_t bands);

// Racer params
extern uint16_t pinkLoc, tealLoc;
extern uint8_t pinkWidth, tealWidth;
void NeonRacers(uint8_t speed, uint8_t racerNumber);

// Bouncer params
extern uint16_t redLoc, blueLoc, greenLoc;
extern uint8_t redWidth, greenWidth, blueWidth;
extern int8_t redDir, blueDir, greenDir;
void RGBBouncers(uint8_t speed, uint8_t racerNumber);

#endif
