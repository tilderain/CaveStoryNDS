#pragma once

#include "WindowsWrapper.h"

struct JOYSTICK_STATUS
{
	BOOL bLeft;
	BOOL bRight;
	BOOL bUp;
	BOOL bDown;
	BOOL bButton[32];
};

void ReleaseDirectInput(void);
BOOL InitDirectInput(void);
BOOL GetJoystickStatus(JOYSTICK_STATUS *status);
BOOL ResetJoystickStatus(void);

bool UpdateInput();