// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"
#include "Config.h"

extern char gModulePath[MAX_PATH];
extern char gDataPath[MAX_PATH];

extern BOOL bFullscreen;

extern BOOL gb50Fps;
extern BOOL gbTransparency;
extern BOOL gbAlternateDrams;

extern int gLoadingProgress;

extern CONFIG conf;
extern CONFIG_BINDING bindings[BINDING_TOTAL];

extern bool gIsCardPopped;
extern int gCardPopTimer;

extern bool gPxtoneInited;

#include "./pxtone/pxtnService.h"
extern pxtnService*   pxtn;

void PutFramePerSecound(void);
unsigned long GetFramePerSecound(void);

void PutLoadingProgress();

BOOL SystemTask(void);

bool _load_ptcop( pxtnService* pxtn, const char* path_src, pxtnERR* p_pxtn_err );
