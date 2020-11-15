#include <stddef.h>
#include <stdio.h>
#include <stdarg.h> 
#include <stdlib.h>
#include <cstring>
#include <ctype.h>

#include "Debug.h"

#include "WindowsWrapper.h"

#include "ArmsItem.h"
#include "Boss.h"
#include "Bullet.h"
#include "CommonDefines.h"
#include "Draw.h"
#include "Flags.h"
#include "Frame.h"
#include "KeyControl.h"
#include "Main.h"
#include "MyChar.h"
#include "MycParam.h"
#include "NpChar.h"
#include "Organya.h"
#include "Sound.h"
#include "Stage.h"
#include "TextScr.h"

#include "nds.h"

DebugVars gDebug;
//spaghetti pizza
//hi rain how ds port

/*

void ConsoleRespond(const char* fmt, ...)
{
va_list buf;
	va_start(buf, fmt);
	vsnprintf(gConsole.response, sizeof(gConsole.response), fmt, buf);
	va_end(buf);

	gConsole.response_timer = 100;
}

void DebugPrint(const char* fmt, ...)
{
	va_list buf;
	va_start(buf, fmt);
	vsnprintf(gConsole.debug_texts[gConsole.debug_text_count], 
		sizeof(gConsole.debug_texts[gConsole.debug_text_count]), fmt, buf);
	va_end(buf);

	gConsole.debug_text_count++;
	if (gConsole.debug_text_count >= CONSOLE_MAX_HISTORY) gConsole.debug_text_count = CONSOLE_MAX_HISTORY - 1;
}

static void ClearDebugText(void)
{
	gConsole.debug_text_count = 0;
	for(int i=0;i<CONSOLE_MAX_HISTORY;i++)
	{
		 gConsole.debug_texts[i] = new char[CONSOLE_MAX_TEXT];
		 gConsole.debug_texts[i][0] = 0;
	};
}

static void PutDebugText(void)
{
	int x, y;
	if (gConsole.debug_text_count == 0)
		return;

	for (int i = 0; i < gConsole.debug_text_count; i++)
	{
		x =  WINDOW_WIDTH - (4 * strlen(gConsole.debug_texts[i]));
		y = 16 + (16 * i);
		PutText(&grcGame, x, y, gConsole.debug_texts[i], RGB(0x0F, 0xFA, 0x00));
	}
	ClearDebugText();
}


static void Callback_Warp(char** args, int num)
{
	//TODO: expose gTMT for name matching
	int stage = atoi(args[0]);
	if(stage >= 128 || stage < 0) {ConsoleRespond("Stage out of bounds.."); return;}
	TransferStage(stage, 0, 8, 8);

	ConsoleRespond("Transfer to stage %d.", stage);
}


static void Callback_Script(char** args, int num)
{
	int script = atoi(args[0]);
	StartTextScript(script);
	ConsoleRespond("Starting text script #%i.", script);
}

static void Callback_HP(char** args, int num)
{
	int health = atoi(args[0]);
	gMC.life = health;
}
static void Callback_MaxHP(char** args, int num)
{
	int health = atoi(args[0]);
	gMC.max_life = health;
}

static void Callback_GiveWeapon(char** args, int num)
{
	int weapon = atoi(args[0]);
	int ammo = 0;
	if (num >= 2) ammo = atoi(args[1]);
	AddArmsData(weapon, ammo);
	PlaySoundObject(38, SOUND_MODE_PLAY);
	ConsoleRespond("Gave weapon %i with %i ammo.", weapon, ammo);
}

static void Callback_GiveItem(char** args, int num)
{
	int item = atoi(args[0]);
	AddItemData(item);
	PlaySoundObject(29, SOUND_MODE_PLAY);
	ConsoleRespond("Gave item: #%i.", item);
}

static void Callback_DropWeapon(char** args, int num)
{
	int weapon = atoi(args[0]);
	SubArmsData(weapon);
	ConsoleRespond("Took weapon %i.", weapon);
}

static void Callback_DropItem(char** args, int num)
{
	int item = atoi(args[0]);
	SubItemData(item);
	ConsoleRespond("Took item: #%i.", item);
}

static void Callback_XP(char** args, int num)
{
	int xp = atoi(args[0]);
	AddExpMyChar(xp);
	PlaySoundObject(14, SOUND_MODE_PLAY);
	ConsoleRespond("Added %i xp.", xp);
}

static void Callback_Level(char** args, int num)
{
	int level = atoi(args[0]);
	gArmsData[gSelectedArms].level = level;
	ConsoleRespond("Set level %i.", level);
}

static void Callback_Ammo(char** args, int num)
{
	int ammo = atoi(args[0]);
	gArmsData[gSelectedArms].num = ammo;
	if (gArmsData[gSelectedArms].max_num == 0)
		gArmsData[gSelectedArms].max_num = ammo;
	ConsoleRespond("Set ammo %i.", ammo);
}

static void Callback_MaxAmmo(char** args, int num)
{
	int ammo = atoi(args[0]);
	gArmsData[gSelectedArms].max_num = ammo;
	ConsoleRespond("Set maxammo %i.", ammo);
}

static void Callback_Flag(char** args, int num)
{
	int flag = atoi(args[0]);
	if(flag < 0 || flag >= 8000)
	{
		ConsoleRespond("Flag not within bounds.");
		return;
	}

	if(GetNPCFlag(flag))
	{
		CutNPCFlag(flag);
		ConsoleRespond("Cut flag %i.", flag);
	}
	else
	{
		SetNPCFlag(flag);
		ConsoleRespond("Set flag %i.", flag);
	}
}

static void Callback_ClearFlag(char** args, int num)
{
	InitFlags();
	ConsoleRespond("Cleared flags.");
}

static void Callback_Equip(char** args, int num)
{
const char* equipnames[] = 
	{
		"booster08",
		"map",
		"armsbarrier",
		"turbocharge",
		"airtank",
		"booster20",
		"mimigamask",
		"whimsical",
		"nikumaru",
	};

	int equip = 1;
	for(int i=0; i<9; i++)
	{
		if(strncmp(args[0], equipnames[i], strlen(args[0])) == 0)
		{
			if(gMC.equip & equip)
			{
				EquipItem(equip, false);
				ConsoleRespond("Un-equipped item %s (0x%x).", equipnames[i], equip);
			}
			else
			{
				EquipItem(equip, true);
				ConsoleRespond("Equipped item %s (0x%x).", equipnames[i], equip);
			}
			return;
		}
		equip <<= 1;
	}

	ConsoleRespond("Invalid item.");
}

static void Callback_Music(char** args, int num)
{
	int song = atoi(args[0]);
	if(song < 0 || song >= 255)
	{
		ConsoleRespond("Song not within bounds.");
		return;
	}
	ConsoleRespond("Playing song ID #%i.", song);
	ChangeMusic((MusicID)song);
}
static void Callback_Sound(char** args, int num)
{
	int sound = atoi(args[0]);
	if(sound < 0 || sound >= 255)
	{
		ConsoleRespond("Sound not within bounds.");
		return;
	}
	PlaySoundObject(sound, SOUND_MODE_PLAY);

	//Keep it open for convenience
	gConsole.visible = true;
	strcpy(gConsole.text, gConsole.text_history[gConsole.text_history_pos-1]);
	gConsole.text_pos = strlen(gConsole.text);
}

static void Callback_SkipIntro(char** args, int num)
{
	/*conf.bSkipIntro ^= 1;
	if(conf.bSkipIntro)
	{
		ConsoleRespond("Skip intro enabled.");
		
	}
	else
	{
		ConsoleRespond("Skip intro disabled.");
	}
	SaveConfigData(&conf);
}

static CommandFormat commandlist[] = 
{
	{"warp", Callback_Warp, 1},
	{"script", Callback_Script, 1},
	{"music", Callback_Music, 1},
	{"sound", Callback_Sound, 1},
	{"hp", Callback_HP, 1},
	{"maxhp", Callback_MaxHP, 1},
	{"giveweapon", Callback_GiveWeapon, 1},
	{"giveitem", Callback_GiveItem, 1},
	{"dropweapon", Callback_DropWeapon, 1},
	{"dropitem", Callback_DropItem, 1},
	{"ammo", Callback_Ammo, 1},
	{"maxammo", Callback_MaxAmmo, 1},
	{"xp", Callback_XP, 1},
	{"level", Callback_Level, 1},
	{"flag", Callback_Flag, 1},
	{"clearflag", Callback_ClearFlag, 0},
	{"equip", Callback_Equip, 1},
	{"skip-intro", Callback_SkipIntro, 0},
};*/

//In lieu of SDL_RenderDrawRect
static void PutSquare(RECT src, unsigned long col)
{
	RECT dst;
	for (int i = 0; i < 4; i++)
	{
		dst = src;
		switch (i)
		{
			case 0:
				dst.right = dst.left ;
				break;
			case 1:
				dst.bottom = dst.top ;
				break;
			case 2:
				dst.top = dst.bottom ;
				break;
			case 3:
				dst.left = dst.right ;
				break;
		}
		CortBox(&dst, col);
	}
}

//yes
static int SubpixelToScreenCoord(int coord)
{
	return (coord / 0x200);
}


static void PutHitboxes(void)
{
	static const long green_color = GetCortBoxColor(RGB(0x00, 0xFF, 0x00));
	static const long red_color = GetCortBoxColor(RGB(0xFF, 0x00, 0x00));
	static const long yellow_color = GetCortBoxColor(RGB(0xFF, 0xFF, 0x00));
	static const long cyan_color = GetCortBoxColor(RGB(0x00, 0xFF, 0xFF));
	static const long white_color = GetCortBoxColor(RGB(0xFF, 0xFF, 0xFF));
	RECT rect;
	int fx, fy;
	int side, back, front, temp;
	long color;

	if(!gDebug.bShowHitboxes)
		return;

	GetFramePosition(&fx, &fy);

	//Player collision box
	rect.left = SubpixelToScreenCoord(gMC.x) - SubpixelToScreenCoord(gMC.hit.back) - SubpixelToScreenCoord(fx);
	rect.top = SubpixelToScreenCoord(gMC.y) - SubpixelToScreenCoord(gMC.hit.top) - SubpixelToScreenCoord(fy);
	rect.right = SubpixelToScreenCoord(gMC.x) + SubpixelToScreenCoord(gMC.hit.back) - SubpixelToScreenCoord(fx);
	rect.bottom = SubpixelToScreenCoord(gMC.y) + SubpixelToScreenCoord(gMC.hit.bottom) - SubpixelToScreenCoord(fy);
	PutSquare(rect, yellow_color);

	//Player hitbox
	rect.left = SubpixelToScreenCoord(gMC.x) - SubpixelToScreenCoord(gMC.hit.back) - SubpixelToScreenCoord(fx) + SubpixelToScreenCoord(3 * 0x200);
	rect.top = SubpixelToScreenCoord(gMC.y) - SubpixelToScreenCoord(gMC.hit.top) - SubpixelToScreenCoord(fy) + SubpixelToScreenCoord(6 * 0x200);
	rect.right = SubpixelToScreenCoord(gMC.x) + SubpixelToScreenCoord(gMC.hit.back) - SubpixelToScreenCoord(fx) - SubpixelToScreenCoord(3 * 0x200);
	rect.bottom = SubpixelToScreenCoord(gMC.y) + SubpixelToScreenCoord(gMC.hit.bottom) - SubpixelToScreenCoord(fy) - SubpixelToScreenCoord(6 * 0x200);
	CortBox(&rect, cyan_color);

	for (int n = 0; n < NPC_MAX; ++n)
	{
		if (gNPC[n].cond & 0x80)
		{
			if(gNPC[n].damage)
			{
				color = red_color;
			}
			else if(gNPC[n].bits & (NPC_EVENT_WHEN_TOUCHED | NPC_INTERACTABLE))
			{
				color = green_color;
			}
			else
			{
				color = white_color;
			}
			
			back = gNPC[n].hit.back;
			front = gNPC[n].hit.front;

			if (gNPC[n].bits & (NPC_SOLID_SOFT | NPC_SOLID_HARD))
			{
				front = gNPC[n].hit.back;
			}

			if (gNPC[n].direct == 0)
			{
				temp = front;
				front = back;
				back = temp;
			}

			rect.left = SubpixelToScreenCoord(gNPC[n].x) - SubpixelToScreenCoord(back) - SubpixelToScreenCoord(fx);
			rect.top = SubpixelToScreenCoord(gNPC[n].y) - SubpixelToScreenCoord(gNPC[n].hit.top) - SubpixelToScreenCoord(fy);
			rect.right = SubpixelToScreenCoord(gNPC[n].x) + SubpixelToScreenCoord(front) - SubpixelToScreenCoord(fx);
			rect.bottom = SubpixelToScreenCoord(gNPC[n].y) + SubpixelToScreenCoord(gNPC[n].hit.bottom) - SubpixelToScreenCoord(fy);

			PutSquare(rect, color);

			/*if (gNPC[n].direct == 0)
				side = gNPC[n].view.front;
			else
				side = gNPC[n].view.back;

			rect.left = SubpixelToScreenCoord(gNPC[n].x) - SubpixelToScreenCoord(side) - SubpixelToScreenCoord(fx);
			rect.top = SubpixelToScreenCoord(gNPC[n].y) - SubpixelToScreenCoord(gNPC[n].view.top) - SubpixelToScreenCoord(fy);
			rect.right = SubpixelToScreenCoord(gNPC[n].x) + SubpixelToScreenCoord(side) - SubpixelToScreenCoord(fx);
			rect.bottom = SubpixelToScreenCoord(gNPC[n].y) + SubpixelToScreenCoord(gNPC[n].view.bottom) - SubpixelToScreenCoord(fy);

			PutSquare(rect, color);
			*/
		}
	}

	for (int n = 0; n < BOSS_MAX; ++n)
	{
		if (gBoss[n].cond & 0x80)
		{
			if(gBoss[n].damage)
			{
				color = red_color;
			}
			else
			{
				color = white_color;
			}

			back = gBoss[n].hit.back;
			front = gBoss[n].hit.front;

			if (gBoss[n].bits & (NPC_SOLID_SOFT | NPC_SOLID_HARD))
			{
				front = gBoss[n].hit.back;
			}

			if (gBoss[n].direct == 0)
			{
				temp = front;
				front = back;
				back = temp;
			}
			rect.left = SubpixelToScreenCoord(gBoss[n].x) - SubpixelToScreenCoord(back) - SubpixelToScreenCoord(fx);
			rect.top = SubpixelToScreenCoord(gBoss[n].y) - SubpixelToScreenCoord(gBoss[n].hit.top) - SubpixelToScreenCoord(fy);
			rect.right = SubpixelToScreenCoord(gBoss[n].x) + SubpixelToScreenCoord(front) - SubpixelToScreenCoord(fx);
			rect.bottom = SubpixelToScreenCoord(gBoss[n].y) + SubpixelToScreenCoord(gBoss[n].hit.bottom) - SubpixelToScreenCoord(fy);

			PutSquare(rect, color);
		}
	}


	for (int n = 0; n < BULLET_MAX; ++n)
	{
		if (gBul[n].cond & 0x80)
		{
			rect.left = SubpixelToScreenCoord(gBul[n].x) - SubpixelToScreenCoord(gBul[n].enemyXL) - SubpixelToScreenCoord(fx);
			rect.top = SubpixelToScreenCoord(gBul[n].y) - SubpixelToScreenCoord(gBul[n].enemyYL) - SubpixelToScreenCoord(fy);
			rect.right = SubpixelToScreenCoord(gBul[n].x) + SubpixelToScreenCoord(gBul[n].enemyXL) - SubpixelToScreenCoord(fx);
			rect.bottom = SubpixelToScreenCoord(gBul[n].y) + SubpixelToScreenCoord(gBul[n].enemyYL) - SubpixelToScreenCoord(fy);

			PutSquare(rect, red_color);

			rect.left = SubpixelToScreenCoord(gBul[n].x) - SubpixelToScreenCoord(gBul[n].blockXL) - SubpixelToScreenCoord(fx);
			rect.top = SubpixelToScreenCoord(gBul[n].y) - SubpixelToScreenCoord(gBul[n].blockYL) - SubpixelToScreenCoord(fy);
			rect.right = SubpixelToScreenCoord(gBul[n].x) + SubpixelToScreenCoord(gBul[n].blockXL) - SubpixelToScreenCoord(fx);
			rect.bottom = SubpixelToScreenCoord(gBul[n].y) + SubpixelToScreenCoord(gBul[n].blockYL) - SubpixelToScreenCoord(fy);

			PutSquare(rect, cyan_color);
		}
	}
}

bool shown = false;
void PutConsole(void)
{

	PutHitboxes();
}

void InitConsole(void)
{
	gDebug.bEnabled = conf.bDebug;

	gDebug.bGodmode = false;
	gDebug.bNoclip = false;
	gDebug.bFastForward = false;
	gDebug.FastForwardTimer = 0;

 	gDebug.FrameAdvanceCount = 0;
	gDebug.bFrameFreeze = false;
	gDebug.bFrameCanAdvance = false;

	gDebug.bShowHitboxes = false;

}
