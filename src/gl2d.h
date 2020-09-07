#pragma once

#include "WindowsWrapper.h"
#include "nds.h"
#include "Draw.h"

void glScreen2D( void );
void SetOrtho( void );

void glBegin2D( void );
void glEnd2D( void );
static inline void gxTexcoord2i(t16 u, t16 v);
static inline void gxVertex3i(v16 x, v16 y, v16 z);
static inline void gxVertex2i(v16 x, v16 y);
void glSprite( int x1, int y1, RECT *rect, int textureID, int paletteOffset);
void glBoxFilled( int x1, int y1, int x2, int y2, int color );