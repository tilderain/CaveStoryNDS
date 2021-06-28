// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code beints to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

enum KeyBind
{
	//The movement keys go in the order of left, right, up and down
	CEY_LEFT =      0x00000001,
	CEY_RIGHT =     0x00000002,
	CEY_UP =        0x00000004,
	CEY_DOWN =      0x00000008,
	//Map key
	CEY_MAP =       0x00000010,
	//Okay and cancel / Jump and Shoot keys
	CEY_X =         0x00000020,
	CEY_Z =         0x00000040,
	//Left and right weapon switch keys
	CEY_ARMS =      0x00000080,
	CEY_ARMSREV =   0x00000100,
	//Inventory
	CEY_ITEM =      0x00000200,
	CEY_ESCAPE =	0x00000400,
};

extern int gKey;
extern int gKeyTrg;
extern int gKeyP1;
extern int gKeyTrgP1;
extern int gKeyP2;
extern int gKeyTrgP2;

extern int gKeyJump;
extern int gKeyShot;
extern int gKeyArms;
extern int gKeyArmsRev;
extern int gKeyItem;
extern int gKeyMap;

extern int gKeyOk;
extern int gKeyCancel;

extern int gKeyLeft;
extern int gKeyUp;
extern int gKeyRight;
extern int gKeyDown;

void GetTrg(void);
