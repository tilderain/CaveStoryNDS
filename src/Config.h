// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

#include "Input.h"

enum
{
	BINDING_UP,
	BINDING_DOWN,
	BINDING_LEFT,
	BINDING_RIGHT,
	BINDING_JUMP,
	BINDING_SHOT,
	BINDING_ARMSREV,
	BINDING_ARMS,
	BINDING_ITEM,
	BINDING_MAP,
	BINDING_TOTAL,
	BINDING_OK,
	BINDING_CANCEL,
	BINDING_PAUSE,
};

typedef struct CONFIG_BINDING
{
	unsigned int keyboard;
} CONFIG_BINDING;

struct CONFIG
{
	char proof[0x20];
	BOOL bBottomScreen;
	BOOL bFps;
	BOOL bDebug;
	CONFIG_BINDING bindings[BINDING_TOTAL];
};

extern const char* const gConfigName;
extern const char* const gProof;

BOOL LoadConfigData(CONFIG *conf);
BOOL SaveConfigData(const CONFIG *conf);
void DefaultConfigData(CONFIG *conf);
