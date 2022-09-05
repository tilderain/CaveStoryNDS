// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

void GetCompileDate(int *year, int *month, int *day);
BOOL GetCompileVersion(int *v1, int *v2, int *v3, int *v4);
void DeleteDebugLog(void);
BOOL PrintDebugLog(const char *string, int value1, int value2, int value3);
BOOL CheckFileExists(const char *name);
long GetFileSizeLong(FILE* fp);
BOOL PrintBitmapError(const char *string, int value);
BOOL IsShiftJIS(unsigned char c);
BOOL IsEnableBitmap(const char *path);
