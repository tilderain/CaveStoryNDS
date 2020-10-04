#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Input.h"
#include "KeyControl.h"
#include "CommonDefines.h"
#include "Tags.h"

#include "nds.h"

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

bool UpdateInput()
{
	
	scanKeys();

	int keys = keysHeld();
	gKey = 0;
	
	gKey |= (keys & KEY_UP) ? gKeyUp : 0 ;
	gKey |= (keys & KEY_RIGHT) ? gKeyRight : 0 ;
	gKey |= (keys & KEY_DOWN) ? gKeyDown : 0 ;
	gKey |= (keys & KEY_LEFT) ? gKeyLeft : 0 ;
	
	gKey |= (keys & KEY_A) ? gKeyJump : 0 ;
	gKey |= (keys & KEY_B) ? gKeyShot : 0 ;
	gKey |= (keys & KEY_A) ? gKeyOk : 0 ;
	gKey |= (keys & KEY_B) ? gKeyCancel : 0 ;
	gKey |= (keys & KEY_START) ? gKeyItem : 0 ;
	gKey |= (keys & KEY_SELECT) ? gKeyMap : 0 ;
	
	gKey |= (keys & KEY_X) ? gKeyArms : 0 ;
	gKey |= (keys & KEY_Y) ? gKeyArmsRev : 0 ;

	if(keys & KEY_L && keys & KEY_R && keys & KEY_START)
	{
		gKey |= CEY_ESCAPE;
	}
	return true;
}