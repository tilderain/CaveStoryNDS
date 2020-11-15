
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
#include "Escape.h"
#include "KeyControl.h"
#include "Main.h"
#include "Organya.h"
#include "Sound.h"
#include "Stage.h"

#include "nds.h"

#define MAX_OPTIONS ((WINDOW_HEIGHT / 20) - 2)	// The maximum number of options we can fit on-screen at once

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

enum
{
	CALLBACK_CONTINUE = -1,
	CALLBACK_PREVIOUS_MENU = -2,
	CALLBACK_RESET = -3,
	CALLBACK_EXIT = -4,
};

typedef enum CallbackAction
{
	ACTION_INIT,
	ACTION_DEINIT,
	ACTION_UPDATE,
	ACTION_OK,
	ACTION_LEFT,
	ACTION_RIGHT
} CallbackAction;

typedef struct Option
{
	const char *name;
	int (*callback)(struct OptionsMenu *parent_menu, size_t this_option, CallbackAction action);
	void *user_data;
	char *value_string;
	long value;
	BOOL disabled;
} Option;

typedef struct OptionsMenu
{
	const char *title;
	const char *subtitle;
	struct Option *options;
	size_t total_options;
	int x_offset;
	BOOL submenu;
} OptionsMenu;

static BOOL restart_required;

RECT rect_cur = {112, 88, 128, 104};

static char* GetKeyName(int key)
{
	
	switch (key)
	{
 		case KEY_A:        return "A";   break;
 		case KEY_B      :   return "B";  break;
 		case KEY_SELECT:     return "Select";     break;
 		case KEY_START:      return "Start";  break;
 		case KEY_RIGHT:      return "Right";    break;
 		case KEY_LEFT :         return "Left";  break;
 		case KEY_UP   :          return "Up";  break;
 		case KEY_DOWN :   return "Down";   break;
 		case KEY_R     :   return "R"; break;
 		case KEY_L     :  return "L";  break;
 		case KEY_X     :   return "X"; break;
 		case KEY_Y     :   return "Y"; break;
 		case KEY_TOUCH :   return "Touch"; break;
	}

	return "Unknown";
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

	for (;;)
	{
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
					else if (gKeyTrg & gKeyLeft)
						action = ACTION_LEFT;
					else if (gKeyTrg & gKeyRight)
						action = ACTION_RIGHT;
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
			PutText(&grcGame, (WINDOW_WIDTH / 2) - ((strlen(options_menu->subtitle) * 5) / 2), y + 14, options_menu->subtitle, RGB(0xFF, 0xFF, 0xFF));

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

		if (!Flip_SystemTask())
		{
			// Quit if window is closed
			return_value = CALLBACK_EXIT;
			break;
		}
	}

	// Deinitialise options
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
	{"Next Weapon",     BINDING_ARMS,    1 << 0},
	{"Previous Weapon", BINDING_ARMSREV, 1 << 0},
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
			strncpy(bound_name_buffers[this_option], GetKeyName(bindings[controls[this_option].binding_index].keyboard), sizeof(bound_name_buffers[0]) - 1);
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
					if(timer++>30 && keys && !setTimer)
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
								key_failed_id = other_option;
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
				strncpy(bound_name_buffers[current_control], GetKeyName(tempBindings[current_control-1].keyboard), sizeof(bound_name_buffers[0]) - 1);
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

static int Callback_Music(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = 1;
			//this needs to be a different value than the Soundtest otherwise they will just optimize into the same var..
			parent_menu->options[this_option].value_string = "1";
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
					parent_menu->options[this_option].value = 41;
			}
			else if (action == ACTION_RIGHT)
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > 41)
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

static int Callback_Sound(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	CONFIG *conf = (CONFIG*)parent_menu->options[this_option].user_data;

	switch (action)
	{
		case ACTION_INIT:
			parent_menu->options[this_option].value = 0;
			parent_menu->options[this_option].value_string = "0";
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
					parent_menu->options[this_option].value = 159;
			}
			else if (action == ACTION_RIGHT)
			{
				// Increment value (with wrapping)
				if (++parent_menu->options[this_option].value > 159)
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

static int Callback_Options(OptionsMenu *parent_menu, size_t this_option, CallbackAction action)
{
	(void)parent_menu;

	if (action != ACTION_OK)
		return CALLBACK_CONTINUE;

	// Make the options match the configuration data
	CONFIG conf;
	if (!LoadConfigData(&conf))
		DefaultConfigData(&conf);

	BOOL is_console = false;

	Option options_console[] = {

	};

	Option options_pc[] = {
		{"Control Config", Callback_ControlsKeyboard, NULL, NULL, 0, FALSE},
		{"Display screen", Callback_Screen, &conf, NULL, 0, FALSE},
		{"Cheat Mode", Callback_Cheat, &conf, NULL, 0, FALSE},
		{"Play song", Callback_Music, NULL, NULL, 0, FALSE},
		{"Play sound", Callback_Sound, NULL, NULL, 0, FALSE},
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

	PlaySoundObject(5, SOUND_MODE_PLAY);

	// Save our changes to the configuration file
	//memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
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
		{"Control Config", Callback_ControlsKeyboard, NULL, NULL, 0, FALSE},
		{"Display screen", Callback_Screen, &conf, NULL, 0, FALSE},
		{"Cheat Mode", Callback_Cheat, &conf, NULL, 0, FALSE},
		{"Play song", Callback_Music, NULL, NULL, 0, FALSE},
		{"Play sound", Callback_Sound, NULL, NULL, 0, FALSE},
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
	//memcpy(conf.bindings, bindings, sizeof(bindings));

	SaveConfigData(&conf);

	return return_value;
}
