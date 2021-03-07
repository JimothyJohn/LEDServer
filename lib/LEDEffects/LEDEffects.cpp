// Base framework
#include <Arduino.h>
#include <FastLED.h>

#include "LEDeffects.h"

// Light configuration
#define LED_PIN     12
#define BRIGHTNESS  220
#define START_LED   0
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define qsuba(x, b)  ((x>b)?x-b:0) // For Plasma Effect
const uint16_t strandLength = 300;
const uint8_t strandNumber = 8;
const uint8_t UPDATE_RATE = 15;

CRGB leds[(strandLength * strandNumber)];

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

void SetupLEDs() {
  //Configure LEDs
  Serial.print('Configuring LEDs...');
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, (strandLength * strandNumber)).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  clearLEDs();
  FastLED.show();
}

#define LAST_VISIBLE_LED 2399
// Matrix table conversion
uint16_t XY(uint16_t x, uint16_t y) {
  // any out of bounds address maps to the first hidden pixel
  if ( (x >= strandLength) || (y >= strandNumber) ) {
    return (LAST_VISIBLE_LED + 1);
  }

  // Configure your table at: https://macetech.github.io/FastLED-XY-Map-Generator/
  const uint16_t XYTable[] = {
     0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299,
   599, 598, 597, 596, 595, 594, 593, 592, 591, 590, 589, 588, 587, 586, 585, 584, 583, 582, 581, 580, 579, 578, 577, 576, 575, 574, 573, 572, 571, 570, 569, 568, 567, 566, 565, 564, 563, 562, 561, 560, 559, 558, 557, 556, 555, 554, 553, 552, 551, 550, 549, 548, 547, 546, 545, 544, 543, 542, 541, 540, 539, 538, 537, 536, 535, 534, 533, 532, 531, 530, 529, 528, 527, 526, 525, 524, 523, 522, 521, 520, 519, 518, 517, 516, 515, 514, 513, 512, 511, 510, 509, 508, 507, 506, 505, 504, 503, 502, 501, 500, 499, 498, 497, 496, 495, 494, 493, 492, 491, 490, 489, 488, 487, 486, 485, 484, 483, 482, 481, 480, 479, 478, 477, 476, 475, 474, 473, 472, 471, 470, 469, 468, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 457, 456, 455, 454, 453, 452, 451, 450, 449, 448, 447, 446, 445, 444, 443, 442, 441, 440, 439, 438, 437, 436, 435, 434, 433, 432, 431, 430, 429, 428, 427, 426, 425, 424, 423, 422, 421, 420, 419, 418, 417, 416, 415, 414, 413, 412, 411, 410, 409, 408, 407, 406, 405, 404, 403, 402, 401, 400, 399, 398, 397, 396, 395, 394, 393, 392, 391, 390, 389, 388, 387, 386, 385, 384, 383, 382, 381, 380, 379, 378, 377, 376, 375, 374, 373, 372, 371, 370, 369, 368, 367, 366, 365, 364, 363, 362, 361, 360, 359, 358, 357, 356, 355, 354, 353, 352, 351, 350, 349, 348, 347, 346, 345, 344, 343, 342, 341, 340, 339, 338, 337, 336, 335, 334, 333, 332, 331, 330, 329, 328, 327, 326, 325, 324, 323, 322, 321, 320, 319, 318, 317, 316, 315, 314, 313, 312, 311, 310, 309, 308, 307, 306, 305, 304, 303, 302, 301, 300,
   899,898,897,896,895,894,893,892,891,890,889,888,887,886,885,884,883,882,881,880,879,878,877,876,875,874,873,872,871,870,869,868,867,866,865,864,863,862,861,860,859,858,857,856,855,854,853,852,851,850,849,848,847,846,845,844,843,842,841,840,839,838,837,836,835,834,833,832,831,830,829,828,827,826,825,824,823,822,821,820,819,818,817,816,815,814,813,812,811,810,809,808,807,806,805,804,803,802,801,800,799,798,797,796,795,794,793,792,791,790,789,788,787,786,785,784,783,782,781,780,779,778,777,776,775,774,773,772,771,770,769,768,767,766,765,764,763,762,761,760,759,758,757,756,755,754,753,752,751,750,749,748,747,746,745,744,743,742,741,740,739,738,737,736,735,734,733,732,731,730,729,728,727,726,725,724,723,722,721,720,719,718,717,716,715,714,713,712,711,710,709,708,707,706,705,704,703,702,701,700,699,698,697,696,695,694,693,692,691,690,689,688,687,686,685,684,683,682,681,680,679,678,677,676,675,674,673,672,671,670,669,668,667,666,665,664,663,662,661,660,659,658,657,656,655,654,653,652,651,650,649,648,647,646,645,644,643,642,641,640,639,638,637,636,635,634,633,632,631,630,629,628,627,626,625,624,623,622,621,620,619,618,617,616,615,614,613,612,611,610,609,608,607,606,605,604,603,602,601,600,
  1199,1198,1197,1196,1195,1194,1193,1192,1191,1190,1189,1188,1187,1186,1185,1184,1183,1182,1181,1180,1179,1178,1177,1176,1175,1174,1173,1172,1171,1170,1169,1168,1167,1166,1165,1164,1163,1162,1161,1160,1159,1158,1157,1156,1155,1154,1153,1152,1151,1150,1149,1148,1147,1146,1145,1144,1143,1142,1141,1140,1139,1138,1137,1136,1135,1134,1133,1132,1131,1130,1129,1128,1127,1126,1125,1124,1123,1122,1121,1120,1119,1118,1117,1116,1115,1114,1113,1112,1111,1110,1109,1108,1107,1106,1105,1104,1103,1102,1101,1100,1099,1098,1097,1096,1095,1094,1093,1092,1091,1090,1089,1088,1087,1086,1085,1084,1083,1082,1081,1080,1079,1078,1077,1076,1075,1074,1073,1072,1071,1070,1069,1068,1067,1066,1065,1064,1063,1062,1061,1060,1059,1058,1057,1056,1055,1054,1053,1052,1051,1050,1049,1048,1047,1046,1045,1044,1043,1042,1041,1040,1039,1038,1037,1036,1035,1034,1033,1032,1031,1030,1029,1028,1027,1026,1025,1024,1023,1022,1021,1020,1019,1018,1017,1016,1015,1014,1013,1012,1011,1010,1009,1008,1007,1006,1005,1004,1003,1002,1001,1000, 999, 998, 997, 996, 995, 994, 993, 992, 991, 990, 989, 988, 987, 986, 985, 984, 983, 982, 981, 980, 979, 978, 977, 976, 975, 974, 973, 972, 971, 970, 969, 968, 967, 966, 965, 964, 963, 962, 961, 960, 959, 958, 957, 956, 955, 954, 953, 952, 951, 950, 949, 948, 947, 946, 945, 944, 943, 942, 941, 940, 939, 938, 937, 936, 935, 934, 933, 932, 931, 930, 929, 928, 927, 926, 925, 924, 923, 922, 921, 920, 919, 918, 917, 916, 915, 914, 913, 912, 911, 910, 909, 908, 907, 906, 905, 904, 903, 902, 901, 900,
  1499,1498,1497,1496,1495,1494,1493,1492,1491,1490,1489,1488,1487,1486,1485,1484,1483,1482,1481,1480,1479,1478,1477,1476,1475,1474,1473,1472,1471,1470,1469,1468,1467,1466,1465,1464,1463,1462,1461,1460,1459,1458,1457,1456,1455,1454,1453,1452,1451,1450,1449,1448,1447,1446,1445,1444,1443,1442,1441,1440,1439,1438,1437,1436,1435,1434,1433,1432,1431,1430,1429,1428,1427,1426,1425,1424,1423,1422,1421,1420,1419,1418,1417,1416,1415,1414,1413,1412,1411,1410,1409,1408,1407,1406,1405,1404,1403,1402,1401,1400,1399,1398,1397,1396,1395,1394,1393,1392,1391,1390,1389,1388,1387,1386,1385,1384,1383,1382,1381,1380,1379,1378,1377,1376,1375,1374,1373,1372,1371,1370,1369,1368,1367,1366,1365,1364,1363,1362,1361,1360,1359,1358,1357,1356,1355,1354,1353,1352,1351,1350,1349,1348,1347,1346,1345,1344,1343,1342,1341,1340,1339,1338,1337,1336,1335,1334,1333,1332,1331,1330,1329,1328,1327,1326,1325,1324,1323,1322,1321,1320,1319,1318,1317,1316,1315,1314,1313,1312,1311,1310,1309,1308,1307,1306,1305,1304,1303,1302,1301,1300,1299,1298,1297,1296,1295,1294,1293,1292,1291,1290,1289,1288,1287,1286,1285,1284,1283,1282,1281,1280,1279,1278,1277,1276,1275,1274,1273,1272,1271,1270,1269,1268,1267,1266,1265,1264,1263,1262,1261,1260,1259,1258,1257,1256,1255,1254,1253,1252,1251,1250,1249,1248,1247,1246,1245,1244,1243,1242,1241,1240,1239,1238,1237,1236,1235,1234,1233,1232,1231,1230,1229,1228,1227,1226,1225,1224,1223,1222,1221,1220,1219,1218,1217,1216,1215,1214,1213,1212,1211,1210,1209,1208,1207,1206,1205,1204,1203,1202,1201,1200,
  1799,1798,1797,1796,1795,1794,1793,1792,1791,1790,1789,1788,1787,1786,1785,1784,1783,1782,1781,1780,1779,1778,1777,1776,1775,1774,1773,1772,1771,1770,1769,1768,1767,1766,1765,1764,1763,1762,1761,1760,1759,1758,1757,1756,1755,1754,1753,1752,1751,1750,1749,1748,1747,1746,1745,1744,1743,1742,1741,1740,1739,1738,1737,1736,1735,1734,1733,1732,1731,1730,1729,1728,1727,1726,1725,1724,1723,1722,1721,1720,1719,1718,1717,1716,1715,1714,1713,1712,1711,1710,1709,1708,1707,1706,1705,1704,1703,1702,1701,1700,1699,1698,1697,1696,1695,1694,1693,1692,1691,1690,1689,1688,1687,1686,1685,1684,1683,1682,1681,1680,1679,1678,1677,1676,1675,1674,1673,1672,1671,1670,1669,1668,1667,1666,1665,1664,1663,1662,1661,1660,1659,1658,1657,1656,1655,1654,1653,1652,1651,1650,1649,1648,1647,1646,1645,1644,1643,1642,1641,1640,1639,1638,1637,1636,1635,1634,1633,1632,1631,1630,1629,1628,1627,1626,1625,1624,1623,1622,1621,1620,1619,1618,1617,1616,1615,1614,1613,1612,1611,1610,1609,1608,1607,1606,1605,1604,1603,1602,1601,1600,1599,1598,1597,1596,1595,1594,1593,1592,1591,1590,1589,1588,1587,1586,1585,1584,1583,1582,1581,1580,1579,1578,1577,1576,1575,1574,1573,1572,1571,1570,1569,1568,1567,1566,1565,1564,1563,1562,1561,1560,1559,1558,1557,1556,1555,1554,1553,1552,1551,1550,1549,1548,1547,1546,1545,1544,1543,1542,1541,1540,1539,1538,1537,1536,1535,1534,1533,1532,1531,1530,1529,1528,1527,1526,1525,1524,1523,1522,1521,1520,1519,1518,1517,1516,1515,1514,1513,1512,1511,1510,1509,1508,1507,1506,1505,1504,1503,1502,1501,1500,
  2099,2098,2097,2096,2095,2094,2093,2092,2091,2090,2089,2088,2087,2086,2085,2084,2083,2082,2081,2080,2079,2078,2077,2076,2075,2074,2073,2072,2071,2070,2069,2068,2067,2066,2065,2064,2063,2062,2061,2060,2059,2058,2057,2056,2055,2054,2053,2052,2051,2050,2049,2048,2047,2046,2045,2044,2043,2042,2041,2040,2039,2038,2037,2036,2035,2034,2033,2032,2031,2030,2029,2028,2027,2026,2025,2024,2023,2022,2021,2020,2019,2018,2017,2016,2015,2014,2013,2012,2011,2010,2009,2008,2007,2006,2005,2004,2003,2002,2001,2000,1999,1998,1997,1996,1995,1994,1993,1992,1991,1990,1989,1988,1987,1986,1985,1984,1983,1982,1981,1980,1979,1978,1977,1976,1975,1974,1973,1972,1971,1970,1969,1968,1967,1966,1965,1964,1963,1962,1961,1960,1959,1958,1957,1956,1955,1954,1953,1952,1951,1950,1949,1948,1947,1946,1945,1944,1943,1942,1941,1940,1939,1938,1937,1936,1935,1934,1933,1932,1931,1930,1929,1928,1927,1926,1925,1924,1923,1922,1921,1920,1919,1918,1917,1916,1915,1914,1913,1912,1911,1910,1909,1908,1907,1906,1905,1904,1903,1902,1901,1900,1899,1898,1897,1896,1895,1894,1893,1892,1891,1890,1889,1888,1887,1886,1885,1884,1883,1882,1881,1880,1879,1878,1877,1876,1875,1874,1873,1872,1871,1870,1869,1868,1867,1866,1865,1864,1863,1862,1861,1860,1859,1858,1857,1856,1855,1854,1853,1852,1851,1850,1849,1848,1847,1846,1845,1844,1843,1842,1841,1840,1839,1838,1837,1836,1835,1834,1833,1832,1831,1830,1829,1828,1827,1826,1825,1824,1823,1822,1821,1820,1819,1818,1817,1816,1815,1814,1813,1812,1811,1810,1809,1808,1807,1806,1805,1804,1803,1802,1801,1800,
  2399,2398,2397,2396,2395,2394,2393,2392,2391,2390,2389,2388,2387,2386,2385,2384,2383,2382,2381,2380,2379,2378,2377,2376,2375,2374,2373,2372,2371,2370,2369,2368,2367,2366,2365,2364,2363,2362,2361,2360,2359,2358,2357,2356,2355,2354,2353,2352,2351,2350,2349,2348,2347,2346,2345,2344,2343,2342,2341,2340,2339,2338,2337,2336,2335,2334,2333,2332,2331,2330,2329,2328,2327,2326,2325,2324,2323,2322,2321,2320,2319,2318,2317,2316,2315,2314,2313,2312,2311,2310,2309,2308,2307,2306,2305,2304,2303,2302,2301,2300,2299,2298,2297,2296,2295,2294,2293,2292,2291,2290,2289,2288,2287,2286,2285,2284,2283,2282,2281,2280,2279,2278,2277,2276,2275,2274,2273,2272,2271,2270,2269,2268,2267,2266,2265,2264,2263,2262,2261,2260,2259,2258,2257,2256,2255,2254,2253,2252,2251,2250,2249,2248,2247,2246,2245,2244,2243,2242,2241,2240,2239,2238,2237,2236,2235,2234,2233,2232,2231,2230,2229,2228,2227,2226,2225,2224,2223,2222,2221,2220,2219,2218,2217,2216,2215,2214,2213,2212,2211,2210,2209,2208,2207,2206,2205,2204,2203,2202,2201,2200,2199,2198,2197,2196,2195,2194,2193,2192,2191,2190,2189,2188,2187,2186,2185,2184,2183,2182,2181,2180,2179,2178,2177,2176,2175,2174,2173,2172,2171,2170,2169,2168,2167,2166,2165,2164,2163,2162,2161,2160,2159,2158,2157,2156,2155,2154,2153,2152,2151,2150,2149,2148,2147,2146,2145,2144,2143,2142,2141,2140,2139,2138,2137,2136,2135,2134,2133,2132,2131,2130,2129,2128,2127,2126,2125,2124,2123,2122,2121,2120,2119,2118,2117,2116,2115,2114,2113,2112,2111,2110,2109,2108,2107,2106,2105,2104,2103,2102,2101,2100
  };

  uint16_t i = (y * strandLength) + x;
  uint16_t j = XYTable[i];
  return j;
}

// Changes an RGB pixel on all strands
void setPixelRGB(uint16_t dot, uint8_t r, uint8_t g, uint8_t b) {
  // For every strand in the band
  for(uint8_t strand=0; strand<strandNumber; strand++) {
    dot = dot % strandLength; // Account for overflow
    dot = XY(dot,strand); // Apply to matrix transoform
    // Assign value to LEDs
    leds[dot][0] = r;
    leds[dot][1] = g;
    leds[dot][2] = b;
  }
}

// Changes an HSV pixel on all strands
void setPixelHSV(uint16_t dot, uint8_t h, uint8_t s, uint8_t v) {
  // For every strand in the band
  for(uint8_t strand=0; strand<strandNumber; strand++) {
    dot = dot % strandLength; // Account for overflow
    dot = XY(dot,strand); // Apply to matrix transoform
    // Assign value to LEDs
    leds[dot] = CHSV(h,s,v);
  }
}

// Changes a section of pixels to a solid RGB color
void rgbZone(uint16_t led_start, uint16_t led_end, uint8_t r, uint8_t g, uint8_t b) {
  //Light each pixel in zone w/ overlap
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    setPixelRGB(dot, r, g, b);
  }
}

// Changes a section of pixels to a solid HSV color
void hsvZone(uint16_t led_start, uint16_t led_end, uint8_t h, uint8_t s, uint8_t v) {
  //Light each pixel in zone w/ overlap
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    setPixelHSV(dot,h,s,v);
  }
}

// Modifies a section of pixels by a color offset
void rgbOverlay(uint16_t led_start, uint16_t led_end, int r_offset, int g_offset, int b_offset) {
  //Light each pixel in zone  w/ overlap
  uint8_t r, g, b;
  for(uint16_t dot=led_start; dot<led_end; dot++) {
    dot = dot % strandLength; // Account for overflow
    r=leds[dot][0];
    g=leds[dot][1];
    b=leds[dot][2];

    r = qadd8(r,r_offset);
    g = qadd8(g,g_offset);
    b = qadd8(b,b_offset);
        
    setPixelRGB(dot, r, g, b);
  }
}

// Turns off all LED's
void clearLEDs() {
  for(int dot = 0; dot < strandLength; dot++) {
    setPixelRGB(dot, 0, 0, 0); 
  }
}

// Plasma glow effect with adjustable pallet
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

// Lightning Strike Effect
void lightning() {
  uint16_t dimmer = 1;
  uint16_t ledstart = random8(strandNumber*strandLength);                               // Determine starting location of flash
  uint16_t ledlen = random8(strandNumber*strandLength-ledstart);                        // Determine length of flash (not to go beyond strandLength-1)
  
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

// Modified Pacifica code to segment effect
// Add one layer of waves into the led array
void pacifica_one_layer(uint16_t ledStart, uint16_t ledEnd, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = ledStart; i < ledEnd; i++) {
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
void pacifica_add_whitecaps(uint16_t ledStart, uint16_t ledEnd) {
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = ledStart; i < ledEnd; i++) {
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
void pacifica_deepen_colors(uint16_t ledStart, uint16_t ledEnd) {
  for( uint16_t i = ledStart; i < ledEnd; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}

void pacifica_loop(uint16_t ledStart, uint16_t ledEnd) {
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
  rgbZone(ledStart, ledEnd, 2, 6, 10);

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer(ledStart, ledEnd, pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer(ledStart, ledEnd, pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer(ledStart, ledEnd, pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer(ledStart, ledEnd, pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps(ledStart, ledEnd);

  // Deepen the blues and greens a bit
  pacifica_deepen_colors(ledStart, ledEnd);
}
// End of Pacifica code

// Shaking effect at tempo
void bpm(uint8_t masterPalette, uint8_t BeatsPerMinute) {
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  CRGBPalette16 palette = PALETTE_SELECT[masterPalette];
  uint8_t beat = beatsin8(BeatsPerMinute, 64, 255);
  for( int i = 0; i < strandLength; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

// Sparkling effect
void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy(leds, strandNumber*strandLength, 10);
  int pos = random16(strandNumber*strandLength);
  leds[pos] += CHSV(gHue + random8(64), 200, 255);
}

// Sawtooth effect
void sawtooth(uint8_t masterPalette, uint8_t BeatsPerMinute) {
  int ms_per_beat = 60000/BeatsPerMinute; // 500ms per beat, where 60,000 = 60 seconds * 1000 ms 
  int ms_per_led = 60000/BeatsPerMinute/strandLength;
  int cur_led = ((millis() % ms_per_beat) / ms_per_led)%(strandLength); // Using millis to count up the strand, with %strandLength at the end as a safety factor.

  CRGBPalette16 palette = PALETTE_SELECT[masterPalette];
  TBlendType currentBlending = LINEARBLEND;
  if (cur_led == 0)
   fill_solid(leds, strandLength, CRGB::Black);
  else
    leds[XY(cur_led,0)] = ColorFromPalette(palette, cur_led, 255, currentBlending); // I prefer to use palettes instead of CHSV or CRGB assignments.
}

// Rainbow chase effect
int16_t bandLoc = 0;
uint8_t numBands = 8;
void RainbowChase(uint8_t speed, int8_t dir, uint8_t bands) {
  uint16_t bandWidth = strandLength / bands; // Number of pixels in color
  uint8_t wavelength = 255 / bands; // Wavelength (color in Hue) of band
  uint8_t satvalOffset = 128; // Start at half Hue
  uint16_t startled = bandLoc;
  uint8_t satval = satvalOffset;
  uint8_t satvalStep = (255-satvalOffset) / bandWidth;

  // Shift effect
  bandLoc += speed * dir;
  // Reset location if at end
  if (bandLoc>=strandLength) {
    bandLoc -= strandLength;
  } else if (bandLoc<0) {
    bandLoc += strandLength;
  }
  
  // Add Color
  for(uint8_t band=0; band<bands; band++) {
    startled = bandLoc+bandWidth*band;
    for(uint16_t dot=0; dot<bandWidth; dot++) {
      satval = dot*satvalStep+satvalOffset;
      setPixelHSV(startled+dot, wavelength*band, satval, satval);
    }
  }
}

// Racer params
uint16_t pinkLoc = 0;
uint16_t tealLoc = 0;
uint8_t pinkWidth = 20;
uint8_t tealWidth = 40;
void NeonRacers(uint8_t speed, uint8_t racerNumber, int8_t dir) {
  // Shift effect
  pinkLoc += speed * 2 * dir;  
  tealLoc += speed * dir;

  //Reset positions
  if (pinkLoc>=strandLength) {
    pinkLoc -= strandLength;
  } else if (pinkLoc<0) {
    pinkLoc += strandLength;
  }
  if (tealLoc>=strandLength) {
    tealLoc -= strandLength;
  } else if (tealLoc<0) {
    tealLoc += strandLength;
  }

  // Add Color
  clearLEDs();
  // Calculate space needed between racers to fill strip
  uint16_t pinkSpace = (strandLength-racerNumber*pinkWidth)/racerNumber+pinkWidth;
  uint16_t tealSpace = (strandLength-racerNumber*tealWidth)/racerNumber+tealWidth;
  for(uint8_t racer=0; racer<racerNumber; racer++) {
    rgbOverlay(pinkLoc+pinkSpace*racer, pinkLoc+pinkWidth+pinkSpace*racer, 250, 10, 20);
    rgbOverlay(tealLoc+tealSpace*racer, tealLoc+tealWidth+tealSpace*racer, 20, 10, 250);
  }
}

// Bouncer params
uint16_t redLoc = 0;
uint16_t blueLoc = 0;
uint16_t greenLoc = 0;
uint8_t redWidth = 40;
uint8_t greenWidth = 20;
uint8_t blueWidth = 10;
int8_t redDir = 1;
int8_t blueDir = 1;
int8_t greenDir = 1;
void RGBBouncers(uint8_t speed, uint8_t racerNumber) {
  // Check for direction switch
  if (redLoc<=speed) {
    redDir = 1;
  } else if (redLoc>=strandLength-redWidth) {
    redDir = -1;
  }
  if (greenLoc<=speed*2) {
    greenDir = 1;
  } else if (greenLoc>=strandLength-greenWidth) {
    greenDir = -1;
  }
  if (blueLoc<=speed*3) {
    blueDir = 1;
  } else if (blueLoc>=strandLength-blueWidth) {
    blueDir = -1;
  }
  redLoc += speed*blueDir;
  greenLoc += speed*2*blueDir;
  blueLoc += speed*3*blueDir;
  
  // Add Color
  clearLEDs();
  rgbOverlay(redLoc, redLoc+redWidth, 255, 0, 0);
  rgbOverlay(greenLoc, greenLoc+greenWidth, 0, 255, 0);
  rgbOverlay(blueLoc, blueLoc+blueWidth, 0, 0, 255);
}

uint8_t warpSpeed = 5;
uint16_t warpLoc = 0;
void Countdown() {
  // Shift band
  warpLoc += warpSpeed;
  // Reset location if at end and speed up
  if (warpLoc>=strandLength) {
    warpLoc -= strandLength;
    if (warpSpeed<30) {
      warpSpeed += 5;
    }
    Serial.print("Warp speed: ");
    Serial.println(warpSpeed);
  }

  clearLEDs();
  rgbZone(warpLoc, warpLoc+75, 255, 255, 255);
}
