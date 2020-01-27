#include "Main.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BOOL BOOL_OGC
#include <fat.h>
#undef BOOL

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Config.h"
#include "Draw.h"
#include "File.h"
#include "Game.h"
#include "Generic.h"
#include "Input.h"
#include "KeyControl.h"
#include "MyChar.h"
#include "Organya.h"
#include "Profile.h"
#include "Resource.h"
#include "Sound.h"
#include "Triangle.h"

#include "nds.h"
#include <filesystem.h>

char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

BOOL bFullscreen;
BOOL gbUseJoystick = FALSE;

int gJoystickButtonTable[8];

static BOOL bActive = TRUE;
static BOOL bFps = FALSE;

static int windowWidth;
static int windowHeight;

#ifdef JAPANESE
static const char *lpWindowName = "洞窟物語";	// "Cave Story"
#else
static const char *lpWindowName = "Cave Story ~ Doukutsu Monogatari";
#endif


//Framerate stuff
void PutFramePerSecound()
{
	if (bFps)
		PutNumber4(WINDOW_WIDTH - 40, 8, GetFramePerSecound(), false);
}

unsigned long GetFramePerSecound(void)
{
	return 60;
}

int main(int argc, char *argv[])
{
	//Get executable's path
	nitroFSInit(&argv[0]);
	
	consoleDemoInit();
	printf("hi from the world of 2morrow\n");
	
	strcpy(gModulePath, "/");
	
	//Get path of the data folder
	strcpy(gDataPath, "nitro:/data");
	
	//Load configuration
	CONFIG conf;
	
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);
	
	//Apply keybinds
	//Swap X and Z buttons
	if (conf.attack_button_mode)
	{
		if (conf.attack_button_mode == 1)
		{
			gKeyJump = CEY_X;
			gKeyShot = CEY_Z;
		}
	}
	else
	{
		gKeyJump = CEY_Z;
		gKeyShot = CEY_X;
	}
	
	//Swap Okay and Cancel buttons
	if (conf.ok_button_mode)
	{
		if (conf.ok_button_mode == 1)
		{
			gKeyOk = gKeyShot;
			gKeyCancel = gKeyJump;
		}
	}
	else
	{
		gKeyOk = gKeyJump;
		gKeyCancel = gKeyShot;
	}
	
	//Swap left and right weapon switch keys
	if (CheckFileExists("s_reverse"))
	{
		gKeyArms = CEY_ARMSREV;
		gKeyArmsRev = CEY_ARMS;
	}
	
	//Alternate movement keys
	if (conf.move_button_mode)
	{
		if (conf.move_button_mode == 1)
		{
			gKeyLeft = CEY_ALT_LEFT;
			gKeyUp = CEY_ALT_UP;
			gKeyRight = CEY_ALT_RIGHT;
			gKeyDown = CEY_ALT_DOWN;
		}
	}
	else
	{
		gKeyLeft = CEY_LEFT;
		gKeyUp = CEY_UP;
		gKeyRight = CEY_RIGHT;
		gKeyDown = CEY_DOWN;
	}

	//Initialize rendering
	StartDirectDraw();
	
	//Initialize input
	InitDirectInput();
	
	//Check debug things
	if (CheckFileExists("fps"))
		bFps = true;

	
	//Set rects
	RECT loading_rect = {0, 0, 64, 8};
	RECT clip_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	
	
	//Load the "LOADING" text
	MakeSurface_File("Loading", SURFACE_ID_LOADING);
	
	printf("hi from the world of 3morrow\n");
	
	//Draw loading screen
	CortBox(&clip_rect, 0x000000);
	printf("hi from the world of 3morrow\n");
	PutBitmap3(&clip_rect, (WINDOW_WIDTH - 64) / 2, (WINDOW_HEIGHT - 8) / 2, &loading_rect, SURFACE_ID_LOADING);
	
	printf("hi from the world of 4morrow\n");
	
	//Draw to screen
	if (Flip_SystemTask())
	{
		printf("hi from the world of 5morrow\n");
		//Initialize sound
		InitDirectSound();
		
		//Initialize stuff
		InitTextObject();
		InitTriangleTable();
		printf("hi from the world of 6morrow\n");
		//Run game code
		Game();
		printf("bye i guess");
		while (true){}
		//End stuff
		EndDirectSound();
		EndTextObject();
		EndDirectDraw();
	}
	
	return 0;
}

void InactiveWindow(void)
{
	if (bActive)
	{
		bActive = FALSE;
		StopOrganyaMusic();
		SleepNoise();
	}

	PlaySoundObject(7, 0);
}

void ActiveWindow(void)
{
	if (!bActive)
	{
		bActive = TRUE;
		StopOrganyaMusic();
		PlayOrganyaMusic();
		ResetNoise();
	}

	PlaySoundObject(7, -1);
}

void JoystickProc(void);

bool SystemTask()
{
	return true;
}

void JoystickProc(void)
{/*
	int i;
	JOYSTICK_STATUS status;

	if (!GetJoystickStatus(&status))
		return;

	gKey &= (CEY_ESCAPE | CEY_F2 | CEY_F1);

	// Set movement buttons
	if (status.bLeft)
		gKey |= gKeyLeft;
	else
		gKey &= ~gKeyLeft;

	if (status.bRight)
		gKey |= gKeyRight;
	else
		gKey &= ~gKeyRight;

	if (status.bUp)
		gKey |= gKeyUp;
	else
		gKey &= ~gKeyUp;

	if (status.bDown)
		gKey |= gKeyDown;
	else
		gKey &= ~gKeyDown;

	// Clear held buttons
	for (i = 0; i < 8; ++i)
		gKey &= ~gJoystickButtonTable[i];

	// Set held buttons
	for (i = 0; i < 8; ++i)
		if (status.bButton[i])
			gKey |= gJoystickButtonTable[i];*/
}
