// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "MiniMap.h"

#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Escape.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MyChar.h"
#include "Stage.h"
#include "Game.h"



#include "ArmsItem.h"
#include "Back.h"
#include "Boss.h"
#include "BossLife.h"
#include "BulHit.h"
#include "Bullet.h"
#include "Caret.h"
#include "CommonDefines.h"
#include "Debug.h"
#include "Draw.h"
#include "Ending.h"
#include "Escape.h"
#include "Fade.h"
#include "Flags.h"
#include "Flash.h"
#include "Frame.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MiniMap.h"
#include "nifi.h"
#include "MyChar.h"
#include "MycHit.h"
#include "MycParam.h"
#include "NpChar.h"
#include "NpcHit.h"
#include "NpcTbl.h"
#include "Pause.h"
#include "Profile.h"
#include "Random.h"
#include "SelStage.h"
#include "Shoot.h"
#include "Sound.h"
#include "Stage.h"
#include "Star.h"
#include "TextScr.h"
#include "ValueView.h"

bool gInMinimap = false;

void WriteMiniMapLine()
{
	int x;
	int line;
	unsigned char a;

	RECT rcLevel[4] = {
		{240, 24, 241, 25},
		{241, 24, 242, 25},
		{242, 24, 243, 25},
		{243, 24, 244, 25},
	};

	int level0Color = GetSurfPixel(rcLevel[0].left, rcLevel[0].top, SURFACE_ID_TEXT_BOX);
	int level1Color = GetSurfPixel(rcLevel[1].left, rcLevel[1].top, SURFACE_ID_TEXT_BOX);
	int level2Color = GetSurfPixel(rcLevel[2].left, rcLevel[2].top, SURFACE_ID_TEXT_BOX);
	int level3Color = GetSurfPixel(rcLevel[3].left, rcLevel[3].top, SURFACE_ID_TEXT_BOX);

	for (line = 0; line < gMap.length; line++)
	{
		char colorHigh = 0;
		char colorLow = 0;

		for (x = 0; x < gMap.width; x++)
		{
			a = GetAttribute(x, line);

			// Yup, this really is an if/else chain.
			// No switch here.
			if (a == 0)
			{
				if(x % 2 == 0) colorHigh = level0Color;
				else colorLow = level0Color;
			}
			else if (a == 68 ||
				a == 1 ||
				a == 64 ||
				a == 128 ||
				a == 129 ||
				a == 130 ||
				a == 131 ||
				a == 81 ||
				a == 82 ||
				a == 85 ||
				a == 86 ||
				a == 2 ||
				a == 96 ||
				a == 113 ||
				a == 114 ||
				a == 117 ||
				a == 118 ||
				a == 160 ||
				a == 161 ||
				a == 162 ||
				a == 163)
			{
				if(x % 2 == 0) colorHigh = level1Color;
				else colorLow = level1Color;
			}
				
			else if (a == 67 ||
				a == 99 ||
				a == 80 ||
				a == 83 ||
				a == 84 ||
				a == 87 ||
				a == 96 ||	// This is already listed above, so that part of the expression is always false
				a == 112 ||
				a == 115 ||
				a == 116 ||
				a == 119)
			{
				if(x % 2 == 0) colorHigh = level2Color;
				else colorLow = level2Color;
			}
				
			else
			{
				if(x % 2 == 0) colorHigh = level3Color;
				else colorLow = level3Color;
			}

			if(colorHigh && colorLow)
			{
				char color = (colorHigh | colorLow << 4);
				SetSurf2Pixels(x, line, SURFACE_ID_MAP, color);
				colorHigh = colorLow = 0;
			}
			if(x == gMap.width - 1 && x % 2 == 0)
			{
				char color = (colorHigh | 0 << 4);
				SetSurf2Pixels(x, line, SURFACE_ID_MAP, color);
				colorHigh = colorLow = 0;
			}
		}
	}
	RECT rect = {0, 0, gMap.width+16, gMap.length};
	CopyDataToTexture(surf[SURFACE_ID_MAP].paletteType, surf[SURFACE_ID_MAP].textureid, SURFACE_ID_MAP, 
		surf[SURFACE_ID_MAP].xoffset, surf[SURFACE_ID_MAP].yoffset, &rect);
}

int MiniMapLoop(void)
{
	int f, line;
	RECT rcView;
	RECT rcMiniMap;

	int my_x, my_x2;
	int my_y, my_y2;
	unsigned char my_wait;
	RECT my_rect = {0, 57, 1, 58};

	my_x = ((gMC.x / 0x200) + 8) / 16;
	my_y = ((gMC.y / 0x200) + 8) / 16;

	my_x2 = ((gMCP2.x / 0x200) + 8) / 16;
	my_y2 = ((gMCP2.y / 0x200) + 8) / 16;

	if(gMap.length > 80)
	{
		surf[SURFACE_ID_MAP].xoffset = 888;
		surf[SURFACE_ID_MAP].yoffset = 0;
		surf[SURFACE_ID_MAP].textureid = gAtlas16Color2;
	}
	else
	{
		surf[SURFACE_ID_MAP].xoffset = 256;
		surf[SURFACE_ID_MAP].yoffset = 416;
		surf[SURFACE_ID_MAP].textureid = gAtlas16Color1;
	}

	free(surf[SURFACE_ID_MAP].data);
	surf[SURFACE_ID_MAP].data = (BUFFER_PIXEL*)malloc(surf[SURFACE_ID_MAP].w * surf[SURFACE_ID_MAP].h * sizeof(BUFFER_PIXEL));

	WriteMiniMapLine();
	for (f = 0; f <= 8; ++f)
	{
		GetTrg();

		if ((gKeyTrg | gKeyTrgP2) & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		//PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		int frame_x;
		int frame_y;
		unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));
		CortBox(&grcFull, color);
		GetFramePosition(&frame_x, &frame_y);
		
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutBullet(frame_x, frame_y);
		PutMyChar(frame_x, frame_y);
		if(SwapMyChar())
		{
			PutMyChar(frame_x, frame_y);
			SwapMyChar();
		}
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();

		rcView.left = (WINDOW_WIDTH / 2) - (((gMap.width * f) / 8) / 2);
		rcView.right = (WINDOW_WIDTH / 2) + (((gMap.width * f) / 8) / 2);
		rcView.top = (WINDOW_HEIGHT / 2) - (((gMap.length * f) / 8) / 2);
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((gMap.length * f) / 8) / 2);

		PutMapName(TRUE);
		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;

		gCounter++;
	}

	rcMiniMap.left = 0;
	rcMiniMap.right = gMap.width;
	rcMiniMap.top = 0;
	rcMiniMap.bottom = 0;

	rcView.right = --rcView.left + gMap.width + 2;
	rcView.bottom = --rcView.top + gMap.length + 2;
	//CortBox2(&rcMiniMap, 0, SURFACE_ID_MAP);

	line = 0;
	my_wait = 0;
	while (1)
	{
		GetTrg();

		if (gKeyTrg & (gKeyOk | gKeyCancel))
			break;

		if ((gKeyTrg | gKeyTrgP2) & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		//PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);
		int frame_x;
		int frame_y;
		unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));
		CortBox(&grcFull, color);		
		GetFramePosition(&frame_x, &frame_y);
		
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutBullet(frame_x, frame_y);
		PutMyChar(frame_x, frame_y);
		if(SwapMyChar())
		{
			PutMyChar(frame_x, frame_y);
			SwapMyChar();
		}
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();

		CortBox(&rcView, 0);

		if (line < gMap.length)
		{

			++line;
		}
		// I guess Pixel duplicated this block of code because he
		// wanted the minimap to draw faster?
		if (line < gMap.length)
		{

			++line;
		}

		if (gKey & CEY_RIGHT)
		{
			rcView.left -= 1;
			rcView.right -= 1;
		}
		if (gKey & CEY_LEFT)
		{
			rcView.left += 1;
			rcView.right += 1;
		}

		if (gKey & CEY_DOWN)
		{
			rcView.top -= 1;
			rcView.bottom -= 1;
		}
		if (gKey & CEY_UP)
		{
			rcView.top += 1;
			rcView.bottom += 1;
		}
			
			
				
		rcMiniMap.bottom = line;
		PutBitmap3(&grcGame, rcView.left + 1, rcView.top + 1, &rcMiniMap, SURFACE_ID_MAP);

		PutMapName(TRUE);

		if (++my_wait / 8 % 2)
			PutBitmap3(&grcGame, my_x + rcView.left + 1, my_y + rcView.top + 1, &my_rect, SURFACE_ID_TEXT_BOX);

		if (my_wait / 8 % 2 && nifiIsLinked())
			PutBitmap3(&grcGame, my_x2 + rcView.left + 1, my_y2 + rcView.top + 1, &my_rect, SURFACE_ID_TEXT_BOX);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;

		
		gCounter++;
	}

	for (f = 8; f >= -1; --f)
	{
		GetTrg();

		if ((gKeyTrg | gKeyTrgP2) & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;
			}
		}

		//PutBitmap4(&grcGame, 0, 0, &grcGame, SURFACE_ID_SCREEN_GRAB);

		int frame_x;
		int frame_y;
		unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));
		CortBox(&grcFull, color);		
		GetFramePosition(&frame_x, &frame_y);
		
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutBullet(frame_x, frame_y);
		PutMyChar(frame_x, frame_y);
		if(SwapMyChar())
		{
			PutMyChar(frame_x, frame_y);
			SwapMyChar();
		}
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();

		rcView.left = (WINDOW_WIDTH / 2) - (((gMap.width * f) / 8) / 2);
		rcView.right = (WINDOW_WIDTH / 2) + (((gMap.width * f) / 8) / 2);
		rcView.top = (WINDOW_HEIGHT / 2) - (((gMap.length * f) / 8) / 2);
		rcView.bottom = (WINDOW_HEIGHT / 2) + (((gMap.length * f) / 8) / 2);

		PutMapName(TRUE);
		CortBox(&rcView, 0);

		PutFramePerSecound();
		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;


		gCounter++;
	}
		int frame_x;
		int frame_y;
		unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));
		CortBox(&grcFull, color);		
		GetFramePosition(&frame_x, &frame_y);
		

		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutBullet(frame_x, frame_y);
		PutMyChar(frame_x, frame_y);
		if(SwapMyChar())
		{
			PutMyChar(frame_x, frame_y);
			SwapMyChar();
		}
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();

	return enum_ESCRETURN_continue;
}

char gMapping[0x80];

BOOL IsMapping(void)
{
	if (!gMapping[gStageNo])
		return FALSE;
	else
		return TRUE;
}

void StartMapping(void)
{
	memset(gMapping, FALSE, sizeof(gMapping));
}

void SetMapping(int a)
{
	gMapping[a] = TRUE;
}
