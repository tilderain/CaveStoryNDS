// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "KeyControl.h"

#include "stdio.h"

int gKey;
int gKeyTrg;

int gKeyP1;
int gKeyTrgP1;
int gKeyP2;
int gKeyTrgP2;

int gKeyJump = CEY_Z;
int gKeyShot = CEY_X;
int gKeyArms = CEY_ARMS;
int gKeyArmsRev = CEY_ARMSREV;
int gKeyItem = CEY_ITEM;
int gKeyMap = CEY_MAP;

int gKeyOk = CEY_Z;
int gKeyCancel = CEY_X;

int gKeyLeft = CEY_LEFT;
int gKeyUp = CEY_UP;
int gKeyRight = CEY_RIGHT;
int gKeyDown = CEY_DOWN;

static void GetTrgOther(void)
{
	static int CEY_old_other;
	gKeyTrgP2 = gKeyP2 ^ CEY_old_other;
	gKeyTrgP2 = gKeyP2 & gKeyTrgP2;
	CEY_old_other = gKeyP2;
}

void GetTrg(void)
{
	static int CEY_old;
	gKeyTrg = gKey ^ CEY_old;
	gKeyTrg = gKey & gKeyTrg;
	CEY_old = gKey;

	GetTrgOther();
}

