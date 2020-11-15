#pragma once

#include "WindowsWrapper.h"

void PutConsole(void);
void InitConsole(void);

struct DebugVars
{
	bool bEnabled;
	bool bGodmode;
	bool bNoclip;
	bool bFastForward;
	int FastForwardTimer;

	int FrameAdvanceCount;
	bool bFrameFreeze;
	bool bFrameCanAdvance;

	bool bShowHitboxes;
};

extern DebugVars gDebug;
