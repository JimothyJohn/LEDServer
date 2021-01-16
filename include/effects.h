/*
  utils.h - Holding section for functions
  that allow for easily configureable asynchronous effects
  Created by Nick Armenta, April 26, 2020.
  Released into the public domain.
*/

#ifndef effects_h
#define effects_h

#include "Arduino.h"
#include "FastLED.h"
#include "LEDEffects.h"

// Effect indexes
#define ZERO_INDEX 0
#define PLASMA_INDEX 1
#define LIGHTNING_INDEX 2
#define PACIFICA_INDEX 3
#define NEON_RACER_INDEX 4
#define BOUNCER_INDEX 5
#define BPM_INDEX 6
#define CONFETTI_INDEX 7
#define SAWTOOTH_INDEX 8
#define COUNTDOWN_INDEX 9
#define CHASE_INDEX 10

// Pallette options
CRGBPalette16 PALETTE_SELECT[] = {OceanColors_p,
                                  LavaColors_p,
                                  ForestColors_p,
                                  RainbowColors_p,
                                  PartyColors_p,
                                  HeatColors_p};

// Pacifica palettes
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

#endif
