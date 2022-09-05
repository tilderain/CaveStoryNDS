// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Input.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Input.h"
#include "KeyControl.h"
#include "CommonDefines.h"
#include "Tags.h"

#include "Main.h"

#include "nds.h"

#include "nifi.h"

#include "Draw.h"

#define STICK_DEADZONE 0x20
#define STICK_DEADSML  -STICK_DEADZONE
#define STICK_DEADBIG   STICK_DEADZONE

void ReleaseDirectInput()
{
	//WPAD_Shutdown();
}

bool InitDirectInput()
{
	//WPAD_Init();
	//PAD_Init();
	return true;
}

int gDisconnectTimer = 0;

int gConsoleTimer = 0;

bool UpdateInput()
{
	
	scanKeys();

	int keys = keysHeld();
	gKey = 0;
	
	gKey |= (keys & bindings[BINDING_UP].keyboard) ? gKeyUp : 0 ;
	gKey |= (keys & bindings[BINDING_RIGHT].keyboard) ? gKeyRight : 0 ;
	gKey |= (keys & bindings[BINDING_DOWN].keyboard) ? gKeyDown : 0 ;
	gKey |= (keys & bindings[BINDING_LEFT].keyboard) ? gKeyLeft : 0 ;
	
	gKey |= (keys & bindings[BINDING_JUMP].keyboard) ? gKeyJump : 0 ;
	gKey |= (keys & bindings[BINDING_SHOT].keyboard) ? gKeyShot : 0 ;
	gKey |= (keys & bindings[BINDING_JUMP].keyboard) ? gKeyOk : 0 ;
	gKey |= (keys & bindings[BINDING_SHOT].keyboard) ? gKeyCancel : 0 ;
	gKey |= (keys & bindings[BINDING_ITEM].keyboard) ? gKeyItem : 0 ;
	gKey |= (keys & bindings[BINDING_MAP].keyboard) ? gKeyMap : 0 ;
	
	gKey |= (keys & bindings[BINDING_ARMS].keyboard) ? gKeyArms : 0 ;
	gKey |= (keys & bindings[BINDING_ARMSREV].keyboard) ? gKeyArmsRev : 0 ;

	if(keys & KEY_L && keys & KEY_R && keys & KEY_START)
	{
		gKey |= CEY_ESCAPE;
	}

	if (keys & KEY_L && keys & KEY_UP)
		gKey |= CEY_CHEAT;

	if (keys & KEY_R)
		gKey |= CEY_FASTFORWARD;


	if(keys & KEY_L && keys & KEY_R && keys & KEY_SELECT && gConsoleTimer == 0)
	{
		if(gConsoleInited == 1) // disabled bottom screen
		{
			gConsoleInited = 2;
			videoBgEnableSub(0);
		}
		else
		{

			gConsoleInited = 1;
			videoBgDisableSub(0);
		}
		gConsoleTimer = 30;
	}
	if(gConsoleTimer > 0) gConsoleTimer--;

	int status = nifiGetStatus();
	if(status == HOST_INGAME || status == CLIENT_INGAME)
	{
		nifiUpdateInput();
	}

	return true;
}