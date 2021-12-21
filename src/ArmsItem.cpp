// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "ArmsItem.h"

#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Escape.h"
#include "Game.h"
#include "KeyControl.h"
#include "Main.h"
#include "Shoot.h"
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


ARMS gArmsDataP1[ARMS_MAX];
ARMS* gArmsData = gArmsDataP1;
ARMS gArmsDataP2[ARMS_MAX];

ITEM gItemData[ITEM_MAX];

int gSelectedArms;
int gSelectedArmsP1;
int gSelectedArmsP2;
int gSelectedItem;

static int gCampTitleY;

int gRowSize = 7;

/// True if we're in the items section of the inventory (not in the weapons section) (only relevant when the inventory is open)
static BOOL gCampActive;

int gArmsEnergyX = 16;
int gArmsEnergyXP2 = 16;

bool gInCamp = false;

void ClearArmsData(void)
{
#ifdef FIX_BUGS
	gSelectedArms = 0; // Should probably be done in order to avoid potential problems with the selected weapon being invalid (like is done in SubArmsData)
	gSelectedArmsP2 = 0;
#endif
	gArmsEnergyX = 32;
	gArmsEnergyXP2 = 32;	
	
	memset(gArmsDataP1, 0, sizeof(gArmsDataP1));
	memset(gArmsDataP2, 0, sizeof(gArmsDataP2));
}

void ClearItemData(void)
{
	memset(gItemData, 0, sizeof(gItemData));
}

BOOL AddArmsData(long code, long max_num)
{
	// Search for code
	int i = 0;
	while (i < ARMS_MAX)
	{
		if (gArmsData[i].code == code)
			break;	// Found identical

		if (gArmsData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	if (i == ARMS_MAX)
		return FALSE;	// No space left

	if (gArmsData[i].code == 0)
	{
		// Initialize new weapon
		memset(&gArmsData[i], 0, sizeof(ARMS));
		gArmsData[i].level = 1;
	}

	// Set weapon and ammo
	gArmsData[i].code = code;
	gArmsData[i].max_num += max_num;
	gArmsData[i].num += max_num;

	// Cap the amount of current ammo to the maximum amount of ammo
	if (gArmsData[i].num > gArmsData[i].max_num)
		gArmsData[i].num = gArmsData[i].max_num;

	if (gArmsDataP2[i].code == 0)
	{
		// Initialize new weapon
		memset(&gArmsDataP2[i], 0, sizeof(ARMS));
		gArmsDataP2[i].level = 1;
	}

	// Set weapon and ammo
	gArmsDataP2[i].code = code;
	gArmsDataP2[i].max_num += max_num;
	gArmsDataP2[i].num += max_num;

	// Cap the amount of current ammo to the maximum amount of ammo
	if (gArmsDataP2[i].num > gArmsDataP2[i].max_num)
		gArmsDataP2[i].num = gArmsDataP2[i].max_num;

	return TRUE;
}

BOOL SubArmsData(long code)
{
	// Search for code
	int i;
	for (i = 0; i < ARMS_MAX; ++i)
		if (gArmsData[i].code == code)
			break;	// Found

#ifdef FIX_BUGS
	if (i == ARMS_MAX)
#else
	if (i == ITEM_MAX)	// Wrong
#endif
		return FALSE;	// Not found

	int old_i = i;
	// Shift all arms from the right to the left
	for (++i; i < ARMS_MAX; ++i)
		gArmsData[i - 1] = gArmsData[i];

	// Clear farthest weapon and select first
	gArmsData[i - 1].code = 0;
	gSelectedArms = 0;

	for (++old_i; old_i < ARMS_MAX; ++old_i)
		gArmsDataP2[old_i - 1] = gArmsDataP2[old_i];

	// Clear farthest weapon and select first
	gArmsDataP2[old_i - 1].code = 0;
	gSelectedArmsP2= 0;

	return TRUE;
}

BOOL TradeArms(long code1, long code2, long max_num)
{
	// Search for code1
	int i = 0;
	while (i < ARMS_MAX)
	{
		if (gArmsData[i].code == code1)
			break;	// Found identical

		++i;
	}

	if (i == ARMS_MAX)
		return FALSE;	// Not found

	// Initialize new weapon replacing old one, but adding the maximum ammunition to that of the old weapon.
	gArmsData[i].level = 1;
	gArmsData[i].code = code2;
	gArmsData[i].max_num += max_num;
	gArmsData[i].num += max_num;
	gArmsData[i].exp = 0;

	gArmsDataP2[i].level = 1;
	gArmsDataP2[i].code = code2;
	gArmsDataP2[i].max_num += max_num;
	gArmsDataP2[i].num += max_num;
	gArmsDataP2[i].exp = 0;

	return TRUE;
}

BOOL AddItemData(long code)
{
	// Search for code
	int i = 0;
	while (i < ITEM_MAX)
	{
		if (gItemData[i].code == code)
			break;	// Found identical

		if (gItemData[i].code == 0)
			break;	// Found free slot

		++i;
	}

	if (i == ITEM_MAX)
		return FALSE;	// Not found

	gItemData[i].code = code;

	return TRUE;
}

BOOL SubItemData(long code)
{
	// Search for code
	int i;
	for (i = 0; i < ITEM_MAX; ++i)
		if (gItemData[i].code == code)
			break;	// Found

	if (i == ITEM_MAX)
		return FALSE;	// Not found

	// Shift all items from the right to the left
	for (++i; i < ITEM_MAX; ++i)
		gItemData[i - 1] = gItemData[i];

	gItemData[i - 1].code = 0;
	gSelectedItem = 0;

	return TRUE;
}

/// Update the inventory cursor
void MoveCampCursor(void)
{
	BOOL bChange;

	// Compute the current amount of weapons and items
	int arms_num = 0;
	int item_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;
	while (gItemData[item_num].code != 0)
		++item_num;

	if (arms_num == 0 && item_num == 0)
		return;	// Empty inventory

	// True if we're currently changing cursor position
	bChange = FALSE;

	if (!gCampActive)
	{
		// Handle selected weapon
		if ((gKeyTrg | gKeyTrgP2) & gKeyLeft)
		{
			if(gTextScriptInitiator == 0)
				--gSelectedArms;
			else
				--gSelectedArmsP2;
			
			bChange = TRUE;
		}

		if ((gKeyTrg | gKeyTrgP2) & gKeyRight)
		{
			if(gTextScriptInitiator == 0)
				++gSelectedArms;
			else
				++gSelectedArmsP2;
			bChange = TRUE;
		}

		if ((gKeyTrg | gKeyTrgP2) & (gKeyUp | gKeyDown))
		{
			// If there are any items, we're changing to the items section, since the weapons section has only 1 row
			if (item_num != 0)
				gCampActive = TRUE;

			bChange = TRUE;
		}

		// Loop around gSelectedArms if needed

		if(gTextScriptInitiator == 0)
		{
			if (gSelectedArms < 0)
				gSelectedArms = arms_num - 1;

			if (gSelectedArms > arms_num - 1)
				gSelectedArms = 0;
		}
		else
		{
			if (gSelectedArmsP2 < 0)
				gSelectedArmsP2 = arms_num - 1;

			if (gSelectedArmsP2 > arms_num - 1)
				gSelectedArmsP2 = 0;
		}
		
	}
	else
	{
		// Handle selected item
		if ((gKeyTrg | gKeyTrgP2) & gKeyLeft)
		{
			if (gSelectedItem % gRowSize == 0)
				gSelectedItem += gRowSize - 1;
			else
				gSelectedItem -= 1;

			bChange = TRUE;
		}

		if ((gKeyTrg | gKeyTrgP2) & gKeyRight)
		{
			if (gSelectedItem == item_num - 1)
				gSelectedItem = (gSelectedItem / gRowSize) * gRowSize;	// Round down to multiple of 6
			else if (gSelectedItem % gRowSize == (gRowSize - 1))
				gSelectedItem -= gRowSize - 1;	// Loop around row
			else
				gSelectedItem += 1;

			bChange = TRUE;
		}

		if ((gKeyTrg | gKeyTrgP2) & gKeyUp)
		{
			if (gSelectedItem / gRowSize == 0)
				gCampActive = FALSE;	// We're on the first row, transition to weapons
			else
				gSelectedItem -= gRowSize;

			bChange = TRUE;
		}

		if ((gKeyTrg | gKeyTrgP2) & gKeyDown)
		{
			if (gSelectedItem / gRowSize == (item_num - 1) / gRowSize)
				gCampActive = FALSE;	// We're on the last row, transition to weapons
			else
				gSelectedItem += gRowSize;

			bChange = TRUE;
		}

		if (gSelectedItem >= item_num)
			gSelectedItem = item_num - 1;	// Don't allow selecting a non-existing item

		if (gCampActive && (gKeyTrg | gKeyTrgP2) & gKeyOk)
			StartTextScript(6000 + gItemData[gSelectedItem].code, gTextScriptInitiator);
	}

	if (bChange)
	{
		if (gCampActive == FALSE)
		{
			// Switch to a weapon
			PlaySoundObject(SND_SWITCH_WEAPON, 1);

			if (arms_num != 0)
			{
				if(gTextScriptInitiator == 0)
					StartTextScript(1000 + gArmsData[gSelectedArms].code, gTextScriptInitiator);
				else
					StartTextScript(1000 + gArmsData[gSelectedArmsP2].code, gTextScriptInitiator);
			}
			else
				StartTextScript(1000, gTextScriptInitiator);
		}
		else
		{
			// Switch to an item
			PlaySoundObject(SND_YES_NO_CHANGE_CHOICE, 1);

			if (item_num != 0)
				StartTextScript(5000 + gItemData[gSelectedItem].code, gTextScriptInitiator);
			else
				StartTextScript(5000, gTextScriptInitiator);
		}
	}
}
	int height2 = (WINDOW_HEIGHT / 2) - 106;
	int height1 = (WINDOW_HEIGHT / 2) - 96;
	int height8 = (WINDOW_HEIGHT / 2) - 90;
	int height3 = (WINDOW_HEIGHT / 2) - 82;
	int height5 = (WINDOW_HEIGHT / 2) - 66;
	int height6 = (WINDOW_HEIGHT / 2) - 58;
	int height4 = (WINDOW_HEIGHT / 2) - 50;
	int height7 = (WINDOW_HEIGHT / 2) - 30;

/// Draw the inventory
void PutCampObject(void)
{
	static unsigned int flash;

	int i;

	/// Rect for the current weapon
	RECT rcArms;

	/// Rect for the current item
	RECT rcItem;

	/// Probably the rect for the slash
	RECT rcPer = {72, 48, 80, 56};

	/// Rect for when there is no ammo (double dashes)
	RECT rcNone = {80, 48, 96, 56};

	/// Rect for the "Lv" text!
	RECT rcLv = {80, 80, 96, 88};

	/// Final rect drawn on the screen
	RECT rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	/// Cursor rect array for weapons, element [1] being for when the cursor is flashing
	RECT rcCur1[2] = {{0, 88, 40, 128}, {40, 88, 80, 128}};

	/// Cursor rect array for items, element [1] being for when the cursor is flashing
	RECT rcCur2[2] = {{80, 88, 112, 104}, {80, 104, 112, 120}};

	RECT rcTitle1 = {80, 48, 144, 56};
	RECT rcTitle2 = {80, 56, 144, 64};
	RECT rcBoxTop = {0, 0, 244, 8};
	RECT rcBoxBody = {0, 8, 244, 16};
	RECT rcBoxBottom = {0, 16, 244, 24};

	u32 alpha = 27;
	// Draw box
	if(gbTransparency)
	{
		PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, height1, &rcBoxTop, SURFACE_ID_TEXT_BOX);
		for (i = 1; i < 18; ++i)
			PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, (height2) + ((i + 1) * 8), &rcBoxBody, SURFACE_ID_TEXT_BOX);
		//PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 122, (height2) + ((i + 1) * 8), &rcBoxBottom, SURFACE_ID_TEXT_BOX);
	}
	else
	{
		PutBitmap3Transparent(&rcView, (WINDOW_WIDTH / 2) - 122, height1, &rcBoxTop, SURFACE_ID_TEXT_BOX, alpha);
		for (i = 1; i < 18; ++i)
			PutBitmap3Transparent(&rcView, (WINDOW_WIDTH / 2) - 122, (height2) + ((i + 1) * 8), &rcBoxBody, SURFACE_ID_TEXT_BOX, alpha);
		//PutBitmap3Transparent(&rcView, (WINDOW_WIDTH / 2) - 122, (height2) + ((i + 1) * 8), &rcBoxBottom, SURFACE_ID_TEXT_BOX, alpha);
	}
	

	// Move titles
	if (gCampTitleY > height8)
		--gCampTitleY;

	// Draw titles
	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 112, gCampTitleY, &rcTitle1, SURFACE_ID_TEXT_BOX);
	PutBitmap3(&rcView, (WINDOW_WIDTH / 2) - 112, gCampTitleY + 52, &rcTitle2, SURFACE_ID_TEXT_BOX);

	// Draw arms cursor
	++flash;
	if(gTextScriptInitiator == 0)
	{
		if (gCampActive == FALSE)
			PutBitmap3(&rcView, (gSelectedArms * 40) + (WINDOW_WIDTH / 2) - 112, height3, &rcCur1[(flash / 2) % 2], SURFACE_ID_TEXT_BOX);
		else	
			PutBitmap3(&rcView, (gSelectedArms * 40) + (WINDOW_WIDTH / 2) - 112, height3, &rcCur1[1], SURFACE_ID_TEXT_BOX);
	}
	else
	{
		if (gCampActive == FALSE)
			PutBitmap3(&rcView, (gSelectedArmsP2 * 40) + (WINDOW_WIDTH / 2) - 112, height3, &rcCur1[(flash / 2) % 2], SURFACE_ID_TEXT_BOX);
		else
			PutBitmap3(&rcView, (gSelectedArmsP2 * 40) + (WINDOW_WIDTH / 2) - 112, height3, &rcCur1[1], SURFACE_ID_TEXT_BOX);
	}
	
	// Draw weapons
	for (i = 0; i < ARMS_MAX; ++i)
	{
		if (gArmsData[i].code == 0)
			break;	// Invalid weapon

		// Get icon rect for next weapon
		rcArms.left = (gArmsData[i].code % 16) * 16;
		rcArms.right = rcArms.left + 16;
		rcArms.top = ((gArmsData[i].code) / 16) * 16;
		rcArms.bottom = rcArms.top + 16;

		// Draw the icon, slash and "Lv"
		PutBitmap3(&rcView, (i * 40) + (WINDOW_WIDTH / 2) - 112, height3, &rcArms, SURFACE_ID_ARMS_IMAGE);
		PutBitmap3(&rcView, (i * 40) + (WINDOW_WIDTH / 2) - 112, height4, &rcPer, SURFACE_ID_TEXT_BOX);
		PutBitmap3(&rcView, (i * 40) + (WINDOW_WIDTH / 2) - 112,height5, &rcLv, SURFACE_ID_TEXT_BOX);
		PutNumber4((i * 40) + (WINDOW_WIDTH / 2) - 112,height5, gArmsData[i].level, FALSE);

		// Draw ammo
		if (gArmsData[i].max_num)
		{
			PutNumber4((i * 40) + (WINDOW_WIDTH / 2) - 112, height6, gArmsData[i].num, FALSE);
			PutNumber4((i * 40) + (WINDOW_WIDTH / 2) - 112, height4, gArmsData[i].max_num, FALSE);
		}
		else
		{
			// Weapon doesn't use ammunition
			PutBitmap3(&rcView, (i * 40) + (WINDOW_WIDTH - 192) / 2, height6, &rcNone, SURFACE_ID_TEXT_BOX);
			PutBitmap3(&rcView, (i * 40) + (WINDOW_WIDTH - 192) / 2, height4, &rcNone, SURFACE_ID_TEXT_BOX);
		}
	}

	// Draw items cursor
	if (gCampActive == TRUE)
		PutBitmap3(&rcView, ((gSelectedItem % gRowSize) * 32) + (WINDOW_WIDTH / 2) - 112, ((gSelectedItem / gRowSize) * 16) + height7, &rcCur2[flash / 2 % 2], SURFACE_ID_TEXT_BOX);
	else
		PutBitmap3(&rcView, ((gSelectedItem % gRowSize) * 32) + (WINDOW_WIDTH / 2) - 112, ((gSelectedItem / gRowSize) * 16) + height7, &rcCur2[1], SURFACE_ID_TEXT_BOX);

	for (i = 0; i < ITEM_MAX; ++i)
	{
		if (gItemData[i].code == 0)
			break;	// Invalid item

		// Get rect for next item
		rcItem.left = (gItemData[i].code % 8) * 32;
		rcItem.right = rcItem.left + 32;
		rcItem.top = (gItemData[i].code / 8) * 16;
		rcItem.bottom = rcItem.top + 16;

		PutBitmap3(&rcView, ((i % gRowSize) * 32) + (WINDOW_WIDTH / 2) - 112, ((i / gRowSize) * 16) + height7, &rcItem, SURFACE_ID_ITEM_IMAGE);
	}
}

int CampLoop(void)
{
	char old_script_path[MAX_PATH];

	RECT rcView = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

	// Save the current script path (to restore it when we get out of the inventory)
	GetTextScriptPath(old_script_path);

	// Load the inventory script
	LoadTextScript2("ArmsItem.tsc");

	gCampTitleY = height3;

	// Put the cursor on the first weapon
	gCampActive = FALSE;
	gSelectedItem = 0;

	if(gTextScriptInitiator != 0)
		gSelectedArms = gSelectedArmsP2;

	// Compute current amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num != 0)
		StartTextScript(1000 + gArmsData[gSelectedArms].code,gTextScriptInitiator);
	else
		StartTextScript(5000 + gItemData[gSelectedItem].code,gTextScriptInitiator);

	for (;;)
	{
		GetTrg();

		// Handle ESC
		if ((gKeyTrg | gKeyTrgP2) & CEY_ESCAPE)
		{
			switch (Call_Escape())
			{
				case enum_ESCRETURN_exit:
					return enum_ESCRETURN_exit;	// Quit game

				case enum_ESCRETURN_restart:
					return enum_ESCRETURN_restart;	// Go to game intro
			}
		}

		if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED)
			MoveCampCursor();

		switch (TextScriptProc())
		{
			case enum_ESCRETURN_exit:
				return enum_ESCRETURN_exit;	// Quit game

			case enum_ESCRETURN_restart:
				return enum_ESCRETURN_restart;	// Go to game intro
		}

		// Get currently displayed image
		//PutBitmap4(&rcView, 0, 0, &rcView, SURFACE_ID_SCREEN_GRAB);
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

		PutCampObject();
		PutTextScript();
		PutFramePerSecound();

		// Check whether we're getting out of the loop
		if (gCampActive)
		{
			if (g_GameFlags & GAME_FLAG_IS_CONTROL_ENABLED && (gKeyTrg | gKeyTrgP2) & (gKeyCancel | gKeyItem))
			{
				StopTextScript();
				break;
			}
		}
		else
		{
			if ((gKeyTrg | gKeyTrgP2) & (gKeyOk | gKeyCancel | gKeyItem))
			{
				StopTextScript();
				break;
			}
		}

		if (!Flip_SystemTask())
			return enum_ESCRETURN_exit;	// Quit game

		gCounter++;
	}

	// Resume original script
	LoadTextScript_Stage(old_script_path);
	gArmsEnergyX = 32; // Displays weapon rotation animation in case the weapon was changed
	gArmsEnergyXP2 = 32;
	return enum_ESCRETURN_continue;	// Go to game
}

BOOL CheckItem(long a)
{
	int i;

	for (i = 0; i < ITEM_MAX; ++i)
		if (gItemData[i].code == a)
			return TRUE;	// Found

	return FALSE;	// Not found
}

BOOL CheckArms(long a)
{
	int i;

	for (i = 0; i < ARMS_MAX; ++i)
		if (gArmsData[i].code == a)
			return TRUE;	// Found

	return FALSE;	// Not found
}

BOOL UseArmsEnergy(long num)
{
	if (gArmsData[gSelectedArms].max_num == 0)
		return TRUE;	// No ammo needed
	if (gArmsData[gSelectedArms].num == 0)
		return FALSE;	// No ammo left

	gArmsData[gSelectedArms].num -= num;

	if (gArmsData[gSelectedArms].num < 0)
		gArmsData[gSelectedArms].num = 0;

	return TRUE;	// Was able to spend ammo
}

BOOL ChargeArmsEnergy(long num)
{
	gArmsData[gSelectedArms].num += num;

	// Cap the ammo to the maximum ammunition
	if (gArmsData[gSelectedArms].num > gArmsData[gSelectedArms].max_num)
		gArmsData[gSelectedArms].num = gArmsData[gSelectedArms].max_num;

	return TRUE;	// Always successfull
}

void FullArmsEnergy(void)
{
	int a;

	for (a = 0; a < ARMS_MAX; ++a)
	{
		if (gArmsData[a].code == 0)
			continue;	// Don't change empty weapons

		gArmsData[a].num = gArmsData[a].max_num;
	}
}

int RotationArms(void)
{
	// Get amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num == 0)
		return 0;

	ResetSpurCharge();

	// Select next valid weapon
	++gSelectedArms;

	while (gSelectedArms < arms_num)
	{
		if (gArmsData[gSelectedArms].code)
			break;

		++gSelectedArms;
	}

	if (gSelectedArms == arms_num)
	{
		gSelectedArms = 0;
	}

		

	if(gCurMyChar == 0)
		gArmsEnergyX = 32;
	else
		gArmsEnergyXP2 = 32;
	
	
	PlaySoundObject(SND_SWITCH_WEAPON, 1);

	return gArmsData[gSelectedArms].code;
}

int RotationArmsRev(void)
{
	// Get amount of weapons
	int arms_num = 0;
	while (gArmsData[arms_num].code != 0)
		++arms_num;

	if (arms_num == 0)
		return 0;

	ResetSpurCharge();

	// Select previous valid weapon
	--gSelectedArms;
	

	if (gSelectedArms < 0)
	{	
		gSelectedArms = arms_num - 1;
	}


	while (gSelectedArms < arms_num)
	{
		if (gArmsData[gSelectedArms].code)
			break;

			// Select previous valid weapon
		--gSelectedArms;
	}

	if(gCurMyChar == 0)
		gArmsEnergyX = 0;
	else
		gArmsEnergyXP2 = 0;
	
	PlaySoundObject(SND_SWITCH_WEAPON, 1);

	return gArmsData[gSelectedArms].code;
}

void ChangeToFirstArms(void)
{
	gSelectedArms = 0;
	gArmsEnergyX = 32;
	PlaySoundObject(SND_SWITCH_WEAPON, 1);
}
