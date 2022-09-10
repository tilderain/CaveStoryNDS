// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

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
#include "Debug.h"
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
//#include "../srccommon/csFifo.h"
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

BOOL gb50Fps = FALSE;
BOOL gbTransparency = FALSE;
BOOL gbAlternateDrams = FALSE;

int gJoystickButtonTable[8];

static BOOL bActive = TRUE;
static BOOL bFps = FALSE;

static int windowWidth;
static int windowHeight;

bool gIsCardPopped = false;
int gCardPopTimer = 0;

int gLoadingProgress = 0;

int gPxtoneInited = 0;

CONFIG conf;
CONFIG_BINDING bindings[BINDING_TOTAL];

#ifdef JAPANESE
static const char *lpWindowName = "洞窟物語";	// "Cave Story"
#else
static const char *lpWindowName = "Cave Story ~ Doukutsu Monogatari";
#endif

#include "./pxtone/pxtnService.h"
#include "./pxtone/pxtnError.h"

pxtnService*   pxtn     = NULL ;

#define _CHANNEL_NUM           1
#define _SAMPLE_PER_SECOND 11025
#define _BUFFER_PER_SEC    (0.3f)

void Timer_1ms()
{
	int p_req_size = 22*4;
	pxtn->Moo( NULL, p_req_size);
}


bool _load_ptcop( pxtnService* pxtn, const char* path_src, pxtnERR* p_pxtn_err )
{
	bool           b_ret     = false;
	pxtnDescriptor desc;
	pxtnERR        pxtn_err  = pxtnERR_VOID;
	FILE*          fp        = NULL;
	int32_t        event_num =    0;

	if( !( fp = fopen( path_src, "rb" ) ) ) goto term;
	if( !desc.set_file_r( fp ) ) goto term;

	pxtn_err = pxtn->read       ( &desc ); if( pxtn_err != pxtnOK ) goto term;
	pxtn_err = pxtn->tones_ready(       ); if( pxtn_err != pxtnOK ) goto term;

	b_ret = true;
term:

	if( fp     ) fclose( fp );
	if( !b_ret ) pxtn->evels->Release();

	if( p_pxtn_err ) *p_pxtn_err = pxtn_err;

	return b_ret;
}

static bool _sampling_func( void *user, void *buf, int *p_res_size, int *p_req_size )
{
	pxtnService* pxtn = static_cast<pxtnService*>( user );

	if( !pxtn->Moo( buf, *p_req_size ) ) return false;
	if( p_res_size ) *p_res_size = *p_req_size;

	return true;
}




void card_line_irq()
{
	if(!gIsCardPopped)
	{
		printf("card line irq popped\n");
		gIsCardPopped = true;
		gCardPopTimer = 300;
	}
}

void PutLoadingProgress()
{

	RECT loading_rect = {0, 0, 64, 8};
	RECT clip_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	PutBitmap3(&clip_rect, (WINDOW_WIDTH - 64) / 2, (WINDOW_HEIGHT - 8) / 2, &loading_rect, SURFACE_ID_LOADING);

	int baseW = (WINDOW_WIDTH - 64) / 2 - 5;
	int baseH = ((WINDOW_HEIGHT - 8) / 2) + 25;
	
	glBoxFilled(baseW, baseH, baseW + (gLoadingProgress / 1.38), baseH, RGB15(50, 255, 50));

	Flip_SystemTask();
}

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
	/*CSFIFOMessage msg;
	int val;

	val = fifoGetDatamsg(FIFO_USER_01, bytes, (u8*)&msg);

	//printf("help");

	if(msg.type == CSFIFO_ARM7_PRINT)
	{
		printf(msg.printData);
		//printf("%p\n", msg.printData);
	}*/
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
#ifdef NITROFS
	nitroFSInit(NULL);
#endif
	defaultExceptionHandler();
	
	//Get executable's path
	fatInitDefault();

 	irqEnable(IRQ_CARD_LINE); 
 	irqSet(IRQ_CARD_LINE, card_line_irq);

#ifdef CYG_PROFILER
 irqEnable(IRQ_HBLANK); 
 irqSet(IRQ_HBLANK, hblankCount);
 cygprofile_begin();
 cygprofile_enable();
#endif

	//irqEnable(IRQ_FIFO_NOT_EMPTY);
	//fifoInit();
	
	strcpy(gModulePath, "/");
bool usebasepath = true;
	//Get path of the data folder
#ifdef READ_FROM_SD
	strcpy(gDataPath, "fat:/datacse2");
	usebasepath = false;
#endif

#ifdef NITROFS
	strcpy(gDataPath, "nitro:/datacse2");
	usebasepath = false;
#endif

	if(usebasepath)
		strcpy(gDataPath, "datacse2");


	//Load configuration
	
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);
	
	gDebug.bEnabled = conf.bDebug;
	gb50Fps = conf.bFps;
	gbTransparency = conf.bTransparency;
	gbAlternateDrams = conf.bDrams;
	
	scanKeys();
	if(conf.bBottomScreen || keysHeld() & KEY_START)
	{
		conf.bBottomScreen = true;
		lcdMainOnBottom();
	}

	
	memcpy(bindings, conf.bindings, sizeof(bindings));

	memset(&profile, 0, sizeof(PROFILE));

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
	
	//Draw loading screen
	CortBox(&clip_rect, 0x000000);
	PutBitmap3(&clip_rect, (WINDOW_WIDTH - 64) / 2, (WINDOW_HEIGHT - 8) / 2, &loading_rect, SURFACE_ID_LOADING);
	

	printf("gDataPath is %s\n", gDataPath);

	//Draw to screen
	if (Flip_SystemTask())
	{


		gLoadingProgress = 2;
		PutLoadingProgress();

#ifdef READ_WITH_SD
		//pxtone stuff

		// INIT PXTONE.
		printf("initing pxtone\n");
		pxtnERR        pxtn_err = pxtnERR_VOID;
		pxtn = new pxtnService();
		pxtn_err = pxtn->init();
		pxtn->set_destination_quality( _CHANNEL_NUM, _SAMPLE_PER_SECOND );

		printf("pxtone INIT\n");

		irqDisable(IRQ_TIMER2);
		// re-set timer2
		TIMER2_CR = 0;
		TIMER2_DATA = TIMER_FREQ_256(250); //1000ms
		TIMER2_CR = TIMER_ENABLE | ClockDivider_256 | TIMER_IRQ_REQ; 
		irqEnable(IRQ_TIMER2);
		irqSet(IRQ_TIMER2, Timer_1ms);

		cpuStartTiming(0);

		gPxtoneInited = 1;
#endif

		//Initialize sound
		InitDirectSound();
		
		//Initialize stuff
		InitTextObject();
		InitTriangleTable();
		//Run game code
		Game();
		printf("bye i guess");
		if(gConsoleInited == 3) while (true){}
		
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
