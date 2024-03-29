// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "SelStage.h"

#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Escape.h"
#include "KeyControl.h"
#include "Main.h"
#include "Sound.h"
#include "TextScr.h"


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
#include "Game.h"
#include "Generic.h"
#include "GenericLoad.h"
#include "KeyControl.h"
#include "Main.h"
#include "Map.h"
#include "MapName.h"
#include "MiniMap.h"
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


PERMIT_STAGE gPermitStage[8];

int gSelectedStage;
int gStageSelectTitleY;

void ClearPermitStage(void)
{
	memset(gPermitStage, 0, sizeof(gPermitStage));
}

BOOL AddPermitStage(int index, int event)
{
	int i = 0;

	while (i < 8)
	{
		if (gPermitStage[i].index == index)
			break;

		if (gPermitStage[i].index == 0)
			break;

		++i;
	}

	if (i == 8)
		return FALSE;

	gPermitStage[i].index = index;
	gPermitStage[i].event = event;

	return TRUE;
}

BOOL SubPermitStage(int index)
{
	int i;

	for (i = 0; i < 8; ++i)
		if (gPermitStage[i].index == index)
			break;

#ifdef FIX_BUGS
	if (i == 8)
#else
	if (i == 32)
#endif
		return FALSE;

	for (++i; i < 8; ++i)
		gPermitStage[i - 1] = gPermitStage[i];

	gPermitStage[i - 1].index = 0;
	gPermitStage[i - 1].event = 0;

	return TRUE;
}

void MoveStageSelectCursor(void)
{
	int stage_num;
	int stage_x;

	stage_num = 0;
	while (gPermitStage[stage_num].index != 0)
		++stage_num;

	stage_x = (WINDOW_WIDTH - (stage_num * 40)) / 2;	// Unused

	if (stage_num == 0)
		return;

	if (gKeyTrg & gKeyLeft)
		--gSelectedStage;

	if (gKeyTrg & gKeyRight)
		++gSelectedStage;

	if (gSelectedStage < 0)
		gSelectedStage = stage_num - 1;

	if (gSelectedStage > stage_num - 1)
		gSelectedStage = 0;

	if (gKeyTrg & (gKeyLeft | gKeyRight))
		StartTextScript(gPermitStage[gSelectedStage].index + 1000);

	if (gKeyTrg & (gKeyLeft | gKeyRight))
		PlaySoundObject(1, 1);
}

void PutStageSelectObject(void)
{
	static unsigned int flash;

	int i;
	RECT rcStage;

	RECT rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	RECT rcCur[2] = {
		{80, 88, 112, 104},
		{80, 104, 112, 120},
	};

	RECT rcTitle1 = {80, 64, 144, 72};

	int stage_num;
	int stage_x;

	if (gStageSelectTitleY > (WINDOW_HEIGHT / 2) - 74)
		--gStageSelectTitleY;

	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 32, gStageSelectTitleY, &rcTitle1, SURFACE_ID_TEXT_BOX);

	stage_num = 0;
	while (gPermitStage[stage_num].index)
		++stage_num;

	++flash;

	if (stage_num != 0)
	{
		stage_x = (WINDOW_WIDTH - (stage_num * 40)) / 2;

		PutBitmap3(&rcView, stage_x + (gSelectedStage * 40), (WINDOW_HEIGHT / 2) - 56, &rcCur[flash / 2 % 2], SURFACE_ID_TEXT_BOX);

		for (i = 0; i < 8; ++i)
		{
			if (gPermitStage[i].index == 0)
				break;

			rcStage.left = (gPermitStage[i].index % 8) * 32;
			rcStage.right = rcStage.left + 32;
			rcStage.top = (gPermitStage[i].index / 8) * 16;
			rcStage.bottom = rcStage.top + 16;

			PutBitmap3(&rcView, stage_x + (i * 40), (WINDOW_HEIGHT / 2) - 56, &rcStage, SURFACE_ID_STAGE_ITEM);
		}
	}
}

int StageSelectLoop(int *p_event)
{
	char old_script_path[MAX_PATH];

	RECT rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	gSelectedStage = 0;
	BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcFull);
	GetTextScriptPath(old_script_path);
	LoadTextScript2("StageSelect.tsc");
	gStageSelectTitleY = (WINDOW_HEIGHT / 2) - 66;
	StartTextScript(gPermitStage[gSelectedStage].index + 1000);

	for (;;)
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

		MoveStageSelectCursor();

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;
		}

#ifdef FIX_BUGS
		//PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
#else
		// The original accidentally drew the screencap with transparency enabled
		//PutBitmap3(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
#endif
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
		PutStar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutFlash();
		PutCaret(frame_x, frame_y);
		PutValueView(frame_x, frame_y);
		PutBossLife();
		PutFade();

		PutStageSelectObject();
		PutTextScript();

		if (gKeyTrg & gKeyOk)
		{
			StopTextScript();
			break;
		}
		else if (gKeyTrg & gKeyCancel)
		{
			StopTextScript();
			LoadTextScript_Stage(old_script_path);
			*p_event = 0;
			return enum_ESCRETURN_continue;
		}

		PutFramePerSecound();

		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;

		++gCounter;
	}

	LoadTextScript_Stage(old_script_path);
	*p_event = gPermitStage[gSelectedStage].event;
	return enum_ESCRETURN_continue;
}
