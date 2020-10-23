// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

typedef struct BACK
{
	BOOL flag;	// Basically unused
	int partsW;
	int partsH;
	int numX;
	int numY;
	int type;
	int fx;
} BACK;

extern BACK gBack;
extern int gWaterY;

BOOL InitBack(const char *fName, int type);
void ActBack(void);
void PutBack(int fx, int fy);
void PutFront(int fx, int fy);
