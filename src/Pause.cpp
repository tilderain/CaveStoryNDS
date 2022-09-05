
#include "Pause.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WindowsWrapper.h"

//#include "Backends/Controller.h"
//#include "Backends/Misc.h"
#include "CommonDefines.h"
#include "Config.h"
#include "Draw.h"
#include "Debug.h"
#include "Escape.h"
#include "KeyControl.h"
#include "Main.h"
#include "Organya.h"
#include "Sound.h"
#include "Stage.h"

#include "Game.h"

#include "Flags.h"

#include "nds.h"

#include "Multi.h"
#include "nifi.h"

#include "Random.h"

#include "Profile.h"

#include "dswifi9.h"

#define MAX_OPTIONS ((WINDOW_HEIGHT / 20) - 2)	// The maximum number of options we can fit on-screen at once

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

static BOOL restart_required;

RECT rect_cur = {112, 88, 128, 104};

char gStartingNetplay = false;
int nifiChannel = 11;

signed char gEnemyHPMultiplier = 2;
signed char gEnemyDamageMultiplier = 0;
char gRespawnEnabled = true;


static int Callback_Stub(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = CALLBACK_CONTINUE;

	return return_value;
}


char* GetKeyName(int key)
{
	
	switch (key)
	{
 		case KEY_A      :  return "A";   break;
 		case KEY_B      :   return "B";  break;
 		case KEY_SELECT:     return "Select";     break;
 		case KEY_START:      return "Start";  break;
 		case KEY_RIGHT:      return "Right";    break;
 		case KEY_LEFT :         return "Left";  break;
 		case KEY_UP   :          return "Up";  break;
 		case KEY_DOWN :   return "Down";   break;
		case KEY_L     :  return "L";  break;
 		case KEY_R     :   return "R"; break;
 		case KEY_X     :   return "X"; break;
 		case KEY_Y     :   return "Y"; break;
 		case KEY_TOUCH :   return "Touch"; break;
	}

	return "Unknown";
}

int autoRepeatTimer = 0;

int EnterOptionsMenuIngame(OptionsMenu *options_menu)
{

	unsigned int anime = 0;

	int return_value;

	unsigned long back_color = GetCortBoxColor(RGB(0, 0, 0));
	
	// Get pressed keys

	// Allow unpausing by pressing the pause button only when in the main pause menu (not submenus)
	/*if (!options_menu->submenu && gKeyTrg & KEY_PAUSE)
	{
		return_value = CALLBACK_CONTINUE;
		break;
	}
*/
	// Go back one submenu when the 'cancel' button is pressed
	if (gKeyTrg & gKeyCancel || gKeyTrgP2 & gKeyCancel)
	{
		return_value = CALLBACK_CONTINUE;
		gDebug.cheatVisible = false;
		for (size_t i = 0; i < options_menu->total_options; ++i)
			options_menu->options[i].callback(options_menu, i, ACTION_DEINIT);
		return return_value;
	}

	// Handling up/down input
	if (gKeyTrg & (gKeyUp | gKeyDown) || gKeyTrgP2 & (gKeyUp | gKeyDown))
	{
		const size_t old_selection = gDebug.cursorPos;

		if (gKeyTrg & gKeyDown || gKeyTrgP2 & gKeyDown)
			if (gDebug.cursorPos++ == options_menu->total_options - 1)
				gDebug.cursorPos = 0;

		if (gKeyTrg & gKeyUp || gKeyTrgP2 & gKeyUp)
			if (gDebug.cursorPos-- == 0)
				gDebug.cursorPos = options_menu->total_options - 1;

		// Update the menu-scrolling, if there are more options than can be fit on the screen
		if (gDebug.cursorPos < old_selection)
			gDebug.cheatScroll = MAX(0, MIN(gDebug.cheatScroll, (int)gDebug.cursorPos - 1));

		if (gDebug.cursorPos > old_selection)
			gDebug.cheatScroll = MIN(MAX(0, (int)options_menu->total_options - MAX_OPTIONS), MAX(gDebug.cheatScroll, (int)gDebug.cursorPos - (MAX_OPTIONS - 2)));

		PlaySoundObject(1, SOUND_MODE_PLAY);
	}

	// Run option callbacks
	for (size_t i = 0; i < options_menu->total_options; ++i)
	{
		if (!options_menu->options[i].disabled)
		{
			CallbackAction action = ACTION_UPDATE;

			if (i == gDebug.cursorPos)
			{
				if (gKeyTrg & gKeyOk || gKeyTrgP2 & gKeyOk)
					action = ACTION_OK;
				else if (gKeyTrg & gKeyLeft || (gKey & gKeyLeft && autoRepeatTimer++ > 20)\
					|| gKeyTrgP2 & gKeyLeft || (gKeyP2 & gKeyLeft && autoRepeatTimer++ > 20))
					action = ACTION_LEFT;
				else if (gKeyTrg & gKeyRight || (gKey & gKeyRight && autoRepeatTimer++ > 20)\
					|| gKeyTrgP2 & gKeyRight || (gKeyP2 & gKeyRight && autoRepeatTimer++ > 20))
					action = ACTION_RIGHT;

				if(gKey & gKeyLeft || gKey & gKeyRight || gKeyP2 & gKeyLeft || gKeyP2 & gKeyRight)
				{}
				else
				{autoRepeatTimer = 0;}
			}
			

			return_value = options_menu->options[i].callback(options_menu, i, action);

			// If the callback returned something other than CALLBACK_CONTINUE, it's time to exit this submenu
			if (return_value != CALLBACK_CONTINUE)
				return return_value;
		}
	}

	if (return_value != CALLBACK_CONTINUE)
		return return_value;

	const size_t visible_options = MIN(MAX_OPTIONS, options_menu->total_options);

	int y = (WINDOW_HEIGHT / 2) - ((visible_options * 20) / 2) - (40 / 2);
	y += 40;
	
	CortBoxAlpha(&grcGame, back_color, 24);

	for (size_t i = gDebug.cheatScroll; i < gDebug.cheatScroll + visible_options; ++i)
	{
		const int x = (WINDOW_WIDTH / 2) + options_menu->x_offset;

		// Draw Quote next to the selected option
		if (i == gDebug.cursorPos)
			PutBitmap3(&grcFull, x - 20, y - 8, &rect_cur, SURFACE_ID_TEXT_BOX);

		unsigned long option_colour = options_menu->options[i].disabled ? RGB(0x80, 0x80, 0x80) : RGB(0xFF, 0xFF, 0xFF);

		// Draw option name
		PutText(&grcGame,x, y - (9 / 2), options_menu->options[i].name, option_colour);

		// Draw option value, if it has one
		if (options_menu->options[i].value_string != NULL)
			PutText(&grcGame,x + 100, y - (9 / 2), options_menu->options[i].value_string, option_colour);

		y += 20;
	}

	return return_value;
}

static int EnterOptionsMenu(OptionsMenu *options_menu, size_t selected_option)
{
	int scroll = 0;

	unsigned int anime = 0;

	int return_value;

	unsigned long back_color = GetCortBoxColor(RGB(0x20, 0x20, 0x20));

	// Initialise options
	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback(options_menu, i, ACTION_INIT);

	int dots = 0;
	int dotstimer = 0;

	for (;;)
	{
		int status = nifiGetStatus();

		if(nifiIsHost() && status != HOST_INGAME) nifiHostWait();
		if(nifiIsClient() && status != CLIENT_INGAME) nifiClientWait();


		if(status == HOST_CONNECTED)
		{
			options_menu->options[0].disabled = false;
			options_menu->options[1].disabled = false;
			options_menu->options[2].disabled = true;
			options_menu->subtitle = "Connected!";
		}
		if(status == CLIENT_CONNECTED)
		{
			options_menu->options[0].name = "Connected! Waiting for host...";
			options_menu->options[1].disabled = true;
		}
		// Get pressed keys
		GetTrg();

		// Allow unpausing by pressing the pause button only when in the main pause menu (not submenus)
		/*if (!options_menu->submenu && gKeyTrg & KEY_PAUSE)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}
*/
		// Go back one submenu when the 'cancel' button is pressed
		if (gKeyTrg & gKeyCancel)
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		// Handling up/down input
		if (gKeyTrg & (gKeyUp | gKeyDown))
		{
			const size_t old_selection = selected_option;

			if (gKeyTrg & gKeyDown)
				if (selected_option++ == options_menu->total_options - 1)
					selected_option = 0;

			if (gKeyTrg & gKeyUp)
				if (selected_option-- == 0)
					selected_option = options_menu->total_options - 1;

			// Update the menu-scrolling, if there are more options than can be fit on the screen
			if (selected_option < old_selection)
				scroll = MAX(0, MIN(scroll, (int)selected_option - 1));

			if (selected_option > old_selection)
				scroll = MIN(MAX(0, (int)options_menu->total_options - MAX_OPTIONS), MAX(scroll, (int)selected_option - (MAX_OPTIONS - 2)));

			PlaySoundObject(1, SOUND_MODE_PLAY);
		}

		// Run option callbacks
		for (size_t i = 0; i < options_menu->total_options; ++i)
		{
			if (!options_menu->options[i].disabled)
			{
				CallbackAction action = ACTION_UPDATE;

				if (i == selected_option)
				{
					if (gKeyTrg & gKeyOk)
						action = ACTION_OK;
					else if (gKeyTrg & gKeyLeft || (gKey & gKeyLeft && autoRepeatTimer++ > 20))
						action = ACTION_LEFT;
					else if (gKeyTrg & gKeyRight || (gKey & gKeyRight && autoRepeatTimer++ > 20))
						action = ACTION_RIGHT;

					if(gKey & gKeyLeft || gKey & gKeyRight)
					{}
					else
					{autoRepeatTimer = 0;}
				}

				return_value = options_menu->options[i].callback(options_menu, i, action);

				// If the callback returned something other than CALLBACK_CONTINUE, it's time to exit this submenu
				if (return_value != CALLBACK_CONTINUE)
					break;
			}
		}

		if (return_value != CALLBACK_CONTINUE)
			break;

		// Update Quote animation counter
		if (++anime >= 40)
			anime = 0;

		// Draw screen
		CortBox(&grcGame, back_color);

		const size_t visible_options = MIN(MAX_OPTIONS, options_menu->total_options);

		int y = (WINDOW_HEIGHT / 2) - ((visible_options * 20) / 2) - (40 / 2);

		// Draw title
		PutText(&grcGame, (WINDOW_WIDTH / 2) - ((strlen(options_menu->title) * 5) / 2), y, options_menu->title, RGB(0xFF, 0xFF, 0xFF));

		// Draw subtitle
		if (options_menu->subtitle != NULL)
		{
			if(options_menu->subtitle[strlen(options_menu->subtitle)-1] == '.')
			{
				int spacing = GetTextSpacing(options_menu->subtitle);
				int spacingDot = GetTextSpacing(".");
				if(dotstimer++ > 40){dots++; dotstimer=0;}
				if(dots > 2) dots = 0;
				for (size_t i = 0; i < dots; i++)
				{
					PutText(&grcGame, (WINDOW_WIDTH / 2) - ((strlen(options_menu->subtitle) * 5) / 2) + spacing + (spacingDot*i), y + 18, ".", RGB(0xFF, 0xFF, 0xFF));
				}
				
			}
			PutText(&grcGame, (WINDOW_WIDTH / 2) - ((strlen(options_menu->subtitle) * 5) / 2), y + 18, options_menu->subtitle, RGB(0xFF, 0xFF, 0xFF));
		}

		y += 40;

		for (size_t i = scroll; i < scroll + visible_options; ++i)
		{
			const int x = (WINDOW_WIDTH / 2) + options_menu->x_offset;

			// Draw Quote next to the selected option
			if (i == selected_option)
				PutBitmap3(&grcFull, x - 20, y - 8, &rect_cur, SURFACE_ID_TEXT_BOX);

			unsigned long option_colour = options_menu->options[i].disabled ? RGB(0x80, 0x80, 0x80) : RGB(0xFF, 0xFF, 0xFF);

			// Draw option name
			PutText(&grcGame,x, y - (9 / 2), options_menu->options[i].name, option_colour);

			// Draw option value, if it has one
			if (options_menu->options[i].value_string != NULL)
				PutText(&grcGame,x + 100, y - (9 / 2), options_menu->options[i].value_string, option_colour);

			y += 20;
		}

		PutFramePerSecound();

		if(gStartingNetplay) 
		{
			return_value = CALLBACK_CONTINUE;
			break;
		}

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return_value = CALLBACK_EXIT;
			break;
		}
	}

	for (size_t i = 0; i < options_menu->total_options; ++i)
		options_menu->options[i].callback(options_menu, i, ACTION_DEINIT);

	return return_value;
}

///////////////////
// Controls menu //
///////////////////

typedef struct Control
{
	const char *name;
	size_t binding_index;
	unsigned char groups;
} Control;

// The bitfield on the right determines which 'group' the
// control belongs to - if two controls are in the same group,
// they cannot be bound to the same key.
static const Control controls[] = {
	{"Up",              BINDING_UP,     (1 << 0) | (1 << 1)},
	{"Down",            BINDING_DOWN,   (1 << 0) | (1 << 1)},
	{"Left",            BINDING_LEFT,   (1 << 0) | (1 << 1)},
	{"Right",           BINDING_RIGHT,  (1 << 0) | (1 << 1)},
	{"Jump",            BINDING_JUMP,    1 << 0},
	{"Shoot",           BINDING_SHOT,    1 << 0},
	{"Previous Weapon", BINDING_ARMSREV, 1 << 0},
	{"Next Weapon",     BINDING_ARMS,    1 << 0},
	{"Inventory",       BINDING_ITEM,    1 << 0},
	{"Map",             BINDING_MAP,     1 << 0},
};

static char bound_name_buffers[sizeof(controls) / sizeof(controls[0])][20];

static int Callback_ControlsKeyboard_Rebind(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	switch (action)
	{
		default:
			break;

		case ACTION_INIT:
			if(this_option == 0) break;
			strncpy(bound_name_buffers[this_option-1], GetKeyName(bindings[controls[this_option-1].binding_index].keyboard), sizeof(bound_name_buffers[0]) - 1);
			break;

		case ACTION_OK:
			PlaySoundObject(5, SOUND_MODE_PLAY);

			char timeout_string[2];
			timeout_string[1] = '\0';


			CONFIG_BINDING tempBindings[BINDING_TOTAL];
			memset(tempBindings, 0, sizeof(tempBindings));
		
			int exit = false;
			int timer = 0;
			// Time-out and exit if the user takes too long (they probably don't want to rebind)
			for (int current_control = 1; current_control < sizeof(controls) / sizeof(controls[0]) + 1; current_control++)
			{
				int setTimer = 0;
				char *key_name = "";
				bool key_failed = false;
				int key_failed_id = 0;
				bool cont = false;
				for(;;)
				{
					int reset = keysHeld();
					if(reset & KEY_L && reset & KEY_R && reset & KEY_START)
					{
						exit = true;
						break;
					}

					int keys = keysUp();
					if(timer++>16 && keys && !setTimer)
					{
						key_name = GetKeyName(keys);

						if(strcmp(key_name, "Unknown") == 0) {Flip_SystemTask(); continue;}

						cont = false;
						// If another control in the same group uses this key, swap them
						for (size_t other_option = 0; other_option < parent_menu->total_options; ++other_option)
						{
							if (other_option != current_control && controls[other_option].groups & controls[current_control].groups && tempBindings[controls[other_option].binding_index].keyboard == keys)
							{
								key_failed = true;
								key_failed_id = other_option+1;
								PlaySoundObject(SND_QUOTE_BUMP_HEAD, 1);
								cont = true;
								timer = 0;
							}
						}
						if(cont) {Flip_SystemTask(); continue;}

						// Otherwise just overwrite the selected control
						tempBindings[current_control-1].keyboard = keys;
		

						PlaySoundObject(18, SOUND_MODE_PLAY);

						gKeyTrg = gKey = 0;	// Prevent weird input-ghosting by doing this
						setTimer = 1;
						key_failed = false;
					}

					// Draw screen
					CortBox(&grcFull, 0x000000);

					const char *string = "Press a key to bind to this action";
					const char *string2 = "or press L+R+Start to cancel";
					PutText(&grcGame,(WINDOW_WIDTH / 2) - ((strlen(string) * 5) / 2), (WINDOW_HEIGHT / 2) - 10 - 25, string, RGB(0xFF, 0xFF, 0xFF));
					PutText(&grcGame,(WINDOW_WIDTH / 2) - ((strlen(string2) * 5) / 2), (WINDOW_HEIGHT / 2) + 4 - 25, string2, RGB(0xFF, 0xFF, 0xFF));

					PutText(&grcGame,(WINDOW_WIDTH / 2) - ((strlen(parent_menu->options[current_control].name) * 5) / 2), (WINDOW_HEIGHT / 2) + 10, parent_menu->options[current_control].name, RGB(0xFF, 0xFF, 0xFF));
					int color;
					if(!key_failed)
						color = RGB(0x90, 0xFF, 0xFF);
					else
						color = RGB(0xFF, 0x90, 0xFF);
					PutText(&grcGame,(WINDOW_WIDTH / 2) - ((strlen(key_name) * 5) / 2), (WINDOW_HEIGHT / 2) + 34, key_name, color);
					const char *string3 = "already used by";
					if(key_failed)
					{
						PutText(&grcGame,(WINDOW_WIDTH / 2) - 38, (WINDOW_HEIGHT / 2) + 50, string3, RGB(0xFF, 0xFF, 0xFF));
						PutText(&grcGame,(WINDOW_WIDTH / 2) - ((strlen(parent_menu->options[key_failed_id].name) * 5) / 2), (WINDOW_HEIGHT / 2) + 68,
							parent_menu->options[key_failed_id].name, RGB(0xFF, 0xFF, 0xFF));
					}
						

					PutFramePerSecound();

					if(setTimer && setTimer++ > 25) break;

					if (!Flip_SystemTask())
					{
						// Quit if window is closed
						return CALLBACK_EXIT;
					}
				}
				if(exit) break;
			}
			if(exit) break;
			for(int current_control=1;current_control < parent_menu->total_options;current_control++)
			{
				for (size_t i = 0; i < parent_menu->total_options; ++i)
					parent_menu->options[i].callback(parent_menu, i, ACTION_INIT);
				bindings[current_control-1].keyboard = tempBindings[current_control-1].keyboard;
			}

			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_ControlsKeyboard(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	Option options[sizeof(controls) / sizeof(controls[0]) + 1];

	options[0].name = "Rebind all";
	options[0].callback = Callback_ControlsKeyboard_Rebind;
	options[0].value_string = " ";
	options[0].disabled = FALSE;

	for (size_t i = 1; i < sizeof(controls) / sizeof(controls[0]) + 1; ++i)
	{
		options[i].name = controls[i-1].name;
		options[i].callback = Callback_ControlsKeyboard_Rebind;
		options[i].value_string = bound_name_buffers[i-1];
		options[i].disabled = TRUE;
	}

	OptionsMenu options_menu = {
		"CONTROLS",
		NULL,
		options,
		sizeof(options) / sizeof(options[0]),
		-60,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	return return_value;
}

/////////////////////
// Soundtrack menu //
/////////////////////
char musicString[32];

static int Callback_Music(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = 1;
			//this needs to be a different value than the Soundtest otherwise they will just optimize into the same var..
			parent_menu->options[this_option].value_string = musicString;
			itoa(parent_menu->options[this_option].value, parent_menu->options[this_option].value_string, 10);
			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = 53;
			}
			else if (action == ACTION_RIGHT)
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > 53)
					parent_menu->options[this_option].value = 0;
			}

			if(action == ACTION_OK)
				ChangeMusic((MusicID)parent_menu->options[this_option].value);

			itoa(parent_menu->options[this_option].value, parent_menu->options[this_option].value_string, 10);

			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

char soundString[32];

static int Callback_Sound(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = 0;
			parent_menu->options[this_option].value_string = soundString;
			itoa(parent_menu->options[this_option].value, parent_menu->options[this_option].value_string, 10);
			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			if (action == ACTION_LEFT)
			{
				// Decrement value (with wrapping)
				if (--parent_menu->options[this_option].value < 0)
					parent_menu->options[this_option].value = 164;
			}
			else if (action == ACTION_RIGHT)
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > 164)
					parent_menu->options[this_option].value = 0;
			}

			if(action == ACTION_OK)
				PlaySoundObject(parent_menu->options[this_option].value, 1);

			itoa(parent_menu->options[this_option].value, parent_menu->options[this_option].value_string, 10);

			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Cheat(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	char *strings[] = {"Off", "L+Up ingame"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bDebug;
			parent_menu->options[this_option].value_string = strings[conf->bDebug];
			break;

		case ACTION_DEINIT:
			conf->bDebug = parent_menu->options[this_option].value;
			gDebug.bEnabled = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Screen(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	char *strings[] = {"Top screen", "Bottom screen"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bBottomScreen;
			parent_menu->options[this_option].value_string = strings[conf->bBottomScreen];
			break;

		case ACTION_DEINIT:
			conf->bBottomScreen = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			lcdSwap();

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}


static int Callback_Transparency(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	char *strings[] = {"On", "Off"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bTransparency;
			parent_menu->options[this_option].value_string = strings[conf->bTransparency];
			break;

		case ACTION_DEINIT:
			conf->bTransparency = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			gbTransparency = parent_menu->options[this_option].value;

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Fps(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	char *strings[] = {"60 fps", "50 fps"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bFps;
			parent_menu->options[this_option].value_string = strings[conf->bFps];
			break;

		case ACTION_DEINIT:
			conf->bFps = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			gb50Fps = parent_menu->options[this_option].value;

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

static int Callback_Drams(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	char *strings[] = {"Off", "On"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = conf->bDrams;
			parent_menu->options[this_option].value_string = strings[conf->bDrams];
			break;

		case ACTION_DEINIT:
			conf->bTransparency = parent_menu->options[this_option].value;
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			parent_menu->options[this_option].value = (parent_menu->options[this_option].value + 1) % (sizeof(strings) / sizeof(strings[0]));

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			gbAlternateDrams = parent_menu->options[this_option].value;

			parent_menu->options[this_option].value_string = strings[parent_menu->options[this_option].value];
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}

////////////////
// Pause menu //
////////////////

static int PromptAreYouSure(void)
{
	struct FunctionHolder
	{
		static int Callback_Yes(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 1;	// Yes
		}

		static int Callback_No(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
		{
			(void)parent_menu;
			(void)this_option;

			if (action != ACTION_OK)
				return CALLBACK_CONTINUE;

			return 0;	// No
		}
	};

	Option options[] = {
		{"Yes", FunctionHolder::Callback_Yes, NULL, NULL, 0, FALSE},
		{"No", FunctionHolder::Callback_No, NULL, NULL, 0, FALSE}
	};

	OptionsMenu options_menu = {
		"ARE YOU SURE?",
		"UNSAVED PROGRESS WILL BE LOST",
		options,
		sizeof(options) / sizeof(options[0]),
		-10,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int return_value = EnterOptionsMenu(&options_menu, 1);

	PlaySoundObject(18, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_Resume(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	PlaySoundObject(18, SOUND_MODE_PLAY);
	return enum_ESCRETURN_continue;
}

static int Callback_Reset(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_RESET;	// Restart game
			break;
	}

	return return_value;
}

static int Callback_Quit(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	int return_value = PromptAreYouSure();

	switch (return_value)
	{
		case 0:
			return_value = CALLBACK_CONTINUE;	// Go back to previous menu
			break;

		case 1:
			return_value = CALLBACK_EXIT;	// Exit game
			break;
	}

	return return_value;
}

int Call_Pause(void)
{
	//(void)parent_menu;

	//if (action != ACTION_OK)
	//	return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIG conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	BOOL is_console = false;

	Option options_console[] = {

	};

	Option options_pc[] = {
		{"Control Config", Callback_ControlsKeyboard, &conf, NULL, 0, FALSE},
		{"Display screen", Callback_Screen, &conf, NULL, 0, FALSE},
		{"Transparent boxes", Callback_Transparency, &conf, NULL, 0, FALSE},
		{"Framerate", Callback_Fps, &conf, NULL, 0, FALSE},
		{"Alternate drums", Callback_Drams, &conf, NULL, 0, FALSE},
		{"Cheat Mode", Callback_Cheat, &conf, NULL, 0, FALSE},
		{"Play song", Callback_Music, NULL, NULL, 0, FALSE},
		{"Play sound", Callback_Sound, NULL, NULL, 0, FALSE},
		{"CSE2 for NDS", Callback_Stub, NULL, "= v0.4 =", 0, TRUE},
	};

	OptionsMenu options_menu = {
		"OPTIONS",
		restart_required ? "RESTART REQUIRED" : NULL,
		is_console ? options_console : options_pc,
		is_console ? (sizeof(options_console) / sizeof(options_console[0])) : (sizeof(options_pc) / sizeof(options_pc[0])),
		is_console ? -60 : -70,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	//PlaySoundObject(5, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	memcpy(conf.bindings, bindings, sizeof(bindings));

	const size_t visible_options = MIN(MAX_OPTIONS, options_menu.total_options);

	int y = (WINDOW_HEIGHT / 2) - ((visible_options * 20) / 2) - (40 / 2);
	char *string = "Saving..";
	PutText(&grcGame, (WINDOW_WIDTH / 2) - ((strlen(string) * 5) / 2), y + 14, string, RGB(0xFF, 0xFF, 0xFF));
	Flip_SystemTask();

	SaveConfigData(&conf);

	return return_value;
}


static void hostStartNetplay()
{

	if(gStartingNetplay == NETPLAY_START_LOAD)
	{
		nifiSendPacket(NIFI_CMD_TRANSFER_SRAM, (u8*)&profile, sizeof(profile), true);
		printf("Sent SRAM.\n");
	}
	int bufferSize = 20;
    u8 buffer[bufferSize];

	buffer[0] = gb50Fps;
	buffer[1] = gDebug.bEnabled;
	buffer[2] = gRespawnEnabled;
	buffer[3] = gEnemyHPMultiplier;
	buffer[4] = gEnemyDamageMultiplier;
	memcpy(&buffer[5], gSkipFlag, sizeof(gSkipFlag));

    nifiSendPacket(NIFI_CMD_HOST_START_GAME, buffer, bufferSize, true);
	nifiSetStatus(HOST_INGAME);
	gCounter = 0;
	msvc_srand(0);
	printf("Host: starting netplay\n");

	ResetDebugOptions();
}

static int Callback_HostStartGame(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	if (action == ACTION_OK)
	{
		LoadProfile(NULL);
		gStartingNetplay = NETPLAY_START_LOAD;
		hostStartNetplay();
	}

	int return_value = CALLBACK_CONTINUE;

	return return_value;
}

static int Callback_HostStartNewFile(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	if (action == ACTION_OK)
	{
		gStartingNetplay = NETPLAY_START_NORMAL;
		hostStartNetplay();
	}

	int return_value = CALLBACK_CONTINUE;

	return return_value;
}

static int Callback_ChangeChannel(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	char *strings[] = {"1", "6", "11"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = nifiChannel;
			for(int i=0;i<3;i++)
			{
				if(atoi(strings[i]) == nifiChannel)
					parent_menu->options[this_option].value_string = strings[i];
			}

			Wifi_SetChannel(nifiChannel);

			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			if(parent_menu->options[this_option].value == 1) parent_menu->options[this_option].value = 6;
			else if(parent_menu->options[this_option].value == 6) parent_menu->options[this_option].value = 11;
			else if(parent_menu->options[this_option].value == 11) parent_menu->options[this_option].value = 1;

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			nifiChannel = parent_menu->options[this_option].value;
			for(int i=0;i<3;i++)
			{
				if(atoi(strings[i]) == nifiChannel)
					parent_menu->options[this_option].value_string = strings[i];
			}

			Wifi_SetChannel(nifiChannel);
			
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}



static int Callback_ChangeEnemyHP(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	char *strings[] = {"1x", "1.5x", "2x", "2.5x", "3x"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = gEnemyHPMultiplier;
			parent_menu->options[this_option].value_string = strings[gEnemyHPMultiplier];

			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			if(action == ACTION_LEFT) gEnemyHPMultiplier--;
			else gEnemyHPMultiplier++;

			if(gEnemyHPMultiplier < 0) gEnemyHPMultiplier = 4;
			if(gEnemyHPMultiplier > 4) gEnemyHPMultiplier = 0;
			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value = gEnemyHPMultiplier;
			parent_menu->options[this_option].value_string = strings[gEnemyHPMultiplier];
			
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}


static int Callback_ChangeEnemyDamage(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	char *strings[] = {"1x", "2x", "3x"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = gEnemyDamageMultiplier;
			parent_menu->options[this_option].value_string = strings[gEnemyDamageMultiplier];

			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:

			// Increment value (with wrapping)
			if(action == ACTION_LEFT) gEnemyDamageMultiplier--;
			else gEnemyDamageMultiplier++;

			if(gEnemyDamageMultiplier < 0) gEnemyDamageMultiplier = 2;
			if(gEnemyDamageMultiplier > 2) gEnemyDamageMultiplier = 0;
			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			parent_menu->options[this_option].value = gEnemyDamageMultiplier;
			parent_menu->options[this_option].value_string = strings[gEnemyDamageMultiplier];
			
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}


static int Callback_ChangeRespawn(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	char *strings[] = {"No", "Yes"};

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = gRespawnEnabled;
			parent_menu->options[this_option].value_string = strings[gRespawnEnabled];
			break;

		case ACTION_DEINIT:
			break;

		case ACTION_OK:
		case ACTION_LEFT:
		case ACTION_RIGHT:
			parent_menu->options[this_option].value ^= 1;

			PlaySoundObject(SND_SWITCH_WEAPON, SOUND_MODE_PLAY);

			gRespawnEnabled = parent_menu->options[this_option].value;
			parent_menu->options[this_option].value_string = strings[gRespawnEnabled];
			
			break;

		case ACTION_UPDATE:
			break;
	}

	return CALLBACK_CONTINUE;
}


static int Callback_MultiHost(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	Option options_pc[] = {
		{"Start game", Callback_HostStartGame, NULL, NULL, 0, TRUE},
		{"Start game (new file)", Callback_HostStartNewFile, NULL, NULL, 0, TRUE},
		{"Channel:", Callback_ChangeChannel, NULL, NULL, 0, FALSE},
		{"Respawning:", Callback_ChangeRespawn, NULL, NULL, 0, FALSE},
		{"Enemy HP:", Callback_ChangeEnemyHP, NULL, NULL, 0, FALSE},
		{"Enemy DMG:", Callback_ChangeEnemyDamage, NULL, NULL, 0, FALSE},
	};

	OptionsMenu options_menu = {
		"Host",
		"Waiting.",
		options_pc,
		sizeof(options_pc) / sizeof(options_pc[0]),
		-60,
		TRUE
	};

	PlaySoundObject(5, SOUND_MODE_PLAY);

	int status = nifiGetStatus();
	if(status != HOST_WAITING && status != HOST_CONNECTED && status != HOST_INGAME)
		nifiHostMenu();
	const int return_value = EnterOptionsMenu(&options_menu, 0);

	if(!gStartingNetplay) nifiStop();

	PlaySoundObject(5, SOUND_MODE_PLAY);

	return return_value;
}

static int Callback_MultiConnect(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	Option options_pc[] = {
		{"Searching for game...", Callback_Stub, NULL, NULL, 0, TRUE},
		{"Channel:", Callback_ChangeChannel, NULL, NULL, 0, FALSE},
	};

	OptionsMenu options_menu = {
		"Connect",
		NULL,
		options_pc,
		sizeof(options_pc) / sizeof(options_pc[0]),
		-70,
		TRUE
	};

	nifiClientMenu();

	PlaySoundObject(5, SOUND_MODE_PLAY);
	const int return_value = EnterOptionsMenu(&options_menu, 0);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	if(!gStartingNetplay) nifiStop();

	return return_value;
}

int Call_Multi(void)
{
	//(void)parent_menu;

	//if (action != ACTION_OK)
	//	return CALLBACK_CONTINUE;

	// Make the options match the configuration data

	BOOL is_console = false;

	Option options_console[] = {

	};

	Option options_pc[] = {
		{"Host", Callback_MultiHost, NULL, NULL, 0, FALSE},
		{"Connect", Callback_MultiConnect, NULL, NULL, 0, FALSE},
	};

	OptionsMenu options_menu = {
		"Local Multiplayer",
		restart_required ? "RESTART REQUIRED" : NULL,
		is_console ? options_console : options_pc,
		is_console ? (sizeof(options_console) / sizeof(options_console[0])) : (sizeof(options_pc) / sizeof(options_pc[0])),
		is_console ? -60 : -60,
		TRUE
	};
	gStartingNetplay = 0;
	ChangeMusic(MUS_WHITE);

	PlaySoundObject(5, SOUND_MODE_PLAY);

	const int return_value = EnterOptionsMenu(&options_menu, 0);

	//PlaySoundObject(5, SOUND_MODE_PLAY);

	// Save our changes to the configuration file

	const size_t visible_options = MIN(MAX_OPTIONS, options_menu.total_options);

	int y = (WINDOW_HEIGHT / 2) - ((visible_options * 20) / 2) - (40 / 2);
	//Flip_SystemTask();

	return return_value;
}
