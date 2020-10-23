// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"

void InitFade(void);
void SetFadeMask(void);
void ClearFade(void);
void StartFadeOut(signed char dir);
void StartFadeIn(signed char dir);
void ProcFade(void);
void PutFade(void);
BOOL GetFadeActive(void);
