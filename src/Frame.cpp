#include <string.h>

#include "Frame.h"
#include "Map.h"
#include "MyChar.h"
#include "NpChar.h"
#include "Game.h"
#include "CommonDefines.h"

FRAME gFrame;

void MoveFrame3()
{
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);
	
	gFrame.x += (*gFrame.tgt_x - (WINDOW_WIDTH << 8) - gFrame.x) / gFrame.wait;
	gFrame.y += (*gFrame.tgt_y - (WINDOW_HEIGHT << 8) - gFrame.y) / gFrame.wait;
	
	//Keep in bounds
	if (gFrame.x <= -0x200)
		gFrame.x = 0;
	if (gFrame.y <= -0x200)
		gFrame.y = 0;
	
	if (gFrame.x > ((((map_w - 1) << 4) - WINDOW_WIDTH)) << 9)
		gFrame.x = (((map_w - 1) << 4) - WINDOW_WIDTH) << 9;
	if (gFrame.y > ((((map_l - 1) << 4) - WINDOW_HEIGHT)) << 9)
		gFrame.y = (((map_l - 1) << 4) - WINDOW_HEIGHT) << 9;

	//Quake
	if (gFrame.quake2)
	{
		gFrame.x += (Random(-5, 5) << 9);
		gFrame.y += (Random(-3, 3) << 9);
		--gFrame.quake2;
	}
	else if (gFrame.quake)
	{
		gFrame.x += (Random(-1, 1) << 9);
		gFrame.y += (Random(-1, 1) << 9);
		--gFrame.quake;
	}
	
	//Keep in bounds
	if (gFrame.x <= -0x200)
		gFrame.x = 0;
	if (gFrame.y <= -0x200)
		gFrame.y = 0;
}

void GetFramePosition(int *fx, int *fy)
{
	*fx = gFrame.x;
	*fy = gFrame.y;
}

void SetFramePosition(int fx, int fy)
{
	//End quake
	gFrame.quake = 0;
	gFrame.quake2 = 0;

	//Move frame position
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);
	
	gFrame.x = fx;
	gFrame.y = fy;
	
	//Keep in bounds
	if (gFrame.x <= -0x200)
		gFrame.x = 0;
	if (gFrame.y <= -0x200)
		gFrame.y = 0;

	if (gFrame.x > ((((map_w - 1) << 4) - WINDOW_WIDTH)) << 9)
		gFrame.x = (((map_w - 1) << 4) - WINDOW_WIDTH) << 9;
	if (gFrame.y > ((((map_l - 1) << 4) - WINDOW_HEIGHT)) << 9)
		gFrame.y = (((map_l - 1) << 4) - WINDOW_HEIGHT) << 9;
}

void SetFrameMyChar()
{
	//Move frame position
	int mc_x, mc_y;
	GetMyCharPosition(&mc_x, &mc_y);
	
	int16_t map_w, map_l;
	GetMapData(0, &map_w, &map_l);
	
	gFrame.x = mc_x - (WINDOW_WIDTH << 8);
	gFrame.y = mc_y - (WINDOW_HEIGHT << 8);
	
	//Keep in bounds
	if (gFrame.x <= -0x200)
		gFrame.x = 0;
	if (gFrame.y <= -0x200)
		gFrame.y = 0;
	
	if (gFrame.x > ((((map_w - 1) << 4) - WINDOW_WIDTH)) << 9)
		gFrame.x = (((map_w - 1) << 4) - WINDOW_WIDTH) << 9;
	if (gFrame.y > ((((map_l - 1) << 4) - WINDOW_HEIGHT)) << 9)
		gFrame.y = (((map_l - 1) << 4) - WINDOW_HEIGHT) << 9;
}

void SetFrameTargetMyChar(int wait)
{
	gFrame.tgt_x = &gMC.tgt_x;
	gFrame.tgt_y = &gMC.tgt_y;
	gFrame.wait = wait;
}

void SetFrameTargetNpChar(int event, int wait)
{
	for (int i = 0; i < NPC_MAX; i++)
	{
		if (gNPC[i].code_event == event)
		{
			gFrame.tgt_x = &gNPC[i].x;
			gFrame.tgt_y = &gNPC[i].y;
			gFrame.wait = wait;
			break;
		}
	}
}

void SetFrameTargetBoss(int no, int wait)
{
	//gFrame.tgt_x = &gBoss[no].x;
	//gFrame.tgt_y = &gBoss[no].y;
	gFrame.wait = wait;
}

void SetQuake(int time)
{
	gFrame.quake = time;
}

void SetQuake2(int time)
{
	gFrame.quake2 = time;
}

void ResetQuake()
{
	gFrame.quake = 0;
	gFrame.quake2 = 0;
}