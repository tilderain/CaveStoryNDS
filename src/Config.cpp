// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "Config.h"
#include "File.h"
#include "Main.h"

#include "nds.h"

const char *config_filename = "Config.dat";	// Not the original name
const char *config_magic = "CSDS01";	// Not the original name

BOOL LoadConfigData(CONFIG *conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(CONFIG));

	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s/%s", gModulePath, config_filename);

	printf("Loading config %s\n", path);

	// Open file
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
	{
		printf("failed to load config\n");
		return FALSE;
	}
		

	// Read the version id and font name
	fread(conf->proof, sizeof(conf->proof), 1, fp);

	fread(&conf->bBottomScreen, 1, 1, fp);

	fread(&conf->bDebug, 1, 1, fp);

	for (size_t i = 0; i < BINDING_TOTAL; ++i)
		conf->bindings[i].keyboard = File_ReadLE32(fp);

	// Close file
	fclose(fp);

	// Check if version is not correct, and return if it failed
	if (strcmp(conf->proof, config_magic))
	{
		memset(conf, 0, sizeof(CONFIG));
		return FALSE;
	}

	return TRUE;
}


BOOL SaveConfigData(const CONFIG *conf)
{
	// Get path
	char path[MAX_PATH];
	sprintf(path, "%s/%s", gModulePath, config_filename);

	printf("Saving config %s\n", path);
	// Open file
	FILE *fp = fopen(path, "wb");
	if (fp == NULL)
	{
		printf("failed to save config\n");
		return FALSE;
	}


	// Write the version id and font name
	fwrite(conf->proof, sizeof(conf->proof), 1, fp);

	fputc(conf->bBottomScreen, fp);
	fputc(conf->bDebug, fp);


	// Write key-bindings
	for (size_t i = 0; i < BINDING_TOTAL; ++i)
	{
		File_WriteLE32(conf->bindings[i].keyboard, fp);
	}

	// Close file
	fclose(fp);

	return TRUE;
}

void DefaultConfigData(CONFIG *conf)
{
	// Clear old configuration data
	memset(conf, 0, sizeof(CONFIG));

	// Fun fact: The Linux port added this line:
	// conf->display_mode = 1;

	strcpy(conf->proof, config_magic);

	// Fun fact: The Linux port added this line:
	// conf->display_mode = 1;

	// Set default key bindings
	conf->bindings[BINDING_UP].keyboard = KEY_UP;
	conf->bindings[BINDING_DOWN].keyboard = KEY_DOWN;
	conf->bindings[BINDING_LEFT].keyboard = KEY_LEFT;
	conf->bindings[BINDING_RIGHT].keyboard = KEY_RIGHT;
	conf->bindings[BINDING_OK].keyboard = KEY_A;
	conf->bindings[BINDING_CANCEL].keyboard = KEY_B;
	conf->bindings[BINDING_JUMP].keyboard = KEY_A;
	conf->bindings[BINDING_SHOT].keyboard = KEY_B;
	conf->bindings[BINDING_ARMSREV].keyboard = KEY_Y;
	conf->bindings[BINDING_ARMS].keyboard = KEY_X;
	conf->bindings[BINDING_ITEM].keyboard = KEY_START;
	conf->bindings[BINDING_MAP].keyboard = KEY_SELECT;
}
