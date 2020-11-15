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
#include "Sound.h"
#include "Triangle.h"

#include "nds.h"
#include <filesystem.h>
#include "../srccommon/csFifo.h"
#include "nds/fifocommon.h"

#ifdef CYG_PROFILER
#define __cplusplus
#include "cyg-profile.h"
#undef __cplusplus
#endif

char gModulePath[MAX_PATH];
char gDataPath[MAX_PATH];

BOOL bFullscreen;
BOOL gbUseJoystick = FALSE;

int gJoystickButtonTable[8];

static BOOL bActive = TRUE;
static BOOL bFps = FALSE;

static int windowWidth;
static int windowHeight;

CONFIG conf;
CONFIG_BINDING bindings[BINDING_TOTAL];

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

void fifoDataHandler(int bytes, void *user_data) 
{
	CSFIFOMessage msg;
	int val;

	val = fifoGetDatamsg(FIFO_USER_01, bytes, (u8*)&msg);

	//printf("help");

	if(msg.type == CSFIFO_ARM7_PRINT)
	{
		printf(msg.printData);
		//printf("%p\n", msg.printData);
	}
}
 int hblanks = 0;
#ifdef CYG_PROFILER

 static void hblankCount (void)
  __attribute__ ((no_instrument_function));

 void hblankCount(){
  hblanks++;
 }
#endif

int main(int argc, char *argv[])
{
	defaultExceptionHandler();
	
	//Get executable's path
	fatInitDefault();

	printf("hi from the world of 2morrow\n");

#ifdef CYG_PROFILER
 irqEnable(IRQ_HBLANK); 
 irqSet(IRQ_HBLANK, hblankCount);
 cygprofile_begin();
 cygprofile_enable();
#endif

	//irqEnable(IRQ_FIFO_NOT_EMPTY);
	//fifoInit();
	
	strcpy(gModulePath, "/");
	
	//Get path of the data folder
	strcpy(gDataPath, "data");
	
	//Load configuration
	
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);
	

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

	// Bandaid fix for weird, probably undefined behavior when loading on hardware
	MakeSurface_Generic(64, 8, SURFACE_ID_LOADING);
	ReloadBitmap_File("Loading", SURFACE_ID_LOADING);
	
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

	//PlaySoundObject(7, 0);
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

	//PlaySoundObject(7, -1);
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
