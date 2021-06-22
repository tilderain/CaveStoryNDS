#pragma once

#include "stddef.h"
#include "WindowsWrapper.h"

enum
{
	CALLBACK_CONTINUE = -1,
	CALLBACK_PREVIOUS_MENU = -2,
	CALLBACK_RESET = -3,
	CALLBACK_EXIT = -4,
};

enum
{
	NETPLAY_START_NONE = 0,
	NETPLAY_START_NORMAL = 1,
	NETPLAY_START_LOAD = 2,
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

int Call_Pause(void);
int Call_Multi(void);
int EnterOptionsMenuIngame(OptionsMenu *options_menu);

