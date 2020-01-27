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
	//Unused?
	CEY_SHIFT =     0x00000200,
	//Function keys
	CEY_F1 =        0x00000400,
	CEY_F2 =        0x00000800,
	//Inventory
	CEY_ITEM =      0x00001000,
	//Escape key
	CEY_ESCAPE =	0x00008000,
	//The alt movement keys go in the order of left, up, right and down
	CEY_ALT_LEFT =  0x00010000,
	CEY_ALT_DOWN =  0x00020000,
	CEY_ALT_RIGHT = 0x00040000,
	CEY_ALT_UP =    0x00180000,
	//Alt up actually has 2 seperate keys, but they're kind of merged together
	CEY_L =         0x00080000,
	CEY_PLUS =      0x00100000
};

extern long gKey;
extern long gKeyTrg;

extern long gKeyJump;
extern long gKeyShot;
extern long gKeyArms;
extern long gKeyArmsRev;
extern long gKeyItem;
extern long gKeyMap;

extern long gKeyOk;
extern long gKeyCancel;

extern long gKeyLeft;
extern long gKeyUp;
extern long gKeyRight;
extern long gKeyDown;

void GetTrg(void);
