#include "game.h"
#include "resources.h"
#include "fmt/core.h"
#include "fmt/printf.h"
#include "system.h"

namespace Game
{
    
//const Common::Point GScrOff(140, 0);
const Common::Point GScrOff(0, 0);

const std::map<uint8_t, vec3i> Engine::_mapsLights = 
{{ 42, {-70,   0,   0} },
 { 43, { -1,  -1,  -1} },
 { 44, {-20,  -1, -70} },
 { 45, {  0, -50, -50} },
 { 46, { -1,  -1,  -1} },
 { 47, { -1,  -1,  -1} },
 { 48, { -1,  -1,  -1} },
 { 49, { -1,  -1,  -1} },
 { 50, { -1,  -1,  -1} }};

const std::array<int32_t, 13> Engine::_lightPhazes = 
{      0,   1800,   1830,   3570,
    3600,   5400,   8100,  11700,
   11880,  11910,  16155,  16200,
   18900};
 
const std::vector<Common::Rect> Engine::_mainMenuBoxes =
{
    //{233, 130, 425, 161},
    {290, 161, 590, 210},
    //{231, 164, 404, 199},
    {288, 205, 504, 250},
    //{212, 200, 390, 240},
    {265, 250, 488, 303},
    //{224, 241, 352, 281},
    {280, 303, 440, 354},
    //{215, 282, 333, 321},
    {268, 353, 416, 402},
    //{222, 327, 300, 371},
    {276, 409, 366, 464}
};

const std::vector<Common::Rect> Engine::_saveMenuBoxes = 
{
    //{140, 69, 510, 100},
    {175, 86, 638, 125},
    //{140, 100, 510, 130},
    {175, 125, 638, 162},
    //{140, 131, 510, 160},
    {175, 164, 638, 200},
    //{140, 162, 510, 190},
    {175, 203, 638, 238},
    //{140, 193, 510, 220},
    {175, 241, 638, 275},
    //{140, 224, 510, 250},
    {175, 280, 638, 313},
    //{140, 255, 510, 280},
    {175, 319, 638, 350},
    //{140, 286, 510, 310},
    {175, 358, 638, 388},
    //{140, 317, 510, 340},
    {175, 396, 638, 425},
    //{140, 348, 510, 370},
    {175, 435, 638, 463} 
};

const int8_t Engine::EqLookUp1[8][6] =
{{ESLT_3, ESLT_2, ESLT_4, ESLT_1, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_4, ESLT_0, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_4, ESLT_0, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_4, ESLT_0, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_4, ESLT_0, ESLT_5, ESLT_1},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_4, ESLT_1, ESLT_0},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_4, ESLT_1, ESLT_0},
 {ESLT_3, ESLT_2, ESLT_4, ESLT_5, ESLT_1, ESLT_0}};

const int8_t Engine::EqLookUp2[4][8][6] = {
{{ESLT_3, ESLT_2, ESLT_0, ESLT_1, ESLT_4, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_0, ESLT_1, ESLT_4, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_0, ESLT_2, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_5, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_0, ESLT_2, ESLT_1, ESLT_4, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_0, ESLT_1, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_0, ESLT_4, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_0, ESLT_4, ESLT_1, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_0, ESLT_4, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_0, ESLT_5, ESLT_4, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_2, ESLT_0, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_0, ESLT_1, ESLT_2},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_4, ESLT_0, ESLT_1},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_0, ESLT_2, ESLT_1, ESLT_4, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_2, ESLT_1, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_4, ESLT_1, ESLT_0},
 {ESLT_3, ESLT_5, ESLT_0, ESLT_4, ESLT_1, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5}} };


const int8_t Engine::EqLookUp3[4][8][6] = {
{{ESLT_3, ESLT_2, ESLT_0, ESLT_4, ESLT_5, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_2, ESLT_4, ESLT_0, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_0, ESLT_2, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_1, ESLT_4, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_0, ESLT_2, ESLT_1, ESLT_4, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_0, ESLT_1, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_0, ESLT_4, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_0, ESLT_4, ESLT_1, ESLT_2},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_2, ESLT_1, ESLT_4, ESLT_0, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_0, ESLT_4, ESLT_1, ESLT_5},
 {ESLT_3, ESLT_2, ESLT_0, ESLT_5, ESLT_4, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_2, ESLT_0, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_0, ESLT_1, ESLT_2},
 {ESLT_3, ESLT_2, ESLT_5, ESLT_4, ESLT_0, ESLT_1},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_0, ESLT_2, ESLT_1, ESLT_4, ESLT_5}},
{{ESLT_3, ESLT_2, ESLT_0, ESLT_4, ESLT_5, ESLT_1},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_2, ESLT_1, ESLT_0},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_2, ESLT_1, ESLT_0},
 {ESLT_3, ESLT_5, ESLT_4, ESLT_1, ESLT_0, ESLT_2},
 {ESLT_3, ESLT_4, ESLT_2, ESLT_1, ESLT_0, ESLT_5},
 {ESLT_3, ESLT_4, ESLT_1, ESLT_0, ESLT_2, ESLT_5},
 {ESLT_3, ESLT_1, ESLT_0, ESLT_2, ESLT_4, ESLT_5},
 {ESLT_3, ESLT_0, ESLT_2, ESLT_1, ESLT_4, ESLT_5}} };


const std::array<Engine::WeapArmorItemInfo, 70> Engine::ArmorWeaponInfo = {{
//ID    Dmg    unk1   unk2     unk3   unk4     unk5    unk6    unk7  LootImgID SprImage
{0,	112,	30,	5,	28,	1,	80,	500,	152,	166,	4},
{1,	220,	60,	5,	55,	1,	320,	700,	145,	166,	4},
{2,	332,	85,	5,	83,	1,	740,	900,	143,	166,	4},
{3,	108,	6,	6,	21,	1,	30,	500,	31,	154,	0},
{4,	212,	11,	6,	45,	1,	180,	1200,	30,	154,	0},
{5,	324,	18,	6,	74,	1,	510,	2200,	29,	154,	0},
{6,	64,	16,	2,	15,	12,	8,	400,	37,	155,	2},
{7,	88,	33,	2,	36,	18,	70,	700,	42,	155,	2},
{8,	112,	56,	2,	65,	25,	220,	1100,	43,	155,	2},
{9,	84,	22,	5,	21,	1,	40,	300,	151,	166,	4},
{10,	180,	50,	5,	45,	1,	220,	600,	147,	166,	4},
{11,	296,	76,	5,	74,	1,	580,	1000,	144,	166,	4},
{12,	140,	7,	6,	28,	1,	60,	1400,	27,	154,	0},
{13,	256,	14,	6,	55,	1,	270,	900,	35,	154,	0},
{14,	368,	20,	6,	83,	1,	660,	2500,	33,	154,	0},
{15,	72,	20,	2,	21,	12,	20,	500,	38,	155,	2},
{16,	96,	36,	2,	45,	18,	110,	800,	41,	155,	2},
{17,	120,	60,	2,	74,	25,	290,	1300,	44,	155,	2},
{18,	60,	16,	5,	3,	1,	15,	200,	150,	166,	4},
{19,	144,	40,	5,	36,	1,	140,	500,	148,	166,	4},
{20,	264,	70,	5,	65,	1,	440,	800,	149,	166,	4},
{21,	80,	5,	6,	10,	1,	10,	1200,	26,	154,	0},
{22,	172,	9,	6,	36,	1,	110,	700,	32,	154,	0},
{23,	284,	13,	6,	65,	1,	380,	2000,	28,	154,	0},
{24,	80,	24,	2,	28,	12,	40,	600,	39,	155,	2},
{25,	104,	40,	2,	55,	18,	160,	900,	40,	155,	2},
{26,	128,	64,	2,	83,	25,	370,	1500,	45,	155,	2},
{27,	380,	120,	5,	300,	1,	1,	700,	146,	166,	4},
{28,	480,	70,	6,	95,	1,	1,	800,	34,	154,	0},
{29,	160,	80,	2,	95,	30,	1,	500,	36,	155,	2},
{30,	1,	1,	0,	0,	0,	1,	30,	23,	153,	-1},
{31,	2,	2,	0,	0,	0,	3,	40,	22,	153,	-1},
{32,	3,	3,	0,	0,	0,	7,	60,	24,	153,	-1},
{33,	4,	4,	0,	0,	0,	20,	60,	25,	153,	-1},
{34,	24,	12,	6,	12,	0,	120,	2700,	54,	158,	17},
{35,	80,	45,	6,	36,	0,	420,	4200,	55,	158,	17},
{36,	128,	64,	6,	65,	0,	640,	7700,	50,	159,	8},
{37,	28,	14,	5,	23,	0,	140,	3200,	139,	165,	20},
{38,	72,	36,	5,	42,	0,	360,	4400,	140,	165,	22},
{39,	104,	52,	5,	63,	0,	520,	5100,	141,	165,	22},
{40,	128,	68,	5,	84,	0,	680,	6400,	142,	165,	22},
{41,	8,	6,	2,	19,	0,	40,	300,	66,	161,	13},
{42,	20,	15,	2,	35,	0,	100,	800,	67,	161,	16},
{43,	48,	36,	2,	56,	0,	240,	1100,	68,	161,	13},
{44,	72,	54,	2,	77,	0,	360,	1800,	69,	161,	16},
{45,	56,	28,	6,	28,	0,	280,	3000,	49,	158,	17},
{46,	108,	54,	6,	55,	0,	540,	5400,	51,	158,	17},
{47,	160,	80,	6,	83,	0,	800,	10500,	50,	159,	8},
{48,	12,	6,	5,	10,	0,	60,	2500,	130,	165,	20},
{49,	40,	20,	5,	28,	0,	200,	3600,	131,	165,	6},
{50,	88,	44,	5,	49,	0,	440,	4200,	132,	165,	20},
{51,	112,	56,	5,	70,	0,	560,	5800,	133,	165,	18},
{52,	4,	3,	2,	8,	0,	20,	400,	57,	161,	9},
{53,	16,	12,	2,	28,	0,	80,	700,	58,	161,	10},
{54,	36,	27,	2,	49,	0,	180,	1200,	59,	161,	11},
{55,	64,	48,	2,	70,	0,	320,	1500,	60,	161,	12},
{56,	36,	18,	6,	21,	0,	180,	2800,	56,	158,	17},
{57,	96,	48,	6,	45,	0,	480,	4800,	53,	158,	17},
{58,	144,	77,	6,	74,	0,	720,	9200,	50,	159,	8},
{59,	20,	10,	5,	19,	0,	100,	1900,	135,	165,	20},
{60,	56,	28,	5,	35,	0,	280,	3100,	136,	165,	20},
{61,	96,	48,	5,	56,	0,	480,	4400,	137,	165,	18},
{62,	120,	60,	5,	77,	0,	600,	5300,	138,	165,	18},
{63,	12,	9,	2,	23,	0,	60,	400,	62,	161,	15},
{64,	28,	21,	2,	42,	0,	140,	900,	63,	161,	15},
{65,	56,	52,	2,	63,	0,	280,	1800,	64,	161,	14},
{66,	80,	60,	2,	84,	0,	400,	2100,	65,	161,	14},
{67,	200,	110,	6,	95,	0,	1,	5000,	52,	159,	8},
{68,	160,	95,	5,	95,	0,	1,	4000,	134,	165,	18},
{69,	100,	80,	2,	95,	0,	1,	1000,	61,	161,	16} }};

const std::array<Engine::AccessorieItemInfo, 9> Engine::AcessoriesInfo = {{
//ID  BonusID Weight  ImgID  LootImgID
{0,	80,	200,	84,	162},
{1,	230,	200,	85,	162},
{2,	470,	300,	86,	162},
{3,	50,	50,	46,	157},
{4,	180,	50,	47,	157},
{5,	400,	100,	48,	157},
{6,	30,	10,	127,	164},
{7,	120,	10,	128,	164},
{8,	300,	20,	129,	164} }};

const std::array<Engine::AccessorieItemInfo, 14> Engine::AlchemiesInfo = {{
//ID  BonusID Weight  ImgID  LootImgID
{0,	10,	100,	81,	167},
{1,	2,	1000,	83,	160},
{2,	1,	1000,	82,	168},
{3,	1,	100,	80,	163},
{4,	0,	500,	79,	163},
{5,	-2,	500,	71,	163},
{6,	-11,	500,	70,	163},
{7,	-3,	500,	72,	163},
{8,	-12,	500,	73,	163},
{9,	-4,	500,	77,	163},
{10,	-5,	500,	78,	163},
{11,	-12,	500,	74,	163},
{12,	-14,	500,	76,	163},
{13,	-15,	500,	75,	163} }};

const std::array<Engine::AccessorieItemInfo, 54> Engine::MiscItemsInfo = {{
//ID  BonusID Weight  ImgID  LootImgID
{0,	100,	300,	87,	163},
{1,	300,	300,	87,	163},
{2,	200,	300,	87,	163},
{3,	400,	300,	87,	163},
{4,	700,	300,	87,	163},
{5,	900,	1000,	88,	163},
{6,	250,	300,	87,	163},
{7,	990,	1000,	89,	163},
{8,	50,	200,	90,	163},
{9,	50,	300,	87,	163},
{10,	0,	300,	91,	163},
{11,	0,	-10000,	92,	163},
{12,	0,	1000,	93,	163},
{13,	0,	2000,	94,	163},
{14,	0,	1000,	95,	163},
{15,	0,	1000,	95,	163},
{16,	0,	1000,	95,	163},
{17,	0,	1000,	95,	163},
{18,	0,	1000,	95,	163},
{19,	0,	1000,	95,	163},
{20,	0,	1000,	95,	163},
{21,	0,	1000,	95,	163},
{22,	0,	500,	96,	163},
{23,	0,	600,	97,	163},
{24,	0,	1000,	98,	163},
{25,	0,	100,	99,	163},
{26,	0,	100,	100,	163},
{27,	0,	100,	101,	163},
{28,	0,	300,	87,	163},
{29,	0,	500,	102,	163},
{30,	0,	500,	102,	163},
{31,	0,	500,	103,	163},
{32,	0,	0,	104,	163},
{33,	0,	100,	105,	163},
{34,	0,	100,	106,	163},
{35,	0,	100,	107,	163},
{36,	0,	1000,	108,	163},
{37,	0,	100,	109,	163},
{38,	0,	100,	110,	163},
{39,	0,	200,	111,	163},
{40,	0,	300,	112,	163},
{41,	0,	200,	113,	163},
{42,	0,	200,	114,	163},
{43,	0,	200,	115,	163},
{44,	0,	200,	116,	163},
{45,	0,	100,	117,	163},
{46,	0,	50,	118,	163},
{47,	0,	50,	119,	163},
{48,	0,	500,	120,	163},
{49,	0,	700,	121,	163},
{50,	0,	1000,	122,	156},
{51,	0,	20000,	123,	163},
{52,	0,	200,	124,	163},
{53,	0,	200,	125,	163} }};

const std::array<Engine::BonusSet, 74> Engine::BonusesInfo = {{
//unk1  unk3   unk4     Bonuses
{15,	0,	0,	{{8,1,4}, {0,0,0}, {0,0,0}}},
{45,	0,	0,	{{8,1,12}, {0,0,0}, {0,0,0}}},
{-15,	0,	0,	{{8,1,-4}, {0,0,0}, {0,0,0}}},
{75,	0,	0,	{{8,1,20}, {0,0,0}, {0,0,0}}},
{105,	0,	0,	{{8,1,28}, {0,0,0}, {0,0,0}}},
{-45,	0,	0,	{{8,1,-12}, {0,0,0}, {0,0,0}}},
{150,	0,	0,	{{8,1,40}, {0,0,0}, {0,0,0}}},
{-75,	0,	0,	{{8,1,-20}, {0,0,0}, {0,0,0}}},
{180,	0,	0,	{{8,1,48}, {0,0,0}, {0,0,0}}},
{-150,	0,	0,	{{8,1,-40}, {0,0,0}, {0,0,0}}},
{30,	0,	0,	{{5,1,1}, {0,0,0}, {0,0,0}}},
{60,	0,	0,	{{5,1,2}, {0,0,0}, {0,0,0}}},
{-30,	0,	0,	{{5,1,-1}, {0,0,0}, {0,0,0}}},
{90,	0,	0,	{{5,1,3}, {0,0,0}, {0,0,0}}},
{150,	0,	0,	{{5,1,4}, {0,0,0}, {0,0,0}}},
{-90,	0,	0,	{{5,1,-3}, {0,0,0}, {0,0,0}}},
{210,	0,	0,	{{5,1,5}, {0,0,0}, {0,0,0}}},
{-150,	0,	0,	{{5,1,-4}, {0,0,0}, {0,0,0}}},
{300,	0,	0,	{{5,1,6}, {0,0,0}, {0,0,0}}},
{-300,	0,	0,	{{5,1,-5}, {0,0,0}, {0,0,0}}},
{25,	0,	0,	{{2,1,1}, {0,0,0}, {0,0,0}}},
{50,	0,	0,	{{2,1,2}, {0,0,0}, {0,0,0}}},
{-25,	0,	0,	{{2,1,-1}, {0,0,0}, {0,0,0}}},
{75,	0,	0,	{{2,1,3}, {0,0,0}, {0,0,0}}},
{125,	0,	0,	{{2,1,4}, {0,0,0}, {0,0,0}}},
{-75,	0,	0,	{{2,1,-3}, {0,0,0}, {0,0,0}}},
{175,	0,	0,	{{2,1,5}, {0,0,0}, {0,0,0}}},
{-125,	0,	0,	{{2,1,-4}, {0,0,0}, {0,0,0}}},
{250,	0,	0,	{{2,1,6}, {0,0,0}, {0,0,0}}},
{-250,	0,	0,	{{2,1,-5}, {0,0,0}, {0,0,0}}},
{40,	0,	0,	{{6,1,1}, {0,0,0}, {0,0,0}}},
{80,	0,	0,	{{6,1,2}, {0,0,0}, {0,0,0}}},
{-40,	0,	0,	{{6,1,-1}, {0,0,0}, {0,0,0}}},
{120,	0,	0,	{{6,1,3}, {0,0,0}, {0,0,0}}},
{200,	0,	0,	{{6,1,4}, {0,0,0}, {0,0,0}}},
{-120,	0,	0,	{{6,1,-3}, {0,0,0}, {0,0,0}}},
{280,	0,	0,	{{6,1,5}, {0,0,0}, {0,0,0}}},
{-200,	0,	0,	{{6,1,-4}, {0,0,0}, {0,0,0}}},
{400,	0,	0,	{{6,1,6}, {0,0,0}, {0,0,0}}},
{-400,	0,	0,	{{6,1,-5}, {0,0,0}, {0,0,0}}},
{20,	0,	0,	{{7,1,4}, {0,0,0}, {0,0,0}}},
{60,	0,	0,	{{7,1,12}, {0,0,0}, {0,0,0}}},
{-20,	0,	0,	{{7,1,-4}, {0,0,0}, {0,0,0}}},
{100,	0,	0,	{{7,1,20}, {0,0,0}, {0,0,0}}},
{140,	0,	0,	{{7,1,28}, {0,0,0}, {0,0,0}}},
{-60,	0,	0,	{{7,1,-12}, {0,0,0}, {0,0,0}}},
{200,	0,	0,	{{7,1,40}, {0,0,0}, {0,0,0}}},
{-100,	0,	0,	{{7,1,-20}, {0,0,0}, {0,0,0}}},
{240,	0,	0,	{{7,1,48}, {0,0,0}, {0,0,0}}},
{-200,	0,	0,	{{7,1,-40}, {0,0,0}, {0,0,0}}},
{930,	1,	40,	{{1,1,30}, {0,0,0}, {0,0,0}}},
{1350,	2,	45,	{{2,1,15}, {0,0,0}, {0,0,0}}},
{1900,	3,	50,	{{3,1,15}, {0,0,0}, {0,0,0}}},
{4900,	4,	90,	{{4,1,80}, {0,0,0}, {0,0,0}}},
{1500,	5,	50,	{{5,1,15}, {0,0,0}, {0,0,0}}},
{1600,	6,	50,	{{6,1,20}, {0,0,0}, {0,0,0}}},
{1100,	6,	65,	{{7,1,20}, {0,0,0}, {0,0,0}}},
{900,	5,	65,	{{8,1,30}, {0,0,0}, {0,0,0}}},
{2200,	1,	30,	{{9,1,20}, {0,0,0}, {0,0,0}}},
{3200,	2,	100,	{{5,1,300}, {0,0,0}, {0,0,0}}},
{2900,	3,	35,	{{2,0,100}, {6,1,-20}, {0,0,0}}},
{3700,	3,	25,	{{3,0,40}, {5,0,30}, {6,0,30}}},
{2500,	6,	25,	{{2,0,30}, {5,0,30}, {6,0,40}}},
{800,	3,	40,	{{2,1,10}, {3,1,-10}, {5,1,10}}},
{400,	3,	50,	{{2,1,-10}, {3,1,10}, {5,1,-10}}},
{600,	5,	40,	{{2,1,10}, {5,1,-10}, {6,1,10}}},
{700,	2,	40,	{{1,1,20}, {2,1,-10}, {5,1,10}}},
{900,	7,	70,	{{7,1,30}, {8,1,-15}, {0,0,0}}},
{300,	5,	30,	{{2,1,10}, {5,1,-10}, {0,0,0}}},
{450,	2,	30,	{{2,1,-10}, {5,1,10}, {0,0,0}}},
{1750,	6,	30,	{{1,1,15}, {3,1,10}, {0,0,0}}},
{200,	2,	40,	{{2,1,-10}, {5,1,-10}, {7,1,40}}},
{1000,	7,	80,	{{2,1,10}, {7,1,-30}, {8,1,50}}},
{3150,	6,	50,	{{1,1,25}, {6,1,30}, {0,0,0}}} }};

const std::array<Engine::BldInfo, 42> Engine::BuildingInfo {{
//ID NeededPlotnicLvl      BurnPeriod2
//        GraphObjectOffset            __unk3        __unk5
//                  BuildPeriod               __unk4
{0,	1,	7,	24,	24,	-32,	224,	7},
{1,	1,	53,	24,	24,	258,	128,	5},
{2,	1,	416,	12,	12,	304,	112,	5},
{3,	1,	274,	12,	12,	-144,	160,	7},
{4,	1,	444,	6,	6,	0,	112,	6},
{5,	1,	21,	18,	18,	192,	272,	5},
{6,	1,	430,	10,	10,	144,	128,	5},
{7,	3,	60,	24,	24,	160,	176,	5},
{8,	3,	224,	24,	24,	-16,	224,	7},
{9,	3,	231,	24,	24,	288,	128,	5},
{10,	1,	238,	24,	24,	270,	260,	5},
{11,	1,	245,	24,	24,	-272,	300,	7},
{12,	1,	252,	24,	24,	258,	200,	5},
{13,	1,	259,	24,	24,	-256,	192,	7},
{14,	5,	402,	1,	1,	0,	0,	-1},
{15,	5,	409,	1,	1,	0,	0,	-1},
{16,	5,	95,	6,	6,	0,	0,	-1},
{17,	5,	102,	6,	6,	0,	0,	-1},
{18,	5,	109,	6,	6,	0,	0,	-1},
{19,	3,	141,	12,	45,	0,	0,	-1},
{20,	3,	148,	12,	45,	0,	0,	-1},
{21,	3,	155,	12,	45,	0,	0,	-1},
{22,	3,	162,	12,	45,	0,	0,	-1},
{23,	3,	169,	12,	45,	0,	0,	-1},
{24,	3,	176,	12,	45,	0,	0,	-1},
{25,	3,	183,	12,	45,	0,	0,	-1},
{26,	3,	190,	12,	45,	0,	0,	-1},
{27,	1,	14,	12,	12,	0,	0,	-1},
{28,	1,	28,	12,	12,	0,	0,	-1},
{29,	1,	39,	12,	12,	0,	0,	-1},
{30,	1,	46,	12,	12,	0,	0,	-1},
{31,	1,	423,	6,	6,	0,	0,	-1},
{32,	1,	67,	48,	48,	0,	0,	-1},
{33,	1,	81,	12,	12,	0,	0,	-1},
{34,	1,	88,	48,	48,	0,	0,	-1},
{35,	1,	437,	6,	6,	0,	0,	-1},
{36,	1,	0,	6,	6,	0,	0,	-1},
{37,	1,	74,	6,	6,	0,	0,	-1},
{38,	1,	366,	6,	6,	0,	0,	-1},
{39,	1,	373,	6,	6,	0,	0,	-1},
{40,	1,	380,	6,	6,	0,	0,	-1},
{41,	1,	387,	6,	6,	0,	0,	-1}}};


const int Engine::FlameAnims[7][2] = 
{ {0, 10},
  {11, 21},
  {22, 32},
  {33, 43},
  {44, 54},
  {55, 59},
  {60, 73} };

const std::array<Common::Point, 8> Engine::O45ad30 = 
{{ 
    {-70, -65},
    {-48, -99},
    { 12, -85},
    { 68, -90},
    { 80, -58},
    { 52, -26},
    {-8,  -10},
    {-60, -31}}};

const std::array<Engine::SpecialDmg, 18> Engine::SpecialDamages =
{{
//ID    DMG     CLASSID
{0,	0,	0x0},
{1,	5000,	0x55},
{2,	500,	0x41},
{3,	3000,	0x46},
{4,	2000,	0x4d},
{5,	3000,	0x4e},
{6,	3500,	0x49},
{7,	4000,	0x4b},
{8,	4000,	0x4c},
{9,	4000,	0x4a},
{10,	4500,	0x45},
{11,	5000,	0x48},
{12,	6000,	0x42},
{13,	7000,	0x47},
{14,	8000,	0x43},
{15,	9000,	0x44},
{16,	2000,	0x4f},
{17,	3000,	0x50}
}};

void Engine::FreeMenuImages()
{
    for(GFX::Image *img : _menuImages)
        delete img;
    
    _menuImages.clear();
    
    if (_bkgImage)
        delete _bkgImage;
    
    _bkgImage = nullptr;
}

bool Engine::LoadMainMenuImages()
{
    _imgQueue1Count = 0;
    _imgQueue2Count = 0;
    _textQueueCount = 0;
    
    printf("Incomplete %s\n", __PRETTY_FUNCTION__);
    
    std::string tmp = fmt::format("MENU{:d}{:s}.RES", SCREENRESX, _langLiter);
    
    FSMgr::File f = FSMgr::Mgr::ReadFile(tmp);
    if (!f)
    {
        System::PostQuitMessage();
        fmt::printf("Menu file %s not found\n", tmp);
        return false;
    }
    
    FreeMenuImages();
    
    f->seek(4, 0);
    uint32_t entrySize = f->readU32L();
    size_t pos = f->tell();
    
    _bkgImage = Resources::LoadRL16BitImage(f.get());
    
    ImgQueue1(_bkgImage, Common::Point(), Common::Rect(_screenSize));

    f->seek(pos + entrySize, 0);
    for (int32_t i = 0; i < 14; ++i)
    {
        f->seek(4, 1);
        entrySize = f->readU32L();
        pos = f->tell();
        _menuImages.push_back( Resources::LoadRL16BitImage(f.get()) );
        f->seek(pos + entrySize, 0);
    }

    return true;
}


}