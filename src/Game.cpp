// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Game.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "WindowsWrapper.h"

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

int g_GameFlags;
int gCounter;

int gCursorPos;

int Random(int min, int max)
{
	const int range = max - min + 1;
	return (msvc_rand() % range) + min;
}

void PutNumber4(int x, int y, int value, BOOL bZero)
{
	// Define rects
	RECT rcClient = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	RECT rect[10] = {
		{0, 56, 8, 64},
		{8, 56, 16, 64},
		{16, 56, 24, 64},
		{24, 56, 32, 64},
		{32, 56, 40, 64},
		{40, 56, 48, 64},
		{48, 56, 56, 64},
		{56, 56, 64, 64},
		{64, 56, 72, 64},
		{72, 56, 80, 64},
	};

	// Digits
	int tbl[4] = {1000, 100, 10, 1};

	int a;
	int sw;
	int offset;

	// Limit value
	if (value > 9999)
		value = 9999;

	// Go through number and draw digits
	offset = 0;
	sw = 0;
	while (offset < 4)
	{
		// Get the digit that this is
		a = 0;

		while (value >= tbl[offset])
		{
			value -= tbl[offset];
			++a;
			++sw;
		}

		// Draw digit
		if ((bZero && offset == 2) || sw != 0 || offset == 3)
			PutBitmap3(&rcClient, x + 8 * offset, y, &rect[a], SURFACE_ID_TEXT_BOX);

		// Go to next digit
		++offset;
	}
}

int ModeOpening(void)
{
	int frame_x;
	int frame_y;
	unsigned int wait;

	InitNpChar();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();
	InitBossLife();
	ChangeMusic(MUS_SILENCE);
	TransferStage(72, 100, 3, 3);
	SetFrameTargetMyChar(16);
	SetFadeMask();

	// Reset cliprect and flags
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif

	
	// To make up for casts obliterating these by its mere presence
	if(npcSymInArmsSlot)
	{
		npcSymInArmsSlot = false;
		ReloadBitmap_File("Npc/NpcSym", SURFACE_ID_NPC_SYM);
		ReloadBitmap_File("Bullet", SURFACE_ID_BULLET);
		ReloadBitmap_File("Arms", SURFACE_ID_ARMS);
	
		// also this
		ReloadBitmap_File("Caret", SURFACE_ID_CARET);
		ReloadBitmap_File("ItemImage", SURFACE_ID_ITEM_IMAGE);
	}

	g_GameFlags = 3;

	CutNoise();

	wait = 0;
	while (wait < 500)
	{
		// Increase timer
		++wait;

		// Get pressed keys
		GetTrg();

		// Escape menu
		if (gKey & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 1;
			}
		}

		// Skip intro if OK is pressed
		if (gKeyTrg & gKeyOk)
			break;

		// Update everything
		ActNpChar();
		ActBossChar();
		ActBack();
		ResetMyCharFlag();
		HitMyCharMap();
		HitMyCharNpChar();
		HitMyCharBoss();
		HitNpCharMap();
		HitBossMap();
		HitBossBullet();
		ActCaret();
		MoveFrame3();
		ProcFade();

		// Draw everything
		CortBox(&grcFull, 0x000000);

		//GetFramePosition(&frame_x, &frame_y);
		frame_x = 32 * 0x200; frame_y = 16 * 0x200;
		PutBack(frame_x, frame_y);
		PutStage_Back(frame_x, frame_y);
		PutBossChar(frame_x, frame_y);
		PutNpChar(frame_x, frame_y);
		PutMapDataVector(frame_x, frame_y);
		PutStage_Front(frame_x, frame_y);
		PutFront(frame_x, frame_y);
		PutCaret(frame_x, frame_y);
		PutFade();

		// Update Text Script
		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return 0;

			case enum_ESCRETURN_restart:
				return 1;
		}

		PutMapName(FALSE);
		PutTextScript();
		PutFramePerSecound();

		if (!Flip_SystemTask())
			return 0;

		++gCounter;
	}

	for (wait = 0; wait < 500; wait += 20)
	{
		CortBox(&grcGame, 0x000000);
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return 0;
	}
	return 2;
}

int ModeTitle(void)
{
	// Set rects
	RECT rcTitle = {0, 0, 144, 40};
	RECT rcPixel = {0, 0, 160, 16};
	RECT rcNew = {144, 0, 192, 16};
	RECT rcContinue = {144, 16, 192, 32};

	RECT rcVersion = {152, 80, 208, 88};
	RECT rcPeriod = {152, 88, 208, 96};

	// Character rects
	RECT rcMyChar[4] = {
		{0, 16, 16, 32},
		{16, 16, 32, 32},
		{0, 16, 16, 32},
		{32, 16, 48, 32},
	};

	RECT rcCurly[4] = {
		{0, 112, 16, 128},
		{16, 112, 32, 128},
		{0, 112, 16, 128},
		{32, 112, 48, 128},
	};

	RECT rcToroko[4] = {
		{64, 80, 80, 96},
		{80, 80, 96, 96},
		{64, 80, 80, 96},
		{96, 80, 112, 96},
	};

	RECT rcKing[4] = {
		{224, 48, 240, 64},
		{288, 48, 304, 64},
		{224, 48, 240, 64},
		{304, 48, 320, 64},
	};

	RECT rcSu[4] = {
		{0, 16, 16, 32},
		{32, 16, 48, 32},
		{0, 16, 16, 32},
		{48, 16, 64, 32},
	};

	unsigned int wait;

	int anime;
	int v1, v2, v3, v4;

	RECT char_rc;
	int char_type;
	int time_counter;
	int char_y;
	SurfaceID char_surf;
	unsigned long back_color;

	// Reset everything
	InitCaret();
	InitStar();
	CutNoise();

	// Create variables
	anime = 0;
	char_type = 0;
	time_counter = 0;
	back_color = GetCortBoxColor(RGB(0x20, 0x20, 0x20));

	GetCompileVersion(&v1, &v2, &v3, &v4);

	// Set state
	if (IsProfile())
		gCursorPos = 1;
	else
		gCursorPos = 0;

	// Set character
	time_counter = LoadTimeCounter();

	if (time_counter && time_counter < 6 * 60 * 50)	// 6 minutes
		char_type = 1;
	if (time_counter && time_counter < 5 * 60 * 50)	// 5 minutes
		char_type = 2;
	if (time_counter && time_counter < 4 * 60 * 50)	// 4 minutes
		char_type = 3;
	if (time_counter && time_counter < 3 * 60 * 50)	// 3 minutes
		char_type = 4;

	// Set music to character's specific music
	if (char_type == 1)
		ChangeMusic(MUS_RUNNING_HELL);
	else if (char_type == 2)
		ChangeMusic(MUS_TOROKOS_THEME);
	else if (char_type == 3)
		ChangeMusic(MUS_WHITE);
	else if (char_type == 4)
		ChangeMusic(MUS_SAFETY);
	else
		ChangeMusic(MUS_CAVE_STORY);

	// Reset cliprect, flags, and give the player the Nikumaru counter
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif

	g_GameFlags = 0;
	gMC.equip |= 0x100;

	// Start loop
	wait = 0;

	ReloadBitmap_File("BITMAP/PIXEL", SURFACE_ID_PIXEL);
	ReloadBitmap_File("Title", SURFACE_ID_TITLE);
	strcpy(surf[SURFACE_ID_LEVEL_SPRITESET_1].name, "asdf");

	while (1)
	{
		// Don't accept selection for 10 frames
		if (wait < 10)
			++wait;

		// Get pressed keys
		GetTrg();

		// Quit when OK is pressed
		if (wait >= 10)
		{
			if (gKeyTrg & gKeyOk)
			{
				if(gCursorPos == 2)
				{
					switch (Call_Pause())
					{
						case enum_ESCRETURN_exit:
							return 0;

						case enum_ESCRETURN_restart:
							return 1;
					}
					continue;
				}


				PlaySoundObject(18, 1);
				break;
			}
		}

		if (gKey & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 1;
			}
		}

		// Move cursor
		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			PlaySoundObject(1, 1);

			if (gKeyTrg & gKeyUp)
			{
				gCursorPos -= 1;
				if (gCursorPos < 0) gCursorPos = 2;
			}
			else
			{
				gCursorPos += 1;
				if (gCursorPos > 2) gCursorPos = 0;
			}
		}

		// Update carets
		ActCaret();

		// Animate character cursor
		if (++anime >= 40)
			anime = 0;

		// Draw title
		CortBox(&grcGame, back_color);

		// Draw version
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 60, WINDOW_HEIGHT - 24, &rcVersion, SURFACE_ID_TEXT_BOX);
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 4, WINDOW_HEIGHT - 24, &rcPeriod, SURFACE_ID_TEXT_BOX);

		PutNumber4((WINDOW_WIDTH / 2) - 20, WINDOW_HEIGHT - 24, v1, FALSE);
		PutNumber4((WINDOW_WIDTH / 2) - 4, WINDOW_HEIGHT - 24, v2, FALSE);
		PutNumber4((WINDOW_WIDTH / 2) + 12, WINDOW_HEIGHT - 24, v3, FALSE);
		PutNumber4((WINDOW_WIDTH / 2) + 28, WINDOW_HEIGHT - 24, v4, FALSE);

		// Draw main title
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 72, 40, &rcTitle, SURFACE_ID_TITLE);
		//PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 24, (WINDOW_HEIGHT / 2) + -8, &rcNew, SURFACE_ID_TITLE);
		//PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 24, (WINDOW_HEIGHT / 2) + 28 - 8 - 8, &rcContinue, SURFACE_ID_TITLE);
		PutText(&grcGame, (WINDOW_WIDTH / 2) - 22,  (WINDOW_HEIGHT / 2) - 4, "New", RGB(0xf7, 0xf7, 0xea));
		PutText(&grcGame, (WINDOW_WIDTH / 2) - 22,  (WINDOW_HEIGHT / 2) + 28 - 8 - 8, "Continue", RGB(0xf7, 0xf7, 0xea));
		PutText(&grcGame, (WINDOW_WIDTH / 2) - 22,  (WINDOW_HEIGHT / 2) + 28 , "Config", RGB(0xf7, 0xf7, 0xea));
		PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 80, WINDOW_HEIGHT - 48, &rcPixel, SURFACE_ID_PIXEL);

		// Draw character cursor
		switch (char_type)
		{
			case 0:
				char_rc = rcMyChar[anime / 10 % 4];
				char_surf = SURFACE_ID_MY_CHAR;
				break;
			case 1:
				char_rc = rcCurly[anime / 10 % 4];
				char_surf = SURFACE_ID_NPC_REGU;
				break;
			case 2:
				char_rc = rcToroko[anime / 10 % 4];
				char_surf = SURFACE_ID_NPC_REGU;
				break;
			case 3:
				char_rc = rcKing[anime / 10 % 4];
				char_surf = SURFACE_ID_NPC_REGU;
				break;
			case 4:
				char_rc = rcSu[anime / 10 % 4];
				char_surf = SURFACE_ID_NPC_REGU;
				break;
		}

		if (gCursorPos == 0)
			char_y = (WINDOW_HEIGHT / 2) + - 7;
		else if (gCursorPos == 1)
			char_y = (WINDOW_HEIGHT / 2) + 27 - 18;
		else if (gCursorPos == 2)
			char_y = (WINDOW_HEIGHT / 2) + 27 - 4;

		// Pixel being redundant
		//if (!bContinue)
		//	PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 44, char_y, &char_rc, char_surf);
		//else
			PutBitmap3(&grcGame, (WINDOW_WIDTH / 2) - 44, char_y, &char_rc, char_surf);

		// Draw carets
		PutCaret(0, 0);

		if (time_counter)
			PutTimeCounter(16, 8);

		PutFramePerSecound();


		if (!Flip_SystemTask())
			return 0;
	}

	ChangeMusic(MUS_SILENCE);

	for (wait = 0; wait < 500; wait += 20)
	{
		CortBox(&grcGame, 0x000000);
		PutFramePerSecound();
		if (!Flip_SystemTask())
			return 0;
	}

	return 3;
}

int ModeAction(void)
{
	int frame_x;
	int frame_y;

	unsigned int swPlay;
	unsigned long color = GetCortBoxColor(RGB(0, 0, 0x20));

	swPlay = 1;

	// Reset stuff
	gCounter = 0;
	grcGame.left = 0;
#if WINDOW_WIDTH != 320 || WINDOW_HEIGHT != 240
	// Non-vanilla: these three lines are widescreen-related
	grcGame.top = 0;
	grcGame.right = WINDOW_WIDTH;
	grcGame.bottom = WINDOW_HEIGHT;
#endif
	g_GameFlags = 3;

	// Initialize everything
	InitMyChar();
	InitNpChar();
	InitBullet();
	InitCaret();
	InitStar();
	InitFade();
	InitFlash();
	ClearArmsData();
	ClearItemData();
	ClearPermitStage();
	StartMapping();
	InitFlags();
	InitBossLife();

	InitConsole();

	if (gCursorPos == 1)
	{
		if (!LoadProfile(NULL) && !InitializeGame())
			return 0;
	}
	else if (gCursorPos == 0)
	{
		if (!InitializeGame())
			return 0;
	}

	while (1)
	{
		// Get pressed keys
		GetTrg();
		long gKeyTrgMemo = gKeyTrg;
		long gKeyMemo = gKey;
		if(gDebug.cheatVisible)
			gKeyTrg = gKey = 0;

		// Escape menu
		if (gKey & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 1;
			}
		}

		if (swPlay % 2 && g_GameFlags & 1)	// The "swPlay % 2" part is always true
		{
			if (g_GameFlags & 2)
				ActMyChar(TRUE);
			else
				ActMyChar(FALSE);

			ActStar();
			ActNpChar();
			ActBossChar();
			ActValueView();
			ActBack();
			ResetMyCharFlag();
			HitMyCharMap();
			HitMyCharNpChar();
			HitMyCharBoss();
			HitNpCharMap();
			HitBossMap();
			HitBulletMap();
			HitNpCharBullet();
			HitBossBullet();
			if (g_GameFlags & 2)
				ShootBullet();
			ActBullet();
			ActCaret();
			MoveFrame3();
#ifdef FIX_BUGS
			// ActFlash uses frame_x and frame_y uninitialised
			GetFramePosition(&frame_x, &frame_y);
#endif
			ActFlash(frame_x, frame_y);

			if (g_GameFlags & 2)
				AnimationMyChar(TRUE);
			else
				AnimationMyChar(FALSE);
		}

		if (g_GameFlags & 8)
		{
			ActionCredit();
			ActionIllust();
			ActionStripper();
		}

		ProcFade();
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

		if (!(g_GameFlags & 4))
		{
			// Open inventory
			if (gKeyTrg & gKeyItem)
			{
				BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

				switch (CampLoop())
				{
					case enum_ESCRETURN_exit:
						return 0;

					case enum_ESCRETURN_restart:
						return 1;
				}

				gMC.cond &= ~1;
			}
			else if (gMC.equip & 2 && gKeyTrg & gKeyMap)
			{
				BackupSurface(SURFACE_ID_SCREEN_GRAB, &grcGame);

				switch (MiniMapLoop())
				{
					case enum_ESCRETURN_exit:
						return 0;

					case enum_ESCRETURN_restart:
						return 1;
				}
			}
		}

		if (g_GameFlags & 2)
		{
			if (gKeyTrg & gKeyArms)
				RotationArms();
			else if (gKeyTrg & gKeyArmsRev)
				RotationArmsRev();
		}

		if (swPlay % 2)	// This is always true
		{
			switch (TextScriptProc())
			{
				case enum_ESCRETURN_exit:
					return 0;

				case enum_ESCRETURN_restart:
					return 1;
			}
		}

		PutMapName(FALSE);
		PutTimeCounter(16, 8);

		if (g_GameFlags & 2)
		{
			PutMyLife(TRUE);
			PutArmsEnergy(TRUE);
			PutMyAir((WINDOW_WIDTH / 2) - 40, (WINDOW_HEIGHT / 2) - 16);
			PutActiveArmsList();
		}

		if (g_GameFlags & 8)
		{
			PutIllust();
			PutStripper();
		}

		PutTextScript();

		PutFramePerSecound();

		if(gDebug.cheatVisible)
		{
			gKeyTrg = gKeyTrgMemo;
			gKey = gKeyMemo;
		}
			
		PutConsole();




		if (!Flip_SystemTask())
			return 0;

		++gCounter;
	}

	return 0;
}

BOOL Game(void)
{
	int mode;

	if (!LoadGenericData())
	{
		printf("Error: Couldn't read general purpose files");
		return FALSE;
	}

	//PlaySoundObject(7, -1);

	char path[MAX_PATH];
	sprintf(path, "%s/npc.tbl", gDataPath);

	if (!LoadNpcTable(path))
	{
		printf("Error: Couldn't read the NPC table");
	}

	InitTextScript2();
	InitSkipFlags();
	InitMapData2();
	InitCreditScript();

	mode = 1;
	while (mode)
	{
		if (mode == 1)
			mode = ModeOpening();
		if (mode == 2)
			mode = ModeTitle();
		if (mode == 3)
			mode = ModeAction();
	}

	printf("bye i guess");

	//PlaySoundObject(7, 0);

	EndMapData();
	EndTextScript();
	ReleaseNpcTable();
	ReleaseCreditScript();

	return TRUE;
}
