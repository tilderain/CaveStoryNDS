// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "MapName.h"

#include <string.h>

#include "CommonDefines.h"
#include "Draw.h"
#include "WindowsWrapper.h"

typedef struct MAP_NAME
{
	BOOL flag;
	int wait;
	char name[0x20];
} MAP_NAME;

MAP_NAME gMapName;
RECT rc = { 0, 0, 160, 13 };

void ReadyMapName(const char *str)
{
	int a;

	// Handle "Studio Pixel presents" text in the intro, using an obfuscated string
	unsigned char presentText[24] = {
#ifdef JAPANESE
		// "開発室Pixel presents"
		0x8A - 1,	// 開
		0x4A - 1,
		0x94 - 1,	// 発
		0xAD - 1,
		0x8E - 1,	// 室
		0xBA - 1,
		'P' - 1,
		'i' - 1,
		'x' - 1,
		'e' - 1,
		'l' - 1,
		' ' - 1,
		'p' - 1,
		'r' - 1,
		'e' - 1,
		's' - 1,
		'e' - 1,
		'n' - 1,
		't' - 1,
		's' - 1,
#else
		// "  Studio Pixel presents"
		' ' - 1,
		' ' - 1,
		'S' - 1,
		't' - 1,
		'u' - 1,
		'd' - 1,
		'i' - 1,
		'o' - 1,
		' ' - 1,
		'P' - 1,
		'i' - 1,
		'x' - 1,
		'e' - 1,
		'l' - 1,
		' ' - 1,
		'p' - 1,
		'r' - 1,
		'e' - 1,
		's' - 1,
		'e' - 1,
		'n' - 1,
		't' - 1,
		's' - 1,
#endif
		0xFF
	};

	// Reset map name flags
	gMapName.flag = FALSE;
	gMapName.wait = 0;

	if (!strcmp(str, "u"))
	{
		for (a = 0; a < (int)sizeof(presentText); ++a)
			presentText[a] = presentText[a] + 1;

		str = (char*)presentText;
	}

	// Copy map's name to the global map name
	
	strcpy(gMapName.name, str);
#ifdef JAPANESE
	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2(0, 0, str, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME);
#endif
}


void PutMapName(BOOL bMini)
{
	// 'unused_rect' isn't the original name. The Linux port optimised this out, so there's no known name for it.
	RECT unused_rect = {0, 0, 160, 16};
	int spacing = GetTextSpacing(gMapName.name);
	if (bMini)
	{
		// Map system
		RECT rcBack;
		rcBack.left = 0;
		rcBack.right = WINDOW_WIDTH;
		rcBack.top = 7;
		rcBack.bottom = 24;

		CortBoxAlpha(&rcBack, 0x000000, 12);
#ifndef JAPANESE
		PutText(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 12, gMapName.name, RGB(0x11, 0x00, 0x22));
		PutText(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 11, gMapName.name, RGB(0xFF, 0xFF, 0xFE));
#else
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 10, &rc, SURFACE_ID_ROOM_NAME);
#endif
	}
	else if (gMapName.flag)
	{
		// MNA
#ifndef JAPANESE
		PutText(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 71, gMapName.name, RGB(0x11, 0x00, 0x22));
		PutText(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 70, gMapName.name, RGB(0xFF, 0xFF, 0xFE));
#else
		int colour = RGB(0x11, 0x00, 0x22);
		const unsigned char col_red = colour & 0x0000FF;
		const unsigned char col_green = (colour & 0x00FF00) >> 8;
		const unsigned char col_blue = (colour & 0xFF0000) >> 16;
		colour = RGB15(col_red / 8, col_green / 8, col_blue / 8);
		glColor(colour);
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 71, &rc, SURFACE_ID_ROOM_NAME);
		glColor(0x7fff);
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - (spacing/2), 70, &rc, SURFACE_ID_ROOM_NAME);
#endif
		if (++gMapName.wait > 160)
			gMapName.flag = FALSE;
	}
}

void StartMapName(void)
{
	gMapName.flag = TRUE;
	gMapName.wait = 0;
}

void RestoreMapName(void)
{
#ifndef JAPANESE
	return;
#endif
	int a = (int)strlen(gMapName.name);

	CortBox2(&rc, 0, SURFACE_ID_ROOM_NAME);
	PutText2(((160 - (a * 6)) / 2) + 6, 1, gMapName.name, RGB(0x11, 0x00, 0x22), SURFACE_ID_ROOM_NAME);
	PutText2(((160 - (a * 6)) / 2) + 6, 0, gMapName.name, RGB(0xFF, 0xFF, 0xFE), SURFACE_ID_ROOM_NAME);
}
