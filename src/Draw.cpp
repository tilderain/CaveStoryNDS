// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Draw.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Debug.h"
#include "Draw.h"
#include "Main.h"
#include "Input.h"
#include "Tags.h"

#include "nds.h"
#include <maxmod9.h>

#include "malloc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#include "lodepng.h"

#include "gl2d.h"

#include "Game.h"
#include "Sound.h"

#include "fopen.h"

#include "Sound.h"
#include "MiniMap.h"

struct VramSlot
{
	SurfaceID id;
	int atlas;
	int x;
	int y; 
};

RECT grcGame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
RECT grcFull = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SURFACE surf[SURFACE_ID_MAX];

PrintConsole bottomScreen;


#ifndef JAPANESE
static char* font_letters[3] = {
	" !\"#$%&`()*+,-./0123456789:;<=>?",
	"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]%_",
	"'abcdefghijklmnopqrstuvwxyz{|}~"
};
#else
static char* font_letters[] = {
" !\"#$%&`()*+,-./0123456789:;<=>?",
"@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_",
"'abcdefghijklmnopqrstuvwxyz{|}~~"
};

static unsigned char font_letters_shift_jis[] =
{


0x81,0x58, 0x81,0x41, 0x81,0x75, 0x81,0x76, 0x81,0x42, 0x82,0x92, 0x82,0x51, 0x81,0x7c, 0x82,0x56, 0x82,0x52, 0x81,0x69, 0x81,0x7b, 0x82,0x53, 0x81,0x44, 0x82,0x9a, 0x82,0x55, 0x82,0x6c, 0x81,0x60, 0x81,0x48, 0x82,0x77, 0x82,0x50, 0x82,0x58, 0x82,0x63, 0x82,0x6f, 0x81,0x49, 0x81,0x6a, 0x81,0x40, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 
0x83,0x57, 0x83,0x7a, 0x83,0x6c, 0x83,0x4e, 0x83,0x72, 0x83,0x87, 0x83,0x47, 0x83,0x5e, 0x83,0x8f, 0x83,0x6b, 0x83,0x70, 0x83,0x86, 0x83,0x7e, 0x83,0x51, 0x83,0x54, 0x83,0x43, 0x83,0x49, 0x83,0x67, 0x83,0x8d, 0x83,0x48, 0x83,0x4b, 0x83,0x7b, 0x83,0x5d, 0x83,0x69, 0x83,0x5f, 0x83,0x88, 0x83,0x5b, 0x83,0x42, 0x83,0x6d, 0x83,0x50, 0x83,0x4a, 0x83,0x46, 
0x83,0x7d, 0x83,0x6a, 0x83,0x80, 0x83,0x79, 0x83,0x45, 0x83,0x40, 0x83,0x85, 0x81,0x5b, 0x83,0x7c, 0x83,0x93, 0x81,0x45, 0x83,0x4c, 0x83,0x55, 0x83,0x53, 0x83,0x77, 0x83,0x4d, 0x83,0x6f, 0x83,0x8a, 0x83,0x56, 0x83,0x76, 0x83,0x60, 0x83,0x75, 0x83,0x4f, 0x83,0x8c, 0x83,0x84, 0x83,0x71, 0x83,0x6e, 0x83,0x94, 0x83,0x82, 0x83,0x44, 0x83,0x73, 0x83,0x81, 
0x83,0x89, 0x83,0x68, 0x83,0x8b, 0x83,0x5c, 0x83,0x96, 0x83,0x5a, 0x83,0x64, 0x83,0x52, 0x83,0x58, 0x83,0x59, 0x83,0x66, 0x83,0x83, 0x83,0x74, 0x83,0x63, 0x83,0x41, 0x83,0x92, 0x83,0x65, 0x83,0x78, 0x83,0x62, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 
0x82,0xe6, 0x82,0xc4, 0x82,0xa9, 0x82,0xe9, 0x82,0xa6, 0x82,0xb7, 0x82,0xa0, 0x82,0xbf, 0x82,0xa4, 0x82,0xdc, 0x82,0xc5, 0x82,0xe4, 0x82,0xae, 0x82,0xa7, 0x82,0xc0, 0x82,0xe8, 0x82,0xca, 0x82,0xa8, 0x82,0xe7, 0x82,0xb3, 0x82,0xd6, 0x82,0xc7, 0x82,0xad, 0x82,0xeb, 0x82,0xe2, 0x82,0xf1, 0x82,0xdd, 0x82,0xb8, 0x82,0xdb, 0x82,0xed, 0x82,0xb1, 0x82,0xc6, 
0x82,0xc3, 0x82,0xcb, 0x82,0xc1, 0x82,0xb2, 0x82,0xa3, 0x82,0xda, 0x82,0xa2, 0x82,0xa5, 0x82,0xd3, 0x82,0xd8, 0x82,0xcc, 0x82,0xba, 0x82,0xd9, 0x82,0xcf, 0x82,0xea, 0x82,0xce, 0x82,0xe1, 0x82,0xdf, 0x82,0xa1, 0x82,0xc8, 0x82,0xaa, 0x82,0xac, 0x82,0xab, 0x82,0xb9, 0x82,0xd4, 0x82,0xc2, 0x82,0xf0, 0x82,0xb0, 0x82,0xc9, 0x82,0xd0, 0x82,0xb5, 0x82,0xbe, 
0x82,0xbc, 0x82,0x9f, 0x82,0xb4, 0x82,0xbd, 0x82,0xaf, 0x82,0xe5, 0x82,0xb6, 0x82,0xd7, 0x82,0xcd, 0x82,0xde, 0x82,0xe0, 0x82,0xbb, 0x82,0xd1, 0x8b,0x9f, 0x87,0x56, 0x87,0x54, 0x81,0x46, 0xa5,0xa5, 0x81,0xf4, 0x81,0xa1, 0x81,0xaa, 0x87,0x57, 0x81,0xab, 0x81,0x63, 0x81,0xa9, 0x87,0x55, 0x81,0xa8, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 
0x92,0x81, 0x90,0xe0, 0x8b,0x46, 0x91,0x9f, 0x88,0xea, 0x89,0xf1, 0x93,0x79, 0x96,0xc6, 0x91,0xfc, 0x91,0xcf, 0x93,0xc1, 0x91,0xd5, 0x92,0x40, 0x95,0xca, 0x8d,0x55, 0x8a,0xae, 0x8e,0xd8, 0x93,0x87, 0x90,0xed, 0x93,0x9e, 0x8a,0xee, 0x89,0xef, 0x8c,0x92, 0x8d,0x4e, 0x92,0xbe, 0x93,0xfc, 0x91,0x66, 0x90,0x67, 0x8f,0xe4, 0x8c,0x82, 0x8f,0xb0, 0x8f,0x57, 
0x89,0xf7, 0x91,0x9d, 0x95,0x69, 0x90,0xf3, 0x8b,0x9b, 0x97,0x46, 0x8f,0x80, 0x8f,0x98, 0x89,0xa3, 0x90,0xd8, 0x89,0xae, 0x8c,0x8c, 0x96,0x82, 0x8e,0x64, 0x8e,0x8e, 0x89,0xe4, 0x94,0xad, 0x8e,0x86, 0x8b,0x77, 0x8e,0xb8, 0x8f,0x68, 0x92,0x73, 0x95,0xf1, 0x88,0xc8, 0x91,0xcc, 0x8f,0x45, 0x90,0xcc, 0x94,0x87, 0x92,0x85, 0x97,0x64, 0x8e,0x6e, 0x93,0x58, 
0x92,0xb5, 0x93,0x49, 0x94,0x70, 0x90,0xa7, 0x91,0xd2, 0x89,0xc6, 0x8c,0xbe, 0x97,0x7a, 0x8b,0xf3, 0x8e,0x96, 0x8a,0xdc, 0x97,0xc7, 0x95,0xdb, 0x8b,0x5a, 0x8d,0xcf, 0x8c,0x9a, 0x97,0x6c, 0x97,0x5e, 0x92,0x99, 0x90,0xa8, 0x94,0x77, 0x8f,0x8a, 0x89,0xca, 0x93,0x96, 0x8e,0xc0, 0x89,0x73, 0x88,0xc0, 0x98,0x46, 0x96,0x88, 0x8b,0xc7, 0x92,0x59, 0x8c,0xf5, 
0x91,0xe8, 0x94,0x92, 0x8e,0x71, 0x97,0x8e, 0x91,0x53, 0x92,0x63, 0x93,0x64, 0x89,0xba, 0x89,0x69, 0x8c,0x52, 0x90,0x65, 0x96,0xd9, 0x95,0x7e, 0x8f,0x49, 0x96,0xda, 0x88,0xab, 0x97,0x8a, 0x8c,0xc9, 0x96,0xf0, 0x93,0xa6, 0x96,0xa1, 0x8b,0x71, 0x96,0x54, 0x88,0xe1, 0x94,0x9a, 0x93,0xa1, 0x8c,0xb3, 0x8b,0x4c, 0x8e,0x76, 0x9c,0x6a, 0x96,0x5f, 0x8c,0x59, 
0x8d,0x73, 0x8f,0xe6, 0x96,0x4f, 0x90,0x5e, 0x90,0xc9, 0x8b,0x74, 0x93,0x60, 0x8e,0x67, 0x8f,0x9f, 0x94,0xc4, 0x89,0xd2, 0x8d,0x91, 0x90,0x5a, 0x91,0xe5, 0x97,0x8d, 0x92,0x87, 0x9c,0x70, 0x95,0xbd, 0x98,0x53, 0x94,0x6a, 0x98,0x4c, 0x8c,0x5a, 0x8b,0xe2, 0x8b,0xc3, 0x89,0xf2, 0x8c,0xc5, 0x8a,0x44, 0x90,0x8f, 0x8b,0xad, 0x95,0xdf, 0x8b,0x80, 0x8f,0xda, 
0x8e,0x9d, 0x93,0x71, 0x92,0xb2, 0x95,0xa1, 0x90,0x69, 0x97,0x44, 0x94,0x5c, 0x95,0xd4, 0x8f,0x82, 0x97,0x74, 0x92,0x9d, 0x89,0xf9, 0x9c,0x66, 0x94,0x52, 0x93,0xb1, 0x97,0x42, 0x92,0x65, 0x91,0x9b, 0x88,0xc5, 0x91,0x49, 0x92,0xc9, 0x91,0xe0, 0x8e,0x4f, 0x88,0xf6, 0x91,0x5f, 0x8b,0x4d, 0x8c,0xe3, 0x93,0x68, 0x8f,0x91, 0x8b,0x4e, 0x90,0xee, 0x88,0xd3, 
0x8d,0xf4, 0x8f,0xac, 0x93,0x69, 0x90,0x48, 0x96,0xba, 0x8c,0xae, 0x8c,0x4a, 0x8e,0x80, 0x89,0xf6, 0x8c,0xaf, 0x8d,0xce, 0x93,0xdb, 0x8c,0xc0, 0x91,0x96, 0x93,0xfa, 0x8d,0x9e, 0x8a,0x79, 0x89,0xb6, 0x8f,0x43, 0x8f,0x6b, 0x8b,0x40, 0x8f,0x50, 0x88,0xc3, 0x8f,0xb8, 0x91,0xb6, 0x92,0xde, 0x8e,0x6d, 0x8f,0x8f, 0x98,0x72, 0x8d,0xec, 0x91,0xbd, 0x93,0x56, 
0x8e,0x47, 0x98,0x4a, 0x89,0xf0, 0x95,0xf6, 0x91,0x4f, 0x95,0x76, 0x96,0xdf, 0x97,0xac, 0x8b,0xef, 0x93,0xb4, 0x8b,0x53, 0x8f,0xc4, 0x8a,0x4b, 0x8f,0xd8, 0x88,0xa2, 0x97,0x88, 0x93,0x7a, 0x8d,0xc0, 0x8a,0xd6, 0x8a,0x4a, 0x8e,0x97, 0x95,0xa5, 0x8a,0x98, 0x94,0x7c, 0x88,0xe6, 0x8c,0x8b, 0x95,0xc2, 0x8d,0x7e, 0x94,0xe6, 0x93,0x5d, 0x92,0x9a, 0x89,0xb4, 
0x8e,0xf3, 0x90,0xda, 0x90,0xbb, 0x93,0xa5, 0x92,0x69, 0x8f,0xce, 0x93,0xb9, 0x8c,0xf8, 0xe8,0x88, 0x8d,0x6c, 0x94,0x4f, 0x8d,0xbb, 0x97,0xf4, 0x8d,0x93, 0x8e,0x9f, 0x95,0x53, 0x89,0x9e, 0x90,0xf2, 0x96,0xb3, 0x95,0xa0, 0x88,0xf3, 0x8a,0xc3, 0x96,0x5c, 0x8f,0x6f, 0x8e,0xe3, 0x90,0xab, 0x8a,0xa5, 0x95,0x83, 0x95,0x61, 0x8e,0x78, 0x8c,0x9f, 0x92,0x67, 
0x91,0xbe, 0x8b,0x96, 0x8e,0xe8, 0x8a,0x6d, 0x97,0xc6, 0x94,0xdf, 0x96,0x43, 0x95,0x7c, 0x94,0x5f, 0x9c,0xdf, 0x90,0x56, 0x8a,0x42, 0x89,0xa1, 0x92,0x55, 0x90,0x6b, 0x94,0xaa, 0x95,0xc7, 0x8e,0x55, 0x96,0xa2, 0x89,0x9c, 0x8b,0xe6, 0x8b,0x76, 0x8e,0xa9, 0x92,0xe8, 0x97,0xc3, 0x8e,0xe6, 0x8a,0x84, 0x89,0xd4, 0x97,0xe9, 0x93,0xaf, 0x95,0x40, 0x8b,0xc8, 
0x89,0x8a, 0x95,0xf0, 0x8a,0x6f, 0x8f,0x9d, 0x91,0xe6, 0x97,0xa3, 0x8b,0x8b, 0x91,0xbc, 0x8a,0xa8, 0x96,0x9c, 0x90,0x54, 0x8d,0x4b, 0x8e,0x9e, 0x97,0xbf, 0x96,0xac, 0x94,0xe0, 0x9c,0x72, 0x98,0x5e, 0x8b,0xa6, 0x8c,0x95, 0x96,0xea, 0x88,0xc9, 0x91,0x80, 0x92,0xb7, 0x8b,0xf0, 0x91,0xca, 0x8d,0xf5, 0x89,0xdf, 0x8b,0x58, 0x95,0x9c, 0x95,0xd6, 0x8e,0xcb, 
0x8c,0x57, 0x8f,0x5a, 0x93,0xaa, 0x92,0x4a, 0x95,0xf3, 0x8d,0xa1, 0x96,0xe8, 0x8a,0xed, 0x8a,0xeb, 0x8d,0x96, 0x95,0x73, 0x89,0xbb, 0x91,0xb9, 0x8e,0x59, 0x89,0x9f, 0x90,0x47, 0x8d,0xa2, 0x90,0xb0, 0x8a,0xe8, 0x92,0xfa, 0x8d,0xc4, 0x8b,0xb0, 0x90,0xcd, 0x91,0x9e, 0x91,0x75, 0x8b,0x60, 0x8c,0x41, 0x88,0xf5, 0x90,0xac, 0x88,0xd7, 0x8a,0x5b, 0x95,0x74, 
0x89,0x5e, 0x91,0x8a, 0x91,0xf2, 0x91,0x52, 0x93,0x72, 0x92,0x75, 0x96,0xca, 0x95,0xa8, 0x92,0xca, 0x96,0xd2, 0x94,0xe4, 0x95,0xfb, 0x90,0xa2, 0x97,0xfb, 0x8e,0xe1, 0x8d,0x75, 0x8e,0x70, 0x92,0x6d, 0x8c,0xa2, 0xe3,0xa9, 0x92,0xb4, 0x98,0x62, 0x96,0xaf, 0x8c,0xea, 0x90,0x81, 0x8e,0xba, 0x97,0x70, 0x8e,0xe5, 0x92,0x42, 0x8f,0x5c, 0x95,0x97, 0x8b,0xc9, 
0x95,0x90, 0x89,0xa9, 0x88,0xd0, 0x8c,0xfb, 0x97,0xcd, 0x8d,0xcb, 0x8b,0xa5, 0x8c,0x6e, 0x8a,0x88, 0x8f,0x83, 0x89,0xaf, 0x92,0xa4, 0x8e,0xbf, 0x89,0x84, 0x90,0xb3, 0x95,0xe2, 0x8d,0xc5, 0x93,0x90, 0x90,0xd4, 0x92,0xad, 0x8f,0x71, 0x88,0xe7, 0x98,0x40, 0x8a,0xfa, 0x8f,0xad, 0x8f,0x70, 0x8e,0x52, 0x91,0x83, 0x96,0xbd, 0x91,0x7a, 0x89,0xce, 0x94,0xfc, 
0x94,0xc2, 0x93,0x6e, 0x8e,0xaf, 0x89,0xc1, 0x8a,0xb4, 0x8c,0x60, 0x92,0xb8, 0x8e,0xd2, 0x8c,0xec, 0x95,0xac, 0x94,0x8e, 0x91,0xc5, 0x8c,0xb4, 0x89,0xb0, 0x8c,0xa4, 0x8e,0x84, 0x90,0xb9, 0x92,0x72, 0x89,0xbd, 0x90,0xce, 0x97,0x45, 0x8a,0x57, 0x98,0x66, 0x8f,0x62, 0x90,0xba, 0x96,0xa7, 0x94,0xb1, 0x90,0x6c, 0x8f,0xed, 0x94,0xbb, 0x96,0xe9, 0x8c,0xb5, 
0x93,0x78, 0x90,0x53, 0x89,0xc2, 0x93,0xac, 0x94,0xf5, 0x89,0xe8, 0x8b,0xea, 0x8e,0x40, 0x8e,0x63, 0x8a,0xc7, 0x8c,0x4e, 0x90,0xaf, 0x96,0xa3, 0x93,0xf1, 0x8d,0x9c, 0x8f,0xbb, 0x92,0x78, 0x91,0x95, 0x8d,0xc8, 0x97,0xe7, 0x8e,0x45, 0x91,0xde, 0x97,0xad, 0x8f,0x9c, 0x93,0x53, 0x8d,0x8f, 0x92,0x62, 0x97,0x76, 0x8b,0x7e, 0x92,0x66, 0x8e,0x7e, 0x88,0xf9, 
0x97,0xa7, 0x8c,0x8e, 0x92,0x6a, 0x95,0xcf, 0x96,0x7b, 0x8b,0xf2, 0x96,0xc0, 0x8f,0x88, 0x96,0xb2, 0x93,0xef, 0x8b,0x7b, 0x8b,0x78, 0x94,0xbc, 0x8f,0x4c, 0x95,0x5c, 0x8f,0xf3, 0x91,0x72, 0x91,0x97, 0x97,0x91, 0x8c,0x76, 0x8f,0xc7, 0x97,0xb7, 0x8c,0xe5, 0x89,0x80, 0x88,0xa4, 0x94,0xf2, 0x88,0xcd, 0x95,0xaa, 0x93,0xe4, 0x92,0x6e, 0x95,0xfa, 0x91,0x6f, 
0x8e,0xed, 0x8b,0xb6, 0x93,0xe9, 0x95,0x4b, 0x8b,0x79, 0x96,0x6c, 0x94,0xbd, 0x89,0x93, 0x8e,0xce, 0x8b,0x89, 0x89,0x5c, 0x8c,0x99, 0x94,0x7a, 0x94,0x6e, 0x95,0x59, 0x8a,0x69, 0x8d,0x87, 0x93,0xbe, 0x94,0x6b, 0x8f,0x95, 0x9f,0x42, 0x93,0x6f, 0x91,0x68, 0xe3,0x4a, 0x8d,0xb8, 0x91,0xa1, 0x95,0xe9, 0x8e,0x95, 0x8a,0x4f, 0x8a,0xef, 0x91,0x81, 0x95,0x95, 
0x8e,0x53, 0x8a,0xf6, 0x8f,0xc0, 0x91,0xe4, 0x97,0x4c, 0x95,0x91, 0x94,0xcd, 0x8f,0x87, 0x96,0x68, 0x89,0x87, 0x92,0xa3, 0x96,0xbc, 0x97,0xe2, 0x8d,0xe7, 0x8e,0x77, 0x93,0xae, 0x96,0x6b, 0x92,0x8e, 0x92,0xa7, 0x95,0xb7, 0x95,0x9e, 0x97,0xb9, 0x97,0x98, 0x96,0xc5, 0x95,0xd3, 0x8b,0x86, 0x98,0x41, 0x8d,0x98, 0x8f,0xc1, 0x8c,0xa9, 0x94,0x43, 0x93,0xcb, 
0x91,0xab, 0x91,0xf0, 0x8c,0xad, 0x8c,0xb1, 0x8d,0xdd, 0x8f,0xe3, 0x89,0x42, 0x8a,0x51, 0x96,0x5d, 0x96,0xe2, 0x93,0x7c, 0x96,0xd8, 0x8c,0x7d, 0x91,0xa4, 0x89,0x68, 0x8a,0x7c, 0x92,0x54, 0x89,0x74, 0x8f,0x89, 0x90,0x85, 0x93,0x8a, 0x95,0x89, 0x97,0x59, 0x94,0x4e, 0x92,0xc7, 0x8e,0xb9, 0x8b,0x6c, 0x89,0x70, 0x90,0xf6, 0xe9,0x78, 0x8c,0xc3, 0x98,0x61, 
0x97,0xdf, 0x8b,0xe0, 0x8c,0xc4, 0x97,0xb3, 0x8c,0x88, 0x95,0xe6, 0x8b,0x8f, 0x95,0x82, 0x90,0xc3, 0x92,0x5b, 0x8d,0x95, 0x94,0x74, 0x93,0x7b, 0x90,0xe6, 0x90,0xe2, 0x8d,0xb0, 0x89,0xe5, 0x8d,0xcd, 0x93,0xf7, 0x8d,0x48, 0x95,0xe7, 0x92,0x44, 0x91,0xa7, 0x92,0x86, 0x96,0x40, 0x8b,0xfc, 0x8e,0xe2, 0x94,0xa0, 0x90,0xb8, 0x89,0xf3, 0x98,0x43, 0x8f,0xcc, 
0x91,0xce, 0x8f,0x5d, 0x8e,0x6f, 0x96,0xad, 0x8a,0xf4, 0x8b,0xc6, 0x94,0x73, 0x93,0x9a, 0x96,0x96, 0x94,0x84, 0x91,0xba, 0x95,0xb6, 0x97,0x9d, 0x8d,0x72, 0x96,0xbe, 0x8d,0xa9, 0x8a,0xe7, 0x8b,0x8e, 0x93,0x44, 0x91,0x4d, 0x8b,0xdf, 0x94,0xb2, 0x89,0xf5, 0x8b,0xd6, 0x8a,0xe6, 0x8c,0xb9, 0x90,0x51, 0x88,0xe3, 0x8c,0xb8, 0x8e,0xd7, 0x92,0xe2, 0x8d,0xc7, 
0x8d,0xe2, 0x91,0x63, 0x8b,0xb3, 0x90,0x4d, 0x89,0xb9, 0x90,0x46, 0x92,0xd7, 0x94,0x68, 0x8c,0xfc, 0x95,0xba, 0x97,0x7e, 0x92,0x45, 0x94,0xd4, 0x91,0x9c, 0x91,0xb1, 0x8d,0x4c, 0x91,0xa6, 0x88,0xda, 0x96,0xb0, 0x8c,0xf0, 0x8d,0xb7, 0x88,0xf8, 0x8e,0xf4, 0x94,0xe9, 0x97,0x54, 0x90,0x94, 0x93,0xc7, 0x8a,0xb7, 0x95,0x94, 0x8b,0x41, 0x8d,0x82, 0x8f,0x65, 
0x95,0x43, 0x9b,0x7a, 0x8c,0xf7, 0x8f,0xea, 0x93,0xe0, 0x8b,0x92, 0x96,0xec, 0x93,0x5f, 0x92,0x83, 0x91,0xe3, 0x95,0xea, 0x8a,0xd1, 0x90,0xf5, 0x8f,0x97, 0x90,0x66, 0x97,0x5c, 0x91,0x88, 0x97,0x48, 0x8b,0x43, 0x94,0x4d, 0x89,0xe6, 0x91,0x71, 0x92,0xf6, 0x8f,0x64, 0x93,0x4f, 0x91,0xd4, 0x97,0xa0, 0x92,0x8d, 0x95,0x77, 0x8b,0x51, 0x8a,0xc5, 0x95,0xe0, 
0x91,0xac, 0x93,0x47, 0x94,0xec, 0x96,0xf2, 0x89,0xa4, 0x88,0xc4, 0x91,0xa2, 0x96,0x58, 0x8c,0xcd, 0x8d,0xde, 0x8c,0xbb, 0x92,0x4e, 0x96,0x59, 0x8e,0xe7, 0x90,0xb6, 0x8e,0xa6, 0x90,0x5f, 0x92,0x50, 0x88,0xd9, 0x92,0xed, 0x8b,0x90, 0x8c,0x8a, 0x8b,0x7d, 0x97,0x5d, 0x8e,0x9a, 0x97,0x56, 0x9a,0xd0, 0x97,0x90, 0x8f,0xee, 0x8c,0x80, 0x94,0xde, 0x96,0x5a, 
0x8d,0x44, 0x98,0x48, 0x93,0x4b, 0x93,0xad, 0x90,0xd3, 0x8a,0xd4, 0x8b,0x9f, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 0x81,0x41, 

};
/*
"々、「」。ｒ２－７３（＋４．ｚ６Ｍ～？Ｘ１９ＤＰ！）　、、、、、",
"ジホネクビョエタワヌパユミゲサイオトロォガボゾナダヨゼィノケカェ",
"マニムペウァューポン・キザゴヘギバリシプチブグレヤヒハヴモゥピメ",
"ラドルソヶセヅコスズデャフツアヲテベッ、、、、、、、、、、、、、",
"よてかるえすあちうまでゆぐぉぢりぬおらさへどくろやんみずぽわこと",
"づねっごぅぼいぇふぺのぜほぱればゃめぃながぎきせぶつをげにひしだ",
"ぞぁざたけょじべはむもそび供ⅢⅠ：･♪■↑Ⅳ↓…←Ⅱ→、、、、、",
"秩説祈臓一回土免只耐特戴叩別攻完借島戦到基会健康沈入素身丈撃床集",
"悔増品浅魚友準序殴切屋血魔仕試我発紙仇失宿痴報以体拾昔這着妖始店",
"跳的廃制待家言陽空事含良保技済建様与貯勢背所果当実鋭安炉毎局炭光",
"題白子落全団電下永軍親黙敷終目悪頼庫役逃味客傍違爆藤元記思徊棒刑",
"行乗飽真惜逆伝使勝汎稼国浸大絡仲徘平牢破廊兄銀凝塊固灰随強捕朽詳",
"持賭調複進優能返盾葉喋懐彷燃導唯弾騒闇選痛隊三因狙貴後塗書起扇意",
"策小妬食娘鍵繰死怪険歳呑限走日込楽恩修縮機襲暗昇存釣士緒腕作多天",
"雑労解崩前夫戻流具洞鬼焼階証阿来奴座関開似払釜培域結閉降疲転丁俺",
"受接製踏段笑道効闊考念砂裂酷次百応泉無腹印甘暴出弱性冠父病支検暖",
"太許手確糧悲砲怖農憑新械横旦震八壁散未奥区久自定療取割花鈴同鼻曲",
"炎呆覚傷第離給他勘万慎幸時料脈扉徨録協剣爺伊操長愚駄索過宜復便射",
"係住頭谷宝今冶器危獄不化損産押触困晴願諦再恐析憎爽義窟員成為骸付",
"運相沢然途置面物通猛比方世練若講姿知犬罠超話民語吹室用主達十風極",
"武黄威口力才凶系活純憶彫質延正補最盗赤眺述育蓮期少術山巣命想火美",
"板渡識加感形頂者護噴博打原臆研私聖池何石勇蓋惑獣声密罰人常判夜厳",
"度心可闘備芽苦察残管君星魅二骨晶遅装妻礼殺退溜除鉄刻鍛要救断止飲",
"立月男変本喰迷処夢難宮休半臭表状喪送卵計症旅悟園愛飛囲分謎地放双",
"種狂馴必及僕反遠斜級噂嫌配馬漂格合得婆助檻登蘇絆査贈暮歯外奇早封",
"惨揮沼台有舞範順防援張名冷咲指動北虫挑聞服了利滅辺究連腰消見任突",
"足択遣験在上隠害望問倒木迎側栄掛探液初水投負雄年追嫉詰英潜騙古和",
"令金呼竜決墓居浮静端黒杯怒先絶魂牙栽肉工慕奪息中法屈寂箱精壊呂称",
"対従姉妙幾業敗答末売村文理荒明昆顔去泥閃近抜快禁頑源寝医減邪停塞",
"坂祖教信音色潰派向兵欲脱番像続広即移眠交差引呪秘裕数読換部帰高銃",
"匹孵功場内拠野点茶代母貫染女診予争幽気熱画倉程重徹態裏注婦飢看歩",
"速敵肥薬王案造帽枯材現誰忘守生示神単異弟巨穴急余字遊墟乱情劇彼忙",
"好路適働責間供、、、、、、、、、、、、、、、、、、、、、、、、、、"
*/
#endif



int gAtlas16Color1;
int gAtlas16Color2;

int gAtlas256Color;

int gTextureLoaded = 0;

int gTextureWidth1024 = TEXTURE_SIZE_1024;
int gTextureHeight512 = TEXTURE_SIZE_512;

int gTextureWidth512 = TEXTURE_SIZE_512;
int gTextureHeight256 = TEXTURE_SIZE_256;

void* gCurrentPalette;

char gConsoleInited = false;

void ErrorInitConsole()
{
	if(!gConsoleInited)
		consoleInit( NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 23, 2, false, true );
	gConsoleInited = 2;
}

static unsigned int nextPowerOf2(unsigned int n)  
{  
    unsigned power = 1; 
    for(;;)
	{
		if(n <= power) return power;
		power <<= 1;
	}
}  



/*---------------------------------------------------------------------------------

	Video API vaguely similar to OpenGL

  Copyright (C) 2005
			Michael Noland (joat)
			Jason Rogers (dovoto)
			Dave Murphy (WinterMute)

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any
  damages arising from the use of this software.

  Permission is granted to anyone to use this software for any
  purpose, including commercial applications, and to alter it and
  redistribute it freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you
     must not claim that you wrote the original software. If you use
     this software in a product, an acknowledgment in the product
     documentation would be appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and
     must not be misrepresented as being the original software.
  3. This notice may not be removed or altered from any source
     distribution.


---------------------------------------------------------------------------------*/

//The following functions are modified from libnds.
//https://github.com/devkitPro/libnds/blob/master/source/arm9/videoGL.c

uint8* vramBlock_examineSpecial( s_vramBlock *mb, uint8 *addr, uint32 size, uint8 align ) {
	// Simple validity tests
	if( !addr || !mb->firstEmpty || !size || align >= 8 )
		return NULL;
	// Start with the first empty block
	struct s_SingleBlock *block = mb->firstEmpty;

	// Set these value to 0/NULL (should only be filled in with valid data in case of error), and copy the address to start checking
	mb->lastExamined = NULL;
	mb->lastExaminedAddr = NULL;
	mb->lastExaminedSize = 0;
	uint8 *checkAddr = addr;

	// If the address is within a valid block, examine if it will initially fit in it
	while( block && checkAddr >= block->AddrSet + block->blockSize )
		block = block->node[ 3 ];
	if( !block ) return NULL;

	// Move the address up if before the first valid block
	if( checkAddr < block->AddrSet )
		checkAddr = block->AddrSet;

	uint8* bankLock[ 5 ] = { 0x0 };
	uint32 bankSize[ 5 ] = { 0x0 };
	uint32 curBank = 0;

	// Values that hold which banks to examine
	uint32 isNotMainBank = ( checkAddr >= (uint8*)VRAM_E ? 1 : 0 );
	uint32 vramCtrl = ( isNotMainBank ? VRAM_EFG_CR : VRAM_CR );
	int vramLock = glGlob->vramLock[ isNotMainBank ];
	uint32 i = 0, iEnd = ( isNotMainBank ? 3 : 4 );

	// Fill in the array with only those banks that are not set for Textures or Texture Palettes
	for( ; i < iEnd; i++ ) {
		if((( vramCtrl & 0x83 ) != 0x83 ) || ( vramLock & 0x1 )) {		// VRAM_ENABLE | ( VRAM_x_TEXTURE | VRAM_x_TEX_PALETTE )
			if( isNotMainBank ) {
				bankLock[ curBank ] = ( i == 0 ? (uint8*)VRAM_E : (uint8*)VRAM_F + (( i - 1 ) * 0x4000 ));
				bankSize[ curBank ] = ( i == 0 ? 0x10000 : 0x4000 );
			} else {
				bankLock[ curBank ] = (uint8*)VRAM_A + ( i * 0x20000 );
				bankSize[ curBank ] = 0x20000;
			}
			curBank++;
		}
		vramCtrl >>= 8;
		vramLock >>= 1;
	}
	curBank = 0;

	// Retrieve the available area from this block using the address given
	uint32 curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet );
	do {
		// Do address adjustments based on locked banks
		if( bankLock[ curBank ] ) {
			// Skip to corresponding bank that address is in
			while( bankLock[ curBank ] && checkAddr >= ( bankLock[ curBank ] + bankSize[ curBank ] ))
				curBank++;
			do {
				// Examine is address is within locked bank, and push it to next bank if needed
				if( bankLock[ curBank ] && checkAddr >= bankLock[ curBank ] && checkAddr < bankLock[ curBank ] + bankSize[ curBank ] )
					checkAddr = bankLock[ curBank ] + bankSize[ curBank ];
				else
					break;
			} while( bankLock[ ++curBank ] != NULL );
			// Continue block and address adjustments
			while( block && checkAddr >= block->AddrSet + block->blockSize )
				block = block->node[ 3 ];
			if( !block )	return NULL;
			if( checkAddr < block->AddrSet )
				checkAddr = block->AddrSet;
			// Adjust the blocks available size based on address location within said block
			if( bankLock[ curBank ] && bankLock[ curBank ] < block->AddrSet + block->blockSize )
				curBlockSize = (uint32)bankLock[ curBank ] - (uint32)checkAddr;
			else
				curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet );
		}

		// Obtained an aligned address, and adjust the available area that can be used
		uint8* aligned_checkAddr = (uint8*)(((uint32)checkAddr + ((1<<align)-1)) & (~((1<<align)-1)));
		uint32 excess = ((uint32)aligned_checkAddr - (uint32)checkAddr);
		curBlockSize -= excess;
		if( curBlockSize >= size ) {
			mb->lastExamined = block;
			mb->lastExaminedAddr = aligned_checkAddr;
			mb->lastExaminedSize = size;
			return aligned_checkAddr;
		} else {
			if( bankLock[ curBank ] && bankLock[ curBank ] < block->AddrSet + block->blockSize ) {
				checkAddr = bankLock[ curBank ] + bankSize[ curBank ];
				curBlockSize = 0;
			} else {
				block = block->node[ 3 ];
				if( !block )	return NULL;
				checkAddr = block->AddrSet;
				curBlockSize = block->blockSize;
			}
		}
	} while( block != NULL );
	return NULL;
}

uint8*
vramBlock__allocateBlock( s_vramBlock *mb, struct s_SingleBlock *block, uint8 *addr, uint32 size ) {
	// Initial tests to ensure allocation is valid
	if( !size || !addr || !block || block->indexOut || addr < block->AddrSet || ( addr + size ) > ( block->AddrSet + block->blockSize )) return 0;

	// Get pointers to the various blocks, as those may change from allocation
	struct s_SingleBlock **first = &mb->firstBlock, **alloc = &mb->firstAlloc, **empty = &mb->firstEmpty;
	// The nodes in the test block array will change as examinations of pre/post blocks are done
	struct s_SingleBlock *testBlock[ 4 ] = { block->node[ 0 ], block->node[ 1 ], block->node[ 2 ], block->node[ 3 ] };

	// Boolean comparisons ( for determining if an empty block set for allocation should be split once, twice, or not at all )
	uint32 valComp[ 2 ] = { addr != block->AddrSet, addr + size < block->AddrSet + block->blockSize };

	int i = 0;
	for( ; i < 2; i++ ) {
		// Generate a new block if condition requires it, based on earlier comparison
		if( valComp[ i ] ) {
			// If comparison check is true, then empty block is split in two empty blocks. Addresses set, block sizes corrected, and
			//			nodes linked between the two. This can be done up to two times, resulting in 3 empty blocks sequentially.
			//			The middle in this case will be the true allocated block. If split once in total, either block will be
			//			the true block. Also done is examination of the first block and first empty block, which will be set as well
			struct s_SingleBlock *newBlock = ( struct s_SingleBlock* )malloc( sizeof( struct s_SingleBlock ));
			newBlock->indexOut = 0;
			newBlock->AddrSet = block->AddrSet + ( i * size );

			if( i ) {
				newBlock->blockSize = block->blockSize - size;
				block->blockSize = size;
			} else {
				newBlock->blockSize = (uint32)addr - (uint32)block->AddrSet;
				block->AddrSet = addr;
				block->blockSize -= newBlock->blockSize;
				if( block == *first )
					*first = newBlock;
			}

			// To get an idea of what the nodes represent, the first two in the array refer to the immediate pre/post blocks, which
			//		can be either empty or allocated blocks. The last two in the array refer to the prior/next blocks of the same type
			//		(empty or allocated), which can skip over blocks of a differing type. This allows for quick examination of similar
			//		blocks while keeping the organization of all the blocks in an ordered fashion. Think of it like a doubly-doubly
			//		linked list
			newBlock->node[ 1 - i ] = block;
			newBlock->node[ i ] = testBlock[ i ];
			newBlock->node[ i + 2 ] = testBlock[ i + 2 ];

			block->node[ i ] = newBlock;
			if( testBlock[ i ] )
				testBlock[ i ]->node[ 1 - i ] = newBlock;
			if( testBlock[ i + 2 ] )
				testBlock[ i + 2 ]->node[ 3 - i ] = newBlock;

			testBlock[ i + 2 ] = newBlock;

			if( block == *empty )
				*empty = newBlock;
		}
	}

	// Finish off node linking (in this case, NULL is possible, which refers to the end of the block sequences)
	if( testBlock[ 2 ] )
		testBlock[ 2 ]->node[ 3 ] = testBlock[ 3 ];
	if( testBlock[ 3 ] )
		testBlock[ 3 ]->node[ 2 ] = testBlock[ 2 ];

	// The condition of examining the first empty block is needed, in case the comparison check early is false for both
	if( block == *empty )
		*empty = block->node[ 3 ];

	block->node[ 2 ] = testBlock[ 0 ];
	block->node[ 3 ] = testBlock[ 1 ];
	if( testBlock[ 0 ] )
		testBlock[ 0 ]->node[ 3 ] = block;
	else
		*alloc = block;
	if( testBlock[ 1 ] )
		testBlock[ 1 ]->node[ 2 ] = block;

	return (uint8*)block;
}

uint32
vramBlock_allocateSpecial( s_vramBlock *mb, uint8 *addr, uint32 size ) {
	// Simple validity tests. Special allocations require "examination" data
	if( !addr || !size || !mb->lastExamined || !mb->lastExaminedAddr ) return 0;
	if( mb->lastExaminedAddr != addr || mb->lastExaminedSize != size ) return 0;

	// Can only get here if prior tests passed, meaning a spot is available, and can be allocated
	struct s_SingleBlock *newBlock = (struct s_SingleBlock*)vramBlock__allocateBlock( mb,  mb->lastExamined, addr, size );
	if( newBlock ) { // with current implementation, it should never be false if it gets to here
		uint32 curBlock;
		// Use a prior index if one exists. Else, obtain a new index
		if( mb->deallocCount )
			curBlock = (uint32)DynamicArrayGet( &mb->deallocBlocks, mb->deallocCount-- );
		else
			curBlock = mb->blockCount++;
		DynamicArraySet( &mb->blockPtrs, curBlock, (void*)newBlock );
		// Clear out examination data
		mb->lastExamined = NULL;
		mb->lastExaminedAddr = NULL;
		mb->lastExaminedSize = 0;
		newBlock->indexOut = curBlock;
		return curBlock;
	}
	return 0;
}

uint16* vramGetBank(uint16 *addr) {
//---------------------------------------------------------------------------------
	if(addr >= VRAM_A && addr < VRAM_B)
		return VRAM_A;
	else if(addr >= VRAM_B && addr < VRAM_C)
		return VRAM_B;
	else if(addr >= VRAM_C && addr < VRAM_D)
		return VRAM_C;
	else if(addr >= VRAM_D && addr < VRAM_E)
		return VRAM_D;
	else if(addr >= VRAM_E && addr < VRAM_F)
		return VRAM_E;
	else if(addr >= VRAM_F && addr < VRAM_G)
		return VRAM_F;
	else if(addr >= VRAM_G && addr < VRAM_H)
		return VRAM_G;
	else if(addr >= VRAM_H && addr < VRAM_I)
		return VRAM_H;
	else return VRAM_I;
}

int AssignColorPalette(SURFACE* surf, uint16 width, const uint16* table)
{
	uint32 colFormatVal = 4;
	uint8* checkAddr = vramBlock_examineSpecial( glGlob->vramBlocks[ 1 ], (uint8*)VRAM_E, width << 1, colFormatVal );

	if(surf->paletteAddress) //use last address
	{
		uint32 tempVRAM = VRAM_EFG_CR;
		uint16 *startBank = vramGetBank( (uint16*)surf->paletteAddress );
		uint16 *endBank = vramGetBank( (uint16*)((char*)surf->paletteAddress + ( width << 1 ) - 1));
		do {
			if( startBank == VRAM_E ) {
				vramSetBankE( VRAM_E_LCD );
				startBank += 0x8000;
			} else if( startBank == VRAM_F ) {
				vramSetBankF( VRAM_F_LCD );
				startBank += 0x2000;
			} else if( startBank == VRAM_G ) {
				vramSetBankG( VRAM_G_LCD );
				startBank += 0x2000;
			}
		} while ( startBank <= endBank );

		swiCopy( table, (void*)surf->paletteAddress, width | COPY_MODE_HWORD );
		vramRestoreBanks_EFG( tempVRAM );
		return surf->paletteOffset;
	}

	if( checkAddr ) {
		// Calculate the address, logical and actual, of where the palette will go
		uint16* baseBank = vramGetBank( (uint16*)checkAddr );
		uint32 addr = ( (uint32)checkAddr - (uint32)baseBank );
		uint8 offset = 0;

		if( baseBank == VRAM_F )
			offset = ( VRAM_F_CR >> 3 ) & 3;
		else if( baseBank == VRAM_G )
			offset = ( VRAM_G_CR >> 3 ) & 3;
		addr += (( offset & 0x1 ) * 0x4000 ) + (( offset & 0x2 ) * 0x8000 );

		addr >>= colFormatVal;
		if( colFormatVal == 3 && addr >= 0x2000 ) {
			// palette location not good because 4 color mode cannot extend past 64K texture palette space
			GFX_PAL_FORMAT = glGlob->activePalette = 0;
			return -1;
		}
		gl_palette_data *palette;
		palette = (gl_palette_data*)malloc( sizeof( gl_palette_data ));
		palette->palIndex = vramBlock_allocateSpecial( glGlob->vramBlocks[ 1 ], checkAddr, width << 1 );
		palette->vramAddr = checkAddr;
		palette->addr = addr;

		palette->connectCount = 1;
		palette->palSize = width << 1;

		// copy straight to VRAM, and assign a palette name
		uint32 tempVRAM = VRAM_EFG_CR;
		uint16 *startBank = vramGetBank( (uint16*)palette->vramAddr );
		uint16 *endBank = vramGetBank( (uint16*)((char*)palette->vramAddr + ( width << 1 ) - 1));
		do {
			if( startBank == VRAM_E ) {
				vramSetBankE( VRAM_E_LCD );
				startBank += 0x8000;
			} else if( startBank == VRAM_F ) {
				vramSetBankF( VRAM_F_LCD );
				startBank += 0x2000;
			} else if( startBank == VRAM_G ) {
				vramSetBankG( VRAM_G_LCD );
				startBank += 0x2000;
			}
		} while ( startBank <= endBank );

		swiCopy( table, palette->vramAddr, width | COPY_MODE_HWORD );
		vramRestoreBanks_EFG( tempVRAM );
		surf->paletteAddress = (int)palette->vramAddr;
		return addr;
	}
	else
	{
		GFX_PAL_FORMAT = glGlob->activePalette;
		return glGlob->activePalette;
	}
	
}

// Copy data from surf_no to texture
BOOL CopyDataToTexture(int paletteType, int textureid, int surf_no,  int xoffset, int yoffset, RECT* rect)
{
	BUFFER_PIXEL* tex;
	int texH;
	int texW;
	
	if(!(tex = (BUFFER_PIXEL*)glGetTexturePointer(textureid)))
		return FALSE;
	if(textureid == gAtlas16Color1)	
	{	
		texW = gTextureWidth1024;	
		texH = gTextureHeight512;	
	}	
	else 
	{	
		texW = gTextureWidth1024;	
		texH = gTextureHeight256;	
	}	
	
	surf[surf_no].textureid = textureid;	

	uint32 vramTemp = VRAM_CR;

	uint32 size = 0;
	size = 1 << (texW + texH + 6);

	uint16 *startBank = vramGetBank( (uint16*)tex );
	uint16 *endBank = vramGetBank(( uint16*)( (char*)tex + size - 1 ));

	do {
		if( startBank == VRAM_A )
			vramSetBankA( VRAM_A_LCD );
		else if( startBank == VRAM_B )
			vramSetBankB( VRAM_B_LCD );
		else if( startBank == VRAM_C )
			vramSetBankC( VRAM_C_LCD );
		else if( startBank == VRAM_D )
			vramSetBankD( VRAM_D_LCD );
		startBank += 0x10000;
	} while ( startBank <= endBank );

	//dmaCopyWords(0, surf[surf_no].data, tex, size);

	int atlasW = 1 << (texW + 3);
	int atlasH = 1 << (texH + 3);

	int texDivi = 2;

	int surfaceW = surf[surf_no].w;
	int rectW = rect->right - rect->left;
	if(rectW <= 3) rectW = 4;
	
	int rectH = rect->bottom - rect->top;

	switch(paletteType)
	{
		case GL_RGB4:
			texDivi = 4;
		break;
		case GL_RGB16:
			texDivi = 2;
		break;
		case GL_RGB256:
			texDivi = 1;
		break;
	}
	int surfStartH = rect->top; 
	// maybe this will help on hardware?
	DC_FlushRange(surf[surf_no].data, surf[surf_no].w*surf[surf_no].h);

	for(int h = 0; h < rectH; h++)
	{
	memcpy(tex + (atlasW*h/2) + (atlasW*yoffset/2) + (xoffset/2), //position to copy to	
			surf[surf_no].data+((rect->left/texDivi) + (surfaceW*surfStartH/texDivi)), //line from the surf				
							(rectW/texDivi)); //how many pixels to copy
		surfStartH++;
	}

	vramRestorePrimaryBanks(vramTemp);

	return TRUE;
}


void initSubSprites(void){
//-------------------------------------------------------
 
	oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
 
	int x = 0;
	int y = 0;
 
	int id = 0;

	//set up a 4x3 grid of 64x64 sprites to cover the screen
	for(y = 0; y < 3; y++)
	for(x = 0; x < 4; x++)
	{
	/*	u16 *offset = &SPRITE_GFX_SUB[(x * 64) + (y * 64 * 256)];
 
		oamSet(&oamSub, x + y * 4, x * 64, y * 64, 0, 15, SpriteSize_64x64, 
			SpriteColorFormat_Bmp, offset, -1, false,false,false,false,false);
	*/
		oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
		oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
		oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
		id++;
	}
 
	swiWaitForVBlank();
 
	oamUpdate(&oamSub);
}


// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//Draw to screen
BOOL Flip_SystemTask()
{
	//Update inputs
	UpdateInput();
	



	if(gDebug.bFastForward)
	{

		if(gDebug.FastForwardTimer++ % 5 == 0)
		{
			glEnd2D();
			glFlush(0);

			swiWaitForVBlank();
		}
		else if(gDebug.FastForwardTimer % 5 == 4)
		{
			glBegin2D();
		}
	}
	else
	{
		glEnd2D();
		glFlush(0);

		swiWaitForVBlank();
		if(gb50Fps && gVBlankCounter % 5 == 0) swiWaitForVBlank();
		
		glBegin2D();
	}
	
		CopyFaceTexture();
		CopyDirtyText();


	#ifdef TWO_SCREENS

		if((gCounter & 1) == 0)
		{
			lcdMainOnTop();
			vramSetBankD(VRAM_D_LCD);
			vramSetBankC(VRAM_C_SUB_BG);
			REG_DISPCAPCNT = DCAP_BANK(3) | DCAP_ENABLE | DCAP_SIZE(3);
		}
		else
		{
			lcdMainOnBottom();
			vramSetBankC(VRAM_C_LCD);
			vramSetBankD(VRAM_D_SUB_SPRITE);
			REG_DISPCAPCNT = DCAP_BANK(2) | DCAP_ENABLE | DCAP_SIZE(3);
		}
#endif

	
	return TRUE;
}

BOOL StartDirectDraw()
{
	
	//-----------------------------------------------------------------
	// Initialize the graphics engines
	//-----------------------------------------------------------------
	videoSetMode( MODE_5_3D );
#ifdef TWO_SCREENS
	videoSetModeSub(MODE_5_2D);
 
	// sub sprites hold the bottom image when 3D directed to top
	initSubSprites();
 
	// sub background holds the top image when 3D directed to bottom
	bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
#endif

    vramSetBankA( VRAM_A_TEXTURE );     
	vramSetBankB( VRAM_B_TEXTURE );
	vramSetBankC( VRAM_C_TEXTURE );
	vramSetBankD( VRAM_D_TEXTURE );
	
	vramSetBankE(VRAM_E_TEX_PALETTE);  // Allocate VRAM bank for all the palettes
	glScreen2D();
	glEnable(GL_TEXTURE_2D);

	glBegin2D();

	glGenTextures(1, &gAtlas16Color1);
	glBindTexture(0, gAtlas16Color1);
	glTexImage2D(0,0, GL_RGB16, gTextureWidth1024, gTextureHeight512, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
		NULL);

	glGenTextures(1, &gAtlas16Color2);
	glBindTexture(0, gAtlas16Color2);
	glTexImage2D(0,0, GL_RGB16, gTextureWidth1024, gTextureHeight256, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
		NULL);

	glGenTextures(1, &gAtlas256Color);
	glBindTexture(0, gAtlas256Color);
	glTexImage2D(0,0, GL_RGB16, gTextureWidth1024, gTextureHeight256, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
		NULL);

#ifndef TWO_SCREENS
	videoSetModeSub( MODE_0_2D  );
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 );
	scanKeys();
	if(keysHeld() & KEY_SELECT)
	{
		consoleInit( NULL, 0, BgType_Text4bpp, BgSize_T_256x256, 23, 2, false, true );
		gConsoleInited = TRUE;
	}

#endif
	
	
	return TRUE;
}

void EndDirectDraw()
{
	//TODO
}

void ReleaseSurface(SurfaceID s)
{
	free(surf[s].data);
}

BOOL MakeSurface_Generic(int bxsize, int bysize, SurfaceID surf_no)
{
	if (surf_no < SURFACE_ID_MAX)
	{

		surf[surf_no].w = bxsize;
		surf[surf_no].h = bysize;
		//surf[surf_no].data = (BUFFER_PIXEL*)malloc(bxsize * bysize * sizeof(BUFFER_PIXEL));
		//surf[surf_no].palette = (u16*)malloc(256*(sizeof(u16)));
		surf[surf_no].textureid = NULL;
		surf[surf_no].paletteAddress = NULL;
	}

	return TRUE;
}


bool npcSymInArmsSlot = false;

BOOL LoadBitmap(FILE_e *fp, SurfaceID surf_no, bool create_surface)
{
	if (surf_no == SURFACE_ID_LEVEL_TILESET || surf_no == SURFACE_ID_TEXT_BOX || surf_no == SURFACE_ID_MY_CHAR \
		|| surf_no == SURFACE_ID_LEVEL_SPRITESET_1 || surf_no == SURFACE_ID_CARET || surf_no == SURFACE_ID_BULLET\
		|| surf_no == SURFACE_ID_NPC_SYM || surf_no == SURFACE_ID_LEVEL_BACKGROUND || surf_no == SURFACE_ID_ITEM_IMAGE\
	 || surf_no == SURFACE_ID_ARMS_IMAGE || surf_no == SURFACE_ID_FADE\
		|| surf_no == SURFACE_ID_STAGE_ITEM || surf_no == SURFACE_ID_LEVEL_SPRITESET_2\
		|| surf_no == SURFACE_ID_ARMS || surf_no == SURFACE_ID_FONT || surf_no == SURFACE_ID_LOADING\
		|| surf_no == SURFACE_ID_PIXEL || surf_no == SURFACE_ID_TITLE || surf_no == SURFACE_ID_NPC_REGU\
		|| surf_no == SURFACE_ID_CASTS || surf_no == SURFACE_ID_FACE || surf_no == SURFACE_ID_CREDITS_IMAGE)
	{

	}
	else
	{
		fclose_embed(fp);
		return TRUE;
	}

	struct stat file_descriptor;
	long file_size;
#ifndef READ_FROM_SD
	file_size = fp->size;
#else
	fstat(fileno(fp), &file_descriptor);
	file_size = file_descriptor.st_size;
#endif

	unsigned char *file_buffer = (unsigned char*)malloc(file_size);
	fread_embed(file_buffer, file_size, 1, fp);

	unsigned int bitmap_width, bitmap_height;
	unsigned char *bitmap_pixels;

	LodePNGState state;
	lodepng_state_init(&state);

	if(surf_no == SURFACE_ID_NPC_REGU || surf_no == SURFACE_ID_FACE || !strcmp("Npc/NpcGuest", surf[surf_no].name))
	{
		state.info_raw.palettesize = 256;
		state.info_raw.bitdepth = 4;
		state.info_raw.colortype = LCT_PALETTE;
	}
	else
	{
		state.info_raw.palettesize = 16;
		state.info_raw.bitdepth = 4;
		state.info_raw.colortype = LCT_PALETTE;
	}
	
	state.decoder.color_convert = true;
	
	
	lodepng_decode(&bitmap_pixels, &bitmap_width, &bitmap_height, &state, file_buffer, file_size);

	printf("Size %d\n", state.info_png.color.palettesize);

	int palettesize = state.info_png.color.palettesize;	

	surf[surf_no].w = bitmap_width; surf[surf_no].h = bitmap_height;
	
	if (create_surface)
	{
		MakeSurface_Generic(bitmap_width, bitmap_height, surf_no);
	}

	surf[surf_no].palette = (u16*)malloc(palettesize*2);
	for (int i = 0; i < palettesize; i++)
	{
		uint8_t r,g,b;
		r = state.info_png.color.palette[i*4] / 8;
		g = state.info_png.color.palette[i*4+1] / 8;
		b = state.info_png.color.palette[i*4+2] / 8;
		surf[surf_no].palette[i] = RGB15(r,g,b);
	}

	lodepng_state_cleanup(&state);
	free(file_buffer);

	GL_TEXTURE_TYPE_ENUM paletteType;
	switch (palettesize)
	{
		case 2:
		case 4:
			paletteType = GL_RGB4;
			break;
		case 16:
			paletteType = GL_RGB16;
			break;
		case 256:
			paletteType = GL_RGB256;
			break;
		default:
			paletteType = GL_RGB16;
			break;
	}

	surf[surf_no].data = (BUFFER_PIXEL*)malloc(bitmap_width * bitmap_height * sizeof(BUFFER_PIXEL));

	// Image must have transparency..
	for (int y = 0; y < bitmap_height; y++)
	{
		for (int x = 0; x < bitmap_width; x++)
		{
			int pos;
			char color;
			pos = (y*bitmap_width) + x;

			switch(paletteType)
			{
				case GL_RGB4:
					color = (bitmap_pixels[pos*4] | bitmap_pixels[pos*4+1] << 2 | bitmap_pixels[pos*4+2] << 4 | bitmap_pixels[pos*4+3] << 6);
				break;
				case GL_RGB16:
					color = (bitmap_pixels[pos*2] | bitmap_pixels[pos*2+1] << 4);
				break;
				case GL_RGB256:
					color = (bitmap_pixels[pos]);
				break;
				case GL_RGB:
					uint8_t r,g,b;
						pos = (y*bitmap_width) + x;
						r = bitmap_pixels[pos*3] / 8;
						g = bitmap_pixels[pos*3+1] / 8;
						b = bitmap_pixels[pos*3+2] / 8;
					color = (bitmap_pixels[pos]);
				break;

			}

			surf[surf_no].data[ pos ].color = color;
		}
	}

	int yoffset = 0;
	int xoffset = 0;
	int paletteOffset = 0;
	int textureid = gAtlas16Color1;

	RECT datarect = {0, 0, bitmap_width, bitmap_height};

	VramSlot table[] =
	{
		{SURFACE_ID_LEVEL_SPRITESET_1, gAtlas16Color1, 0, 0},
		{SURFACE_ID_LEVEL_SPRITESET_2, gAtlas256Color, 640, 0},
		{SURFACE_ID_NPC_SYM, gAtlas16Color1, 704, 256},
		{SURFACE_ID_CASTS, gAtlas16Color1, 256, 256},
		{SURFACE_ID_CREDITS_IMAGE, gAtlas16Color2, 480, 0},
		{SURFACE_ID_LEVEL_TILESET, gAtlas16Color1, 0, 240},
		{SURFACE_ID_TITLE, gAtlas16Color1, 0, 0},
		{SURFACE_ID_LOADING, gAtlas16Color1, 0, 0},
		{SURFACE_ID_PIXEL, gAtlas16Color1, 0, 48},
		{SURFACE_ID_BULLET, gAtlas16Color1, 256, 240},
		{SURFACE_ID_ARMS, gAtlas16Color2, 0, 0},
		{SURFACE_ID_ITEM_IMAGE, gAtlas16Color2, 64, 128},
		{SURFACE_ID_ARMS_IMAGE, gAtlas16Color1, 512, 496},
		{SURFACE_ID_STAGE_ITEM, gAtlas16Color1, 256, 496},
		{SURFACE_ID_FADE, gAtlas16Color1, 0, 480},
		{SURFACE_ID_NPC_REGU, gAtlas256Color, 0, 0},
		{SURFACE_ID_FACE, gAtlas16Color2, 888, 168},
		{SURFACE_ID_LEVEL_BACKGROUND, gAtlas16Color1, 640, 0},
		{SURFACE_ID_CARET, gAtlas16Color2, 320,0},
		{SURFACE_ID_TEXT_BOX, gAtlas16Color2, 640, 64},
		{SURFACE_ID_FONT, gAtlas16Color2, 640, 216},
		{SURFACE_ID_MY_CHAR, gAtlas16Color2, 640, 0},
	};
	bool found = false;
	for (size_t i = 0; i < sizeof(table)/sizeof(table[0]); i++)
	{
		if(surf_no == table[i].id)
		{
			xoffset = table[i].x;
			yoffset = table[i].y;
			textureid = table[i].atlas;
			found = true;
			break;
		}
	}
	if(!found)
	{
		fclose_embed(fp);
		free(bitmap_pixels);
		free(surf[surf_no].data);
		return TRUE;
	}

	if(surf_no == SURFACE_ID_NPC_SYM && npcSymInArmsSlot)
	{
		textureid = gAtlas16Color2;
		xoffset = 0;
		yoffset = 0;
	}
	if(surf_no == SURFACE_ID_FACE)
		goto facejump;

#ifdef JAPANESE
	if(surf_no == SURFACE_ID_FONT)
		{datarect.right = 384; datarect.bottom = 39;}
#endif

	if(!CopyDataToTexture(paletteType, textureid, surf_no, xoffset, yoffset, &datarect))
	{
		fclose_embed(fp);
		free(bitmap_pixels);
		free(surf[surf_no].data);
		return TRUE;
	}

facejump:
	surf[surf_no].paletteType = paletteType;
	surf[surf_no].palettesize = palettesize;
	surf[surf_no].xoffset = xoffset;
	surf[surf_no].yoffset = yoffset;

	surf[surf_no].paletteOffset = AssignColorPalette(&surf[surf_no], 256, surf[surf_no].palette);


	free(bitmap_pixels);
	if(surf_no != SURFACE_ID_TEXT_BOX && surf_no != SURFACE_ID_FACE
#ifdef JAPANESE
	&& surf_no != SURFACE_ID_FONT
#endif
	)
		free(surf[surf_no].data);
	fclose_embed(fp);
	
	return TRUE;
}

BOOL LoadBitmap_File(const char *name, SurfaceID surf_no, bool create_surface)
{
	if(surf_no == SURFACE_ID_PIXEL || surf_no == SURFACE_ID_TITLE || surf_no == SURFACE_ID_CARET
		|| surf_no == SURFACE_ID_BULLET || surf_no == SURFACE_ID_ITEM_IMAGE || surf_no == SURFACE_ID_NPC_SYM)
	{

	}
	else if(!strcmp(name, surf[surf_no].name)) return TRUE;
	printf("LoadBitmap_File %s\n", name);
	printf("Memory: %d %d %d\n", mallinfo().arena, mallinfo().uordblks, mallinfo().fordblks);
	//Attempt to load PNG
	
	char path[MAX_PATH];
	sprintf(path, "%s/%s.png", gDataPath, name);
	
	FILE_e *fp = fopen_embed(path, "rb");
	if (fp)
	{
		printf("Loading surface (as .png) from %s for surface id %d\n", path, surf_no);
		if (LoadBitmap(fp, surf_no, create_surface))
		{
			strcpy(surf[surf_no].name, name);
			return TRUE;
		}
			
	}
	ErrorInitConsole();
	printf("Failed to open file %s\n", name);
	return FALSE;
}

BOOL LoadBitmap_Resource(const char *res, SurfaceID surf_no, bool create_surface)
{
	//TODO
	return TRUE;
}

BOOL MakeSurface_File(const char *name, SurfaceID surf_no)
{
	return LoadBitmap_File(name, surf_no, true);
}

BOOL MakeSurface_Resource(const char *res, SurfaceID surf_no)
{
	return LoadBitmap_Resource(res, surf_no, true);
}

BOOL ReloadBitmap_File(const char *name, SurfaceID surf_no)
{
	return LoadBitmap_File(name, surf_no, false);
}

BOOL ReloadBitmap_Resource(const char *res, SurfaceID surf_no)
{
	return LoadBitmap_Resource(res, surf_no, false);
}

void BackupSurface(SurfaceID surf_no, RECT *rect)
{
	return;
	//TODO:
	for (int fx = rect->left; fx < rect->right; fx++)
	{
		for (int fy = rect->top; fy < rect->bottom; fy++)
		{
			int dx = fx - rect->left;
			int dy = fy - rect->top;
			
			if (dx < 0 || dy < 0)
				continue;
			if (dx >= surf[surf_no].w || dy >= surf[surf_no].h)
				continue;
			
			//BUFFER_PIXEL *fromPixel = &screenBuffer[fy * WINDOW_WIDTH + fx];
			//SET_BUFFER_PIXEL(surf[surf_no].data, surf[surf_no].w, dx, dy, fromPixel->r, fromPixel->g, fromPixel->b);
		}
	}
}

int gFaceNo = -1;
int gFaceNoCurrent = -1;

bool TextLinesDirty[6] = {0};

void CopyFaceTexture()
{
	if(gInMinimap) return;
	if(gFaceNoCurrent == gFaceNo) return;
	gFaceNoCurrent = gFaceNo;
	int face = gFaceNoCurrent;
	RECT rcFace;
	rcFace.left = (face % 6) * 48;
	rcFace.top = (face / 6) * 48;
	rcFace.right = rcFace.left + 48;
	rcFace.bottom = rcFace.top + 48;
	CopyDataToTexture(surf[SURFACE_ID_FACE].paletteType, gAtlas16Color2, SURFACE_ID_FACE, 
		surf[SURFACE_ID_FACE].xoffset, surf[SURFACE_ID_FACE].yoffset, &rcFace);
}

void CopyDirtyText()
{
	if(gInMinimap) return;
	for(int i=0;i<6;i++)
	{
		if(!TextLinesDirty[i]) continue;
		TextLinesDirty[i] = false;
		if(i == 4)
		{
			RECT rect = {0, 0, surf[SURFACE_ID_ROOM_NAME].w, surf[SURFACE_ID_ROOM_NAME].h};
			CopyDataToTexture(surf[SURFACE_ID_ROOM_NAME].paletteType, surf[SURFACE_ID_ROOM_NAME].textureid, SURFACE_ID_ROOM_NAME, 
			surf[SURFACE_ID_ROOM_NAME].xoffset, surf[SURFACE_ID_ROOM_NAME].yoffset, &rect);

		}
		else if (i == 5)
		{
			RECT rect = {0, 0, surf[SURFACE_ID_CREDIT_CAST].w, surf[SURFACE_ID_CREDIT_CAST].h};
			CopyDataToTexture(surf[SURFACE_ID_CREDIT_CAST].paletteType, surf[SURFACE_ID_CREDIT_CAST].textureid, SURFACE_ID_CREDIT_CAST, 
			surf[SURFACE_ID_CREDIT_CAST].xoffset, surf[SURFACE_ID_CREDIT_CAST].yoffset, &rect);
		}
		else
		{
			RECT rect = {0, 0, surf[SURFACE_ID_TEXT_LINE1].w, surf[SURFACE_ID_TEXT_LINE1].h};
			CopyDataToTexture(surf[30 + i].paletteType, surf[30 + i].textureid, 30 + i, 
			surf[30 + i].xoffset, surf[30 + i].yoffset, &rect);
		}
		
	}

}


void DrawBitmapSizeParam(RECT *rcView, int x, int y, int width, int height, RECT *rect, SurfaceID surf_no, bool transparent)
{
	//TODO: draw queueing
	//TODO: don't render if transparent
#ifdef TWO_SCREENS
	int temp = x;
	x = WINDOW_HEIGHT - y;
	y = temp;

	if((gCounter & 1) == 0) // bottom screen
	{
		
	}
	else
	{
		y -= WINDOW_WIDTH / 2;
	}

#endif

	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	if(surf_no == SURFACE_ID_SCREEN_GRAB) return;


	int textureid;
	if(!surf[surf_no].textureid) {textureid = gAtlas16Color1;}
	else
	{
		textureid = surf[surf_no].textureid;
	}

////
	RECT srcRect = *rect;

	if(surf_no == SURFACE_ID_FACE)
	{
		srcRect.right -= srcRect.left;
		srcRect.bottom -= srcRect.top;
		srcRect.left = 0;
		srcRect.top = 0;
	}
	srcRect.top += surf[surf_no].yoffset;
	srcRect.bottom += surf[surf_no].yoffset;

	if(surf[surf_no].paletteType == GL_RGB16)
	{
		srcRect.left += surf[surf_no].xoffset;
		srcRect.right += surf[surf_no].xoffset;
	}
	else
	{
		// To compensate for the texture size being halved when turning from 16 to 256 color
		srcRect.left += surf[surf_no].xoffset / 2;
		srcRect.right += surf[surf_no].xoffset / 2;
	}
/*
	if(rcView->left > x)
	{
		srcRect.left += rcView->left - x;
		x = rcView->left;
	}

	if(rcView->top > y)
	{
		srcRect.top += rcView->top - y;
		y = rcView->top;
	}

	if(x + width > rcView->right)
	{
		srcRect.right -= x + width - rcView->right;
	}

	if(y + height > rcView->bottom)
	{
		srcRect.bottom -= y + height - rcView->bottom;
	}*/
////

	//glSprite(x, y, rect, gAtlas16Color1, 0);
	glSpriteSizeParam(x, y, width, height, &srcRect, surf[surf_no].textureid, surf[surf_no].paletteOffset, surf[surf_no].paletteType);
}
/*
__attribute__((hot))
void PutBitmap3(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no) //Transparency
{
	DrawBitmap(rcView, x, y, rect, surf_no, true);
}

void PutBitmap4(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no) //No Transparency
{
	DrawBitmap(rcView, x, y, rect, surf_no, false);
}*/

void PutBitmap3Transparent(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no, u32 alpha)
{
	//TODO: draw queueing
	//TODO: don't render if transparent
#ifdef TWO_SCREENS
	int temp = x;
	x = WINDOW_HEIGHT - y;
	y = temp;

	if((gCounter & 1) == 0) // bottom screen
	{
		
	}
	else
	{
		y -= WINDOW_WIDTH / 2;
	}

#endif

	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	int textureid = surf[surf_no].textureid;
	//if(!surf[surf_no].textureid) {textureid = gAtlas16Color1;}
////
	RECT srcRect = *rect;

	srcRect.top += surf[surf_no].yoffset;
	srcRect.bottom += surf[surf_no].yoffset;

	if(surf[surf_no].paletteType == GL_RGB16)
	{
		srcRect.left += surf[surf_no].xoffset;
		srcRect.right += surf[surf_no].xoffset;
	}
	else
	{
		// To compensate for the texture size being halved when turning from 16 to 256 color
		srcRect.left += surf[surf_no].xoffset / 2;
		srcRect.right += surf[surf_no].xoffset / 2;
	}

	if(rcView->left > x)
	{
		srcRect.left += rcView->left - x;
		x = rcView->left;
	}

	if(rcView->top > y)
	{
		srcRect.top += rcView->top - y;
		y = rcView->top;
	}

	int width = srcRect.right - srcRect.left;
	int height = srcRect.bottom - srcRect.top;
	if(x + width > rcView->right)
	{
		srcRect.right -= x + width - rcView->right;
	}

	if(y + height > rcView->bottom)
	{
		srcRect.bottom -= y + height - rcView->bottom;
	}
////

	//glSprite(x, y, rect, gAtlas16Color1, 0);
	glSpriteTransparent(x, y, &srcRect, surf[surf_no].textureid, surf[surf_no].paletteOffset, surf[surf_no].paletteType, alpha);
}

int GetSurfPixel(int x, int y, int surf_no)
{
		//color = (bitmap_pixels[pos*2] | bitmap_pixels[pos*2+1] << 4);
	int height = (y * surf[surf_no].w) / 2;
	
	if(x % 2 == 0)
		return surf[surf_no].data[x/2 + height].color & 0x0F;
	else
		return surf[surf_no].data[x/2 + height].color >> 4;
}

#ifdef JAPANESE
static void SetFontChar(int dx, int dy, int sx, int sy, int surf_no)
{
	int startx = sx * 12;
	int starty = sy * 13;

	bool odd = false;
	if(dx % 2 == 1) odd = true;

	for(int k=0;k<13;k++)
	{
		if(!odd)
		{
			int height = ((starty + k) * surf[SURFACE_ID_FONT].w) / 2;
			int height2 = ((dy + k) * surf[surf_no].w) / 2;
			memcpy(&surf[surf_no].data[dx/2 + height2], 
				&surf[SURFACE_ID_FONT].data[(startx/2 + height)],
				12/2);	
		}
		else
		{
			for(int j=0;j<6;j++)
			{
				if(j==0)
				{
					int level0Color = GetSurfPixel(startx+(j*2), starty+k, SURFACE_ID_FONT);
					SetSurf2Pixels(dx + j*2, k + dy, surf_no, (level0Color << 4));
				}
				else
				{
					int level0Color = GetSurfPixel(startx+(j*2-1), starty+k, SURFACE_ID_FONT);
					int level1Color = GetSurfPixel(startx+(j*2), starty+k, SURFACE_ID_FONT);
					SetSurf2Pixels(dx + j*2, k + dy, surf_no, (level0Color | level1Color << 4));
				}
			}
		}
	}
}

static void SetFontSymbol(int dx, int dy, int surf_no)
{
	RECT rcSymbol = {64, 48, 72, 56};	
	int startx = 64;
	int starty = 48;

	for(int k=0;k<56-48;k++)
	{	
		int height = ((starty + k) * surf[SURFACE_ID_TEXT_BOX].w) / 2;
		int height2 = ((dy + k) * surf[surf_no].w) / 2;
		memcpy(&surf[surf_no].data[dx/2 + height2], 
			&surf[SURFACE_ID_TEXT_BOX].data[(startx/2 + height)],
			(72-64)/2);	
	}
}


#endif


void SetSurf2Pixels(int x, int y, int surf_no, char color)
{
	int height = (y * surf[surf_no].w) / 2;
	surf[surf_no].data[x/2 + height].color = color;
}

void Surface2Surface(int x, int y, RECT *rect, int to, int from)
{
	BUFFER_PIXEL* dFrom = surf[from].data;
	BUFFER_PIXEL* dTo = surf[to].data;


	int fW = surf[from].w;
	int fH = surf[from].h;

	int tW = surf[to].w;
	int tH = surf[to].h;

	int rectW = rect->right - rect->left;
	int rectH = rect->bottom - rect->top;

	int surfStartH = rect->top; 
	for(int h = 0; h < rectH; h++)
	{

		memcpy(dTo + (tW*h/2) + (tW*y/2) + (x/2),
				dFrom+((rect->left/2) + (fW*surfStartH/2)),
				(rectW/2));
		surfStartH++;
	}
}

void Surface2Texture(int x, int y, RECT *rect, int to, int from)
{
	//TODO
	CopyDataToTexture(surf[from].paletteType, surf[from].textureid, from, x + surf[to].xoffset, y + surf[to].yoffset, rect);
}

unsigned long GetCortBoxColor(unsigned long col)
{
	// TODO
	return col;
}

void CortBox(RECT *rect, uint32_t col)
{
	const unsigned char col_red = col & 0x0000FF;
	const unsigned char col_green = (col & 0x00FF00) >> 8;
	const unsigned char col_blue = (col & 0xFF0000) >> 16;
	//const BUFFER_PIXEL colPixel = {col_red, col_green, col_blue};

	col = RGB15(col_red / 8, col_green / 8, col_blue / 8);
	
	glBoxFilled(rect->left, rect->top, rect->right, rect->bottom, col);

}

void CortBoxAlpha(RECT *rect, uint32_t col, u32 alpha)
{
	const unsigned char col_red = col & 0x0000FF;
	const unsigned char col_green = (col & 0x00FF00) >> 8;
	const unsigned char col_blue = (col & 0xFF0000) >> 16;
	//const BUFFER_PIXEL colPixel = {col_red, col_green, col_blue};

	col = RGB15(col_red / 8, col_green / 8, col_blue / 8);
	
	glBoxFilledTransparent(rect->left, rect->top, rect->right, rect->bottom, col, alpha);

}


void CortBox2(RECT *rect, uint32_t col, SurfaceID surf_no)
{
	if (surf[surf_no].data)
	{
		int width = (rect->right - rect->left)/2;
		int height = rect->bottom - rect->top;
		int sW = surf[surf_no].w/2;
		for(int i=0;i<height;i++)
		{
			memset(&surf[surf_no].data[rect->left/2 + (((rect->top + i) * sW))], col, width * sizeof(BUFFER_PIXEL));
		}
		
		if(surf_no == SURFACE_ID_TEXT_LINE1) TextLinesDirty[0] = true;
		if(surf_no == SURFACE_ID_TEXT_LINE2) TextLinesDirty[1] = true;
		if(surf_no == SURFACE_ID_TEXT_LINE3) TextLinesDirty[2] = true;
		if(surf_no == SURFACE_ID_TEXT_LINE4) TextLinesDirty[3] = true;	
		if(surf_no == SURFACE_ID_ROOM_NAME) TextLinesDirty[4] = true;		
		if(surf_no == SURFACE_ID_CREDIT_CAST) TextLinesDirty[5] = true;
	}
}

void InitTextObject()
{
	MakeSurface_File("smalfont", SURFACE_ID_FONT);
}

static RECT GetFontRect(char character)
{
	RECT rect;
	bool found = false;
	int j,k;
	for(j=0;j<3;j++)
	{
		for(k=0;k<33;k++)
		{
			if(font_letters[j][k] == character)
			{	
				found = true;
				break;
			}
		}
		if(found) break;
	}
	rect = {k*6, j*12, k*6+6, j*12+12};
	return rect;
}

int GetTextSpacing(const char *text)
{
#ifndef JAPANESE
	int spacing = 0;
	char v;
	while (v = *text++)
	{
	    if ((v -= 0x20) >= 0x00 && v <= 0x60)
	    {
	        spacing += font_space[v];
	    }
	}
	return spacing;
#else

	int spacing = 0;
	int i = 0;
	while(text[i] != NULL)
	{
		if(text[i] & 0x80)
		{
			//hardcode this fucking character   ･
			if(text[i] == 0xa5)
			{
				spacing += font_space[17 + (6+3*32)];
				i++; continue;
			}
			bool found = false;
			for(int yy=0; yy<33;yy++)
			{
				for(int xx=0; xx<32; xx++)
				{
					if(text[i] == font_letters_shift_jis[xx*2 + (yy*32*2)] && text[i+1] == font_letters_shift_jis[xx*2 + (yy*32*2)+1])
					{
						spacing += font_space[xx +((yy+3)*32)];
						found = true;
						break;
					}
					
				}
				if(found) break;
			}
			i+=2;
		}
		else
		{
			char v = text[i];
	    	if ((v -= 0x20) >= 0x00 && v <= 0x60)
	    	{
	    	    spacing += font_space[v];
	    	}
			i++;
		}
	}
	return spacing;
#endif
}

void PutText(RECT* rcView, int x, int y, const char *text, unsigned long colour)
{

    char v;

	const unsigned char col_red = colour & 0x0000FF;
	const unsigned char col_green = (colour & 0x00FF00) >> 8;
	const unsigned char col_blue = (colour & 0xFF0000) >> 16;
	colour = RGB15(col_red / 8, col_green / 8, col_blue / 8);
	

	glColor(colour);
    while (v = *text++)
    {
        if ((v -= 0x20) >= 0x00 && v <= 0x60)
        {
			if(v == '=' - 0x20)
			{
				RECT rcSymbol = {64, 48, 72, 56};
				//PutBitmap3(rcView, x, y + 2, &rcSymbol, SURFACE_ID_TEXT_BOX);
				x += font_space[v];
			}
			else
			{
#ifndef JAPANESE				
				RECT rect = {(v & 0x1F) << 3, (v / 32) * 12, ((v & 0x1F) + 1) << 3, ((v / 32) + 1) * 12};
#else
				RECT rect = {(v & 0x1F) * 12, (v / 32) * 13, ((v & 0x1F) + 1) *12, ((v / 32) + 1) * 13};
#endif
            	PutBitmap3(rcView, x, y, &rect, SURFACE_ID_FONT);
            	x += font_space[v];
			}

        }
    }
	glColor( 0x7FFF );
}

void PutText2(int x, int y, const char *text, unsigned long color, SurfaceID surf_no)
{
#ifdef JAPANESE
	int i = 0;

	int dx = 0;
	
	//32 * 33
	RECT rect = {0,0,216,16};
	while(text[i] != NULL)
	{
		if(text[i] & 0x80)
		{
			//hardcode this fucking character   ･
			if(text[i] == 0xa5)
			{
				SetFontChar(x, y, 17, 6+3, surf_no);
				x += font_space[17 + (6+3*32)];
				i++; continue;
			}
			bool found = false;
			for(int yy=0; yy<33;yy++)
			{
				for(int xx=0; xx<32; xx++)
				{
					if(text[i] == font_letters_shift_jis[xx*2 + (yy*32*2)] && text[i+1] == font_letters_shift_jis[xx*2 + (yy*32*2)+1])
					{
						//printf("%x%x\n", font_letters_shift_jis[xx*2 + (yy*32*2)], font_letters_shift_jis[xx*2 + (yy*32*2)+1]);
						SetFontChar(x, y, xx, yy+3, surf_no);
						x += font_space[xx +((yy+3)*32)];
						found = true;
						break;
					}
					
				}
				if(found) break;
			}
			//if(!found) printf("%x%x wtf\n", text[i], text[i+1]);
			i+=2;
		}
		else
		{
			char v = text[i];
			if ((v -= 0x20) >= 0x00 && v <= 0x60)
        	{
				if(v == '=' - 0x20)
				{
					//RECT rcSymbol = {64, 48, 72, 56};
					SetFontSymbol(x-1, y+2, surf_no);
				}
				else
				{
        	    	SetFontChar(x, y, (v)%32, (v)/32, surf_no);
				}
		    	x += font_space[v];

        	}
			i++;
		}
	}
	if(surf_no == SURFACE_ID_ROOM_NAME)
	{
		TextLinesDirty[4] = true;
	}
	else if (surf_no == SURFACE_ID_CREDIT_CAST)
	{
		TextLinesDirty[5] = true;
	}
	else
	{
		TextLinesDirty[surf_no - 30] = true;
	}
	
	

#endif
}


void EndTextObject()
{
	ReleaseSurface(SURFACE_ID_FONT);
}
