// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "KeyControl.h"

long gKey;
long gKeyTrg;

long gKeyJump = CEY_Z;
long gKeyShot = CEY_X;
long gKeyArms = CEY_ARMS;
long gKeyArmsRev = CEY_ARMSREV;
long gKeyItem = CEY_ITEM;
long gKeyMap = CEY_MAP;

long gKeyOk = CEY_Z;
long gKeyCancel = CEY_X;

long gKeyLeft = CEY_LEFT;
long gKeyUp = CEY_UP;
long gKeyRight = CEY_RIGHT;
long gKeyDown = CEY_DOWN;

void GetTrg(void)
{
	static int CEY_old;
	gKeyTrg = gKey ^ CEY_old;
	gKeyTrg = gKey & gKeyTrg;
	CEY_old = gKey;
}
