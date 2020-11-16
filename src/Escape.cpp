// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Escape.h"

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "KeyControl.h"
#include "Main.h"

#include "Sound.h"
#include "nds.h"

#include "Pause.h"

int Call_Escape(void)
{
	RECT rc = {0, 128, 208, 144};

	int cursorPos = 1;

	while (1)
	{
		// Get pressed keys
		GetTrg();

		/*if (gKeyTrg & CEY_ESCAPE) // Escape is pressed, quit game
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_exit;
		}*/
		if (gKeyTrg & CEY_F1) // F1 is pressed, continue
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_continue;
		}
		if (gKeyTrg & CEY_F2) // F2 is pressed, reset
		{
			gKeyTrg = 0;
			return enum_ESCRETURN_restart;
		}

		if (gKeyTrg & gKeyLeft)
		{
			cursorPos--; PlaySoundObject(1, 1);
			if(cursorPos<0) cursorPos = 2;
		}

		if (gKeyTrg & gKeyRight)
		{
			cursorPos++; PlaySoundObject(1, 1);
			if(cursorPos>2) cursorPos = 0;
		}

		if (gKeyTrg & gKeyUp)
		{
			cursorPos = 1; PlaySoundObject(1, 1);
		}

		if (gKeyTrg & gKeyDown)
		{
			cursorPos = 3; PlaySoundObject(1, 1);
		}

		if (gKeyTrg & gKeyJump)
		{
			GetTrg();
			switch (cursorPos)
			{
				case 0: return enum_ESCRETURN_exit;
				case 1: return enum_ESCRETURN_continue;
				case 2: return enum_ESCRETURN_restart;
				case 3: Call_Pause(); return enum_ESCRETURN_continue;
			}
		}

		// Draw screen
		CortBox(&grcFull, 0x000000);
		PutBitmap3(&grcFull, (WINDOW_WIDTH / 2) - 104, (WINDOW_HEIGHT / 2) - 8, &rc, SURFACE_ID_TEXT_BOX);

		RECT rect_cur = {112, 88, 128, 104};
		int x = 0;
		int y = WINDOW_HEIGHT / 2 - 8;
		switch (cursorPos)
		{
			case 0: x = 40;break;
			case 1: x = 96;break;
			case 2: x = 180;break;
			case 3: x = 96; y = 150; break;
		}

		PutText(&grcFull, 115, 152, "Config", RGB(160, 181, 222));
		PutBitmap3(&grcFull, x, y, &rect_cur, SURFACE_ID_TEXT_BOX);

		PutFramePerSecound();

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			gKeyTrg = 0;
			return enum_ESCRETURN_exit;
		}
	}

	return enum_ESCRETURN_exit;
}
