#include <Arduino.h>
#include <FastLED.h>
#include <LEDEffects.h>
#include <WebStrings.h>
#include <effects.h>

#ifdef ESP32
  #include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>

// Light constants
#define LED_PIN     12
#define BRIGHTNESS  220
#define START_LED   0
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define qsuba(x, b)  ((x>b)?x-b:0) // Analog Unsigned subtraction macro. if result <0, then => 0

// XY Matrix code
const uint16_t kMatrixWidth = 300;
const uint8_t kMatrixHeight = 5;

#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
CRGB leds[ NUM_LEDS ];
#define LAST_VISIBLE_LED 1499
uint16_t XY (uint16_t x, uint16_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= kMatrixWidth) || (y >= kMatrixHeight) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  const uint16_t XYTable[] = {
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
   599, 598, 597, 596, 595, 594, 593, 592, 591, 590, 589, 588, 587, 586, 585, 584, 583, 582, 581, 580, 579, 578, 577, 576, 575, 574, 573, 572, 571, 570, 569, 568, 567, 566, 565, 564, 563, 562, 561, 560, 559, 558, 557, 556, 555, 554, 553, 552, 551, 550, 549, 548, 547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 512, 511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300,
   600, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674, 675, 676, 677, 678, 679, 680, 681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694, 695, 696, 697, 698, 699, 700, 701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714, 715, 716, 717, 718, 719, 720, 721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734, 735, 736, 737, 738, 739, 740, 741, 742, 743, 744, 745, 746, 747, 748, 749, 750, 751, 752, 753, 754, 755, 756, 757, 758, 759, 760, 761, 762, 763, 764, 765, 766, 767, 768, 769, 770, 771, 772, 773, 774, 775, 776, 777, 778, 779, 780, 781, 782, 783, 784, 785, 786, 787, 788, 789, 790, 791, 792, 793, 794, 795, 796, 797, 798, 799, 800, 801, 802, 803, 804, 805, 806, 807, 808, 809, 810, 811, 812, 813, 814, 815, 816, 817, 818, 819, 820, 821, 822, 823, 824, 825, 826, 827, 828, 829, 830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844, 845, 846, 847, 848, 849, 850, 851, 852, 853, 854, 855, 856, 857, 858, 859, 860, 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874, 875, 876, 877, 878, 879, 880, 881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894, 895, 896, 897, 898, 899,
  1199,1198,1197,1196,1195,1194,1193,1192,1191,1190,1189,1188,1187,1186,1185,1184,1183,1182,1181,1180,1179,1178,1177,1176,1175,1174,1173,1172,1171,1170,1169,1168,1167,1166,1165,1164,1163,1162,1161,1160,1159,1158,1157,1156,1155,1154,1153,1152,1151,1150,1149,1148,1147,1146,1145,1144,1143,1142,1141,1140,1139,1138,1137,1136,1135,1134,1133,1132,1131,1130,1129,1128,1127,1126,1125,1124,1123,1122,1121,1120,1119,1118,1117,1116,1115,1114,1113,1112,1111,1110,1109,1108,1107,1106,1105,1104,1103,1102,1101,1100,1099,1098,1097,1096,1095,1094,1093,1092,1091,1090,1089,1088,1087,1086,1085,1084,1083,1082,1081,1080,1079,1078,1077,1076,1075,1074,1073,1072,1071,1070,1069,1068,1067,1066,1065,1064,1063,1062,1061,1060,1059,1058,1057,1056,1055,1054,1053,1052,1051,1050,1049,1048,1047,1046,1045,1044,1043,1042,1041,1040,1039,1038,1037,1036,1035,1034,1033,1032,1031,1030,1029,1028,1027,1026,1025,1024,1023,1022,1021,1020,1019,1018,1017,1016,1015,1014,1013,1012,1011,1010,1009,1008,1007,1006,1005,1004,1003,1002,1001,1000, 999, 998, 997, 996, 995, 994, 993, 992, 991, 990, 989, 988, 987, 986, 985, 984, 983, 982, 981, 980, 979, 978, 977, 976, 975, 974, 973, 972, 971, 970, 969, 968, 967, 966, 965, 964, 963, 962, 961, 960, 959, 958, 957, 956, 955, 954, 953, 952, 951, 950, 949, 948, 947, 946, 945, 944, 943, 942, 941, 940, 939, 938, 937, 936, 935, 934, 933, 932, 931, 930, 929, 928, 927, 926, 925, 924, 923, 922, 921, 920, 919, 918, 917, 916, 915, 914, 913, 912, 911, 910, 909, 908, 907, 906, 905, 904, 903, 902, 901, 900,
  1200,1201,1202,1203,1204,1205,1206,1207,1208,1209,1210,1211,1212,1213,1214,1215,1216,1217,1218,1219,1220,1221,1222,1223,1224,1225,1226,1227,1228,1229,1230,1231,1232,1233,1234,1235,1236,1237,1238,1239,1240,1241,1242,1243,1244,1245,1246,1247,1248,1249,1250,1251,1252,1253,1254,1255,1256,1257,1258,1259,1260,1261,1262,1263,1264,1265,1266,1267,1268,1269,1270,1271,1272,1273,1274,1275,1276,1277,1278,1279,1280,1281,1282,1283,1284,1285,1286,1287,1288,1289,1290,1291,1292,1293,1294,1295,1296,1297,1298,1299,1300,1301,1302,1303,1304,1305,1306,1307,1308,1309,1310,1311,1312,1313,1314,1315,1316,1317,1318,1319,1320,1321,1322,1323,1324,1325,1326,1327,1328,1329,1330,1331,1332,1333,1334,1335,1336,1337,1338,1339,1340,1341,1342,1343,1344,1345,1346,1347,1348,1349,1350,1351,1352,1353,1354,1355,1356,1357,1358,1359,1360,1361,1362,1363,1364,1365,1366,1367,1368,1369,1370,1371,1372,1373,1374,1375,1376,1377,1378,1379,1380,1381,1382,1383,1384,1385,1386,1387,1388,1389,1390,1391,1392,1393,1394,1395,1396,1397,1398,1399,1400,1401,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414,1415,1416,1417,1418,1419,1420,1421,1422,1423,1424,1425,1426,1427,1428,1429,1430,1431,1432,1433,1434,1435,1436,1437,1438,1439,1440,1441,1442,1443,1444,1445,1446,1447,1448,1449,1450,1451,1452,1453,1454,1455,1456,1457,1458,1459,1460,1461,1462,1463,1464,1465,1466,1467,1468,1469,1470,1471,1472,1473,1474,1475,1476,1477,1478,1479,1480,1481,1482,1483,1484,1485,1486,1487,1488,1489,1490,1491,1492,1493,1494,1495,1496,1497,1498,1499
  };

  uint16_t i = (y * kMatrixWidth) + x;
  uint16_t j = XYTable[i];
  return j;
}

Segment bkg = {0, NUM_LEDS, NUM_LEDS};
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

AsyncWebServer server(80);

void setPixelRGB(uint16_t dot, uint8_t r, uint8_t g, uint8_t b) {
  uint16_t newdot;
  for(uint8_t strand=0; strand<kMatrixHeight; strand++) {
    newdot = XY(dot,strand);
    leds[newdot][0] = r;
    leds[newdot][1] = g;
    leds[newdot][2] = b;
  }
}

void rgbZone(uint16_t led_start, uint16_t led_end, uint8_t r, uint8_t g, uint8_t b) {
  //Light each pixel in zone w/ overlap
  uint16_t newdot;
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    if (dot<kMatrixWidth) {
      newdot = dot;
    } else {
      newdot = dot-kMatrixWidth;
    }
    setPixelRGB(newdot, r, g, b);
  }
}

void hsvZone(uint16_t led_start, uint16_t led_end, uint8_t h, uint8_t s, uint8_t v) {
  //Light each pixel in zone w/ overlap
  uint16_t newdot;
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    if (dot<kMatrixWidth) {
      newdot = dot;
    } else {
      newdot = dot-kMatrixWidth;
    }
    for(uint8_t strand=0; strand<kMatrixHeight; strand++) {
      leds[XY(newdot,strand)] = CHSV(h,s,v);
    }
  }
}

void rgbOverlay(uint16_t led_start, uint16_t led_end, int r_offset, int g_offset, int b_offset) {
  //Light each pixel in zone  w/ overlap
  uint8_t r, g, b;
  uint16_t newdot;
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    if (dot<kMatrixWidth) {
      newdot = dot;
    } else {
      newdot = dot-kMatrixWidth;
    }
    r=leds[newdot][0];
    g=leds[newdot][1];
    b=leds[newdot][2];

    r = qadd8(r,r_offset);
    g = qadd8(g,g_offset);
    b = qadd8(b,b_offset);
        
    setPixelRGB(newdot, r, g, b);
  }
}

void clearLEDs() {
  for(int dot = 0; dot < NUM_LEDS; dot++) {
    setPixelRGB(dot, 0, 0, 0); 
  }
}

void plasma(uint8_t palette, uint16_t led_start, uint16_t led_end) {                                                 // This is the heart of this program. Sure is short. . . and fast.

  int thisPhase = beatsin8(6,-64,64);                           // Setting phase change for a couple of waves.
  int thatPhase = beatsin8(7,-64,64);
  TBlendType currentBlending = LINEARBLEND;
  CRGBPalette16 targetPalette = PALETTE_SELECT[palette];

  for (int k=led_start; k<led_end; k++) {                              // For each of the LED's in the strand, set a brightness based on a wave as follows:

    int colorIndex = cubicwave8((k*23)+thisPhase)/2 + cos8((k*15)+thatPhase)/2;           // Create a wave and add a phase change and add another wave with its own phase change.. Hey, you can even change the frequencies if you wish.
    int thisBright = qsuba(colorIndex, beatsin8(7,0,96));                                 // qsub gives it a bit of 'black' dead space by setting sets a minimum value. If colorIndex < current value of beatsin8(), then bright = 0. Otherwise, bright = colorIndex..

    leds[k] = ColorFromPalette(targetPalette, colorIndex, thisBright, currentBlending);  // Let's now add the foreground colour.
  }
}

void lightning() {
  uint16_t dimmer = 1;
  uint16_t ledstart = random8(NUM_LEDS);                               // Determine starting location of flash
  uint16_t ledlen = random8(NUM_LEDS-ledstart);                        // Determine length of flash (not to go beyond NUM_LEDS-1)
  
  for (int flashCounter = 0; flashCounter < random8(3,8); flashCounter++) {
    if(flashCounter == 0) dimmer = 5;                         // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1,3);                               // return strokes are brighter than the leader
    
    fill_solid(leds+ledstart,ledlen,CHSV(255, 0, 255/dimmer));
    FastLED.show();                       // Show a section of LED's
    delay(random8(4,10));                                     // each flash only lasts 4-10 milliseconds
    fill_solid(leds+ledstart,ledlen,CHSV(255,0,0));           // Clear the section of LED's
    FastLED.show();
    
    if (flashCounter == 0) delay (150);                       // longer delay until next flash after the leader
    
    delay(50+random8(100));                                   // shorter delay between strokes  
  }  
  delay(random8(50)*100);                              // delay between strikes
}

// Start of Pacifica code - Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps() {
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors() {
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

void pacifica_loop() {
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}
// End of Pacifica code

// Racer params
uint8_t pinkWidth = 15;
uint8_t blueWidth = 30;
Segment pinkRacer = {0, pinkWidth, kMatrixWidth};
Segment blueRacer = {0, blueWidth, kMatrixWidth};
void NeonRacers(uint8_t speed, int racedir, uint8_t racerNumber) {
  // Motion Effects
  pinkRacer.Scan((0.05/speed), racedir);
  blueRacer.Scan((0.1/speed), racedir);

  // Add Color
  rgbZone(bkg.loc0, bkg.loc1, 0, 0, 0);
  uint16_t pinkSpace = (kMatrixWidth-racerNumber*pinkWidth)/racerNumber+pinkWidth;
  uint16_t blueSpace = (kMatrixWidth-racerNumber*blueWidth)/racerNumber+blueWidth;
  for(uint8_t racer=0; racer<racerNumber; racer++) {
    rgbOverlay(pinkRacer.loc0+pinkSpace*racer, pinkRacer.loc1+pinkSpace*racer, 250, 10, 20);
    rgbOverlay(blueRacer.loc0+blueSpace*racer, blueRacer.loc1+blueSpace*racer, 20, 10, 250);
  }
}

// Bouncer params
Segment redBouncer = {0, 30, kMatrixWidth};
Segment blueBouncer = {0, 20, kMatrixWidth};
Segment greenBouncer = {0, 10, kMatrixWidth};
void Bouncers(uint8_t bounceSpeed) {
  float speed = 6 / bounceSpeed;
  
  // Motion Effects
  redBouncer.Bounce(speed, kMatrixWidth);
  blueBouncer.Bounce(speed/2, kMatrixWidth);
  greenBouncer.Bounce(speed/3, kMatrixWidth);

  // Add Color
  rgbZone(bkg.loc0, bkg.loc1, 0, 0, 0);
  rgbOverlay(redBouncer.loc0, redBouncer.loc1, 255, 0, 0);
  rgbOverlay(blueBouncer.loc0, blueBouncer.loc1, 0, 255, 0);
  rgbOverlay(greenBouncer.loc0, greenBouncer.loc1, 0, 0, 255);
}

uint8_t tempo = 120;
void bpm(uint8_t masterPalette, uint8_t BeatsPerMinute) {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  CRGBPalette16 palette = PALETTE_SELECT[masterPalette];
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

void sawtooth(uint8_t masterPalette, uint8_t BeatsPerMinute) {
  int ms_per_beat = 60000/BeatsPerMinute; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/BeatsPerMinute/kMatrixWidth;
  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(kMatrixWidth); // Using millis to count up the strand, with %NUM_LEDS at the end as a safety factor.

  CRGBPalette16 palette = PALETTE_SELECT[masterPalette];
  TBlendType currentBlending = LINEARBLEND;
  if (cur_led == 0)
   fill_solid(leds, kMatrixWidth, CRGB::Black);
  else
    leds[XY(cur_led,0)] = ColorFromPalette(palette, cur_led, 255, currentBlending); // I prefer to use palettes instead of CHSV or CRGB assignments.
}

// Countdown sequence
uint8_t sequencer = 0;
uint8_t lastbeat = 0;
uint8_t beatmin = 255;
void countdown() {
  uint16_t sectionLength = (kMatrixWidth / 5)-1;
  uint16_t sectionLEDs = kMatrixWidth - sectionLength*sequencer;
  uint8_t beat = beatsin8(60, 0, 255);
  for(int dot = 0; dot < sectionLEDs; dot++) {
    setPixelRGB(dot, beat, 0, 0);
  }
  // Count pulses
  if (beat<=3) {
    if (lastbeat==1 && beat>lastbeat) {
      sequencer++;
      Serial.print("\rSequencer at ");
      Serial.print(sequencer);
    }
    lastbeat = beat;
  }
}

// Warp sequence
uint8_t starWidth = 20;
uint8_t lastpos = 0;
Segment starStripe = {0, starWidth, kMatrixWidth};
void Warpspeed() {
  // Limit speed
  uint8_t stpDist = (sequencer - 4)*2;
  if (sequencer > 10) {
    stpDist = 16;
  }
  
  // Motion Effects
  starStripe.Scan(0.05, stpDist);

  // Add Color
  rgbZone(bkg.loc0, bkg.loc1, 0, 0, 0);
  rgbZone(starStripe.loc0, starStripe.loc1, 255, 255, 255);

  // Count cycles
  if (starStripe.loc0<stpDist*2 && lastpos>(kMatrixWidth-stpDist*2)) {
      sequencer++;
      Serial.print("\rSequencer at ");
      Serial.print(sequencer);
  }
  lastpos = starStripe.loc0;
}

// Twinkle sequence
fract8 chanceOfGlitter = 255;
void Dissolve(fract8 chanceOfGlitter) {
  rgbZone(bkg.loc0, bkg.loc1, 0, 0, 0);
  for (uint8_t i=0; i<NUM_LEDS; i++) {
    if(random8() < chanceOfGlitter) {
      leds[random16(NUM_LEDS)] += CRGB::White;
    }
  }
}

// Racer params
Segment chaseBand = {0, 20, kMatrixWidth};
void RainbowChase(uint8_t speed, int dir, uint8_t bands) {
  uint16_t bandWidth = kMatrixWidth / bands;
  uint8_t wavelength = 255 / bands;

  // Motion Effects
  chaseBand.Scan((0.1/speed), dir);
  uint16_t startled = chaseBand.loc0;
  uint8_t satvalOffset = 128;
  uint8_t satval = startled;
  uint8_t satvalStep = (255-satvalOffset) / bandWidth;
  uint16_t newdot;

  // Add Color
  for(uint8_t band=0; band<bands; band++) {
    startled = chaseBand.loc0+bandWidth*band;
    for(uint16_t dot=startled; dot<startled+bandWidth; dot++) {
      satval = (dot-startled)*satvalStep+satvalOffset;
      if (dot<kMatrixWidth) {
        newdot = dot;
      } else {
        newdot = dot-kMatrixWidth;
      }
      for(uint8_t strand=0; strand<kMatrixHeight; strand++) {
        leds[XY(newdot,strand)] = CHSV(wavelength*band,satval,satval);
      }
    }
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

uint8_t activeEffect = ZERO_INDEX;
uint8_t masterSpeed = 1;
uint8_t masterTempo = 1;
uint8_t masterPalette = 1;
uint8_t masterDir = 1;
uint8_t numRacers = 1;
// https://github.com/me-no-dev/ESPAsyncWebServer/blob/master/examples/simple_server/simple_server.ino
void SetupServer() {
  // Index page, defaults to off
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", navHTML(activeEffect));
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
    }
    sequencer = 0;
    chanceOfGlitter = 255;
    request->send(200, "text/html", navHTML(activeEffect));
  });
  
  // RGB color selection
  server.on("/color", HTTP_GET, [](AsyncWebServerRequest *request){
    String color;
    if (request->hasParam("color")) {
      color = request->getParam("color")->value();
      //plasmaPalette = palette.toInt() - 1;
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });
  
  // Movement speed
  server.on("/speed", HTTP_GET, [](AsyncWebServerRequest *request){
    String speed;
    if (request->hasParam("speed")) {
      speed = request->getParam("speed")->value();
      masterSpeed = speed.toInt();
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });
    
  // Forwards/backwards
  server.on("/direction", HTTP_GET, [](AsyncWebServerRequest *request){
    String direction;
    if (request->hasParam("direction")) {
      direction = request->getParam("direction")->value();
      masterDir = direction.toInt();
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });
  
  // Tempo of effect in BPM
  server.on("/tempo", HTTP_GET, [](AsyncWebServerRequest *request){
    String beats;
    if (request->hasParam("tempo")) {
      beats = request->getParam("tempo")->value();
      masterTempo = beats.toInt();
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });
  
  // Color palette selector
  server.on("/palette", HTTP_GET, [](AsyncWebServerRequest *request){
    String palette;
    if (request->hasParam("palette")) {
      palette = request->getParam("palette")->value();
      masterPalette = palette.toInt() - 1;
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });

  // Custom - Number of racers
  server.on("/racers", HTTP_GET, [](AsyncWebServerRequest *request){
    String number;
    if (request->hasParam("racers")) {
      number = request->getParam("racers")->value();
      numRacers = number.toInt();
    }
    request->send(200, "text/html", navHTML(activeEffect));
  });
  
  server.onNotFound(notFound);
  server.begin();
}

// https://github.com/espressif/arduino-esp32/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino
void SetupOTA() {
  ArduinoOTA.setHostname("myesp32");

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

// https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino
void SetupWiFi() {
  WiFi.mode(WIFI_STA); // it is a good practice to make sure your code sets wifi mode how you want it.

  WiFiManager wm;

  bool res = wm.autoConnect("LEDSetup"); // anonymous ap

  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      Serial.println("Connected to WiFi!");
  }
}

void setup() {
  delay(1000); // pwrLevel-up safety delay
  
  // Start serial server and connect to WiFi
  Serial.begin(115200);
  while (!Serial);

  // Uses soft AP to connect to Wi-Fi (if saved credentials aren't valid)
  SetupWiFi();
  // Configures Async web server
  SetupServer();
  // Configures over-the-air updates
  SetupOTA();
  // Allows for connection at http://ledcontrol.local/
  if(!MDNS.begin("ledcontrol")) {
    Serial.println("Error starting mDNS!");
    ESP.restart();
  }

  //Configure LEDs
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  clearLEDs();
  FastLED.show();
}

void loop() {
  ArduinoOTA.handle();
  switch (activeEffect) {
    case ZERO_INDEX:
      break;
    case PLASMA_INDEX:
      EVERY_N_MILLISECONDS(50) {plasma(masterPalette, 0, NUM_LEDS);}
      break;
    case LIGHTNING_INDEX:
      lightning();
      break;
    case PACIFICA_INDEX:
      EVERY_N_MILLISECONDS(20) {pacifica_loop();}
      break;
    case NEON_RACER_INDEX:
      NeonRacers(masterSpeed, masterDir, numRacers);
      break;
    case BOUNCER_INDEX:
      Bouncers(masterSpeed);
      break;
    case BPM_INDEX:
      EVERY_N_MILLISECONDS(20) {gHue++;} 
      bpm(masterPalette, masterTempo);
      break;
    case CONFETTI_INDEX:
      EVERY_N_MILLISECONDS(20) {gHue++;}
      confetti();
      break;
    case SAWTOOTH_INDEX:
      sawtooth(masterPalette, masterTempo);
      break;
    case COUNTDOWN_INDEX:
      if ((sequencer>=0) && (sequencer<5)) {countdown();
      } else if ((sequencer>=5) && (sequencer<20)) {Warpspeed();
      } else if (sequencer>=20 && chanceOfGlitter>8) {
        Dissolve(chanceOfGlitter);
        chanceOfGlitter -= 8;
        delay(50);
      } else {rgbZone(bkg.loc0, bkg.loc1, 0, 0, 0);}
      break;
    case CHASE_INDEX:
      RainbowChase(masterSpeed, masterDir, 5);
      break;
  }
  FastLED.show();
}
