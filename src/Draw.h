// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include "WindowsWrapper.h"
#include "nds.h"
#include "gl2d.h"
#include "CommonDefines.h"

extern RECT grcGame;
extern RECT grcFull;

extern int magnification;
extern BOOL fullscreen;

extern int gFaceNo;
extern int gFaceNoCurrent;

extern bool npcSymInArmsSlot;

extern char gConsoleInited;

extern int gAtlas16Color1;
extern int gAtlas16Color2;


void ErrorInitConsole();

#ifndef JAPANESE
static const u8 font_space[32*3] = {
    3,2,4,8,6,8,7,2,4,4,8,6,3,6,2,4,6,3,6,6,7,6,6,6,6,6,2,3,4,8,4,5,
    8,6,6,6,6,5,5,6,6,2,5,6,5,6,6,6,6,6,6,5,6,6,6,6,6,6,5,4,4,4,4,6,
    3,6,6,6,6,6,5,6,6,2,4,5,3,8,6,6,6,6,5,5,5,6,6,6,6,6,6,5,2,5,6,4,
};
#else
static const u8 font_space[32*35] = {

3,2,5,7,6,7,7,3,4,4,6,6,2,6,2,7,5,3,5,5,5,5,5,5,5,5,2,2,6,6,6,6,
8,7,7,8,7,6,6,8,7,4,6,6,5,8,7,8,6,8,8,7,8,7,9,10,8,8,7,4,6,4,6,5,
2,6,6,6,6,6,4,6,6,2,3,6,2,8,6,6,6,6,4,6,4,6,6,8,6,6,5,4,2,4,11,2,
8,5,6,6,6,6,9,9,8,9,6,9,10,4,7,9,11,13,7,10,6,9,9,9,3,6,3,3,3,3,3,3,
10,11,11,9,8,7,10,9,10,10,11,11,8,11,11,9,10,7,8,9,11,11,11,11,10,9,11,8,8,10,10,8,
10,10,11,11,10,8,9,10,11,10,4,11,11,10,11,11,13,6,10,10,10,10,10,8,11,8,11,10,10,8,9,9,
9,8,11,10,9,14,9,9,10,11,11,9,9,10,10,9,10,11,8,3,3,3,3,3,3,3,3,3,3,3,3,3,
10,10,11,10,10,11,11,10,8,10,11,11,9,9,10,8,11,11,9,9,11,10,6,10,11,10,12,11,11,11,9,9,
11,12,9,10,7,11,11,9,11,11,11,11,11,11,12,11,9,11,9,11,11,10,10,11,11,10,10,11,10,11,9,10,
11,9,10,10,11,8,9,11,11,11,10,10,11,13,11,5,4,4,8,11,7,13,7,12,13,8,13,3,3,3,3,3,
13,13,13,13,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,13,13,13,13,13,13,13,
13,12,13,13,13,13,13,13,13,13,13,13,13,13,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,11,13,13,13,13,13,13,13,13,12,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,12,13,13,13,13,13,13,13,13,13,13,13,






};
#endif

typedef enum SurfaceID
{
	SURFACE_ID_TITLE = 0,
	SURFACE_ID_PIXEL = 1,
	SURFACE_ID_LEVEL_TILESET = 2,
	SURFACE_ID_FADE = 6,
	SURFACE_ID_ITEM_IMAGE = 8,
	SURFACE_ID_MAP = 9,
	SURFACE_ID_SCREEN_GRAB = 10,
	SURFACE_ID_ARMS = 11,
	SURFACE_ID_ARMS_IMAGE = 12,
	SURFACE_ID_ROOM_NAME = 13,
	SURFACE_ID_STAGE_ITEM = 14,
	SURFACE_ID_LOADING = 15,
	SURFACE_ID_MY_CHAR = 16,
	SURFACE_ID_BULLET = 17,
	SURFACE_ID_CARET = 19,
	SURFACE_ID_NPC_SYM = 20,
	SURFACE_ID_LEVEL_SPRITESET_1 = 21,
	SURFACE_ID_LEVEL_SPRITESET_2 = 22,
	SURFACE_ID_NPC_REGU = 23,
	SURFACE_ID_TEXT_BOX = 26,
	SURFACE_ID_FACE = 27,
	SURFACE_ID_LEVEL_BACKGROUND = 28,
	SURFACE_ID_VALUE_VIEW = 29,
	SURFACE_ID_TEXT_LINE1 = 30,
	SURFACE_ID_TEXT_LINE2 = 31,
	SURFACE_ID_TEXT_LINE3 = 32,
	SURFACE_ID_TEXT_LINE4 = 33,
	SURFACE_ID_TEXT_LINE5 = 34,
	SURFACE_ID_CREDIT_CAST = 35,
	SURFACE_ID_CREDITS_IMAGE = 36,
	SURFACE_ID_CASTS = 37,
	SURFACE_ID_FONT = 40,
	SURFACE_ID_MAX = 41
} SurfaceID;

struct BUFFER_PIXEL
{
	char color;
};

struct SURFACE
{
	int w;
	int h;
	int textureid;
	int xoffset;
	int yoffset;
	int paletteOffset; //offset from palette vram banks
	int paletteAddress; //real address of palette in memory
	GL_TEXTURE_TYPE_ENUM paletteType;
	int palettesize;
	BUFFER_PIXEL *data;
	u16* palette;
	char name[16];
};

extern SURFACE surf[SURFACE_ID_MAX];

#define PutBitmap4 PutBitmap3

BOOL Flip_SystemTask(void);
BOOL StartDirectDraw();
void EndDirectDraw(void);
void ReleaseSurface(SurfaceID s);
BOOL MakeSurface_Resource(const char *name, SurfaceID surf_no);
BOOL MakeSurface_File(const char *name, SurfaceID surf_no);
BOOL ReloadBitmap_Resource(const char *name, SurfaceID surf_no);
BOOL ReloadBitmap_File(const char *name, SurfaceID surf_no);
BOOL MakeSurface_Generic(int bxsize, int bysize, SurfaceID surf_no);
void BackupSurface(SurfaceID surf_no, RECT *rect);
void CopyFaceTexture();
void CopyDirtyText();
void makeNpcSymTakeArmsSlot(bool toggle);
void PutBitmap3Transparent(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no, u32 alpha);
void DrawBitmapSizeParam(RECT *rcView, int x, int y, int width, int height, RECT *rect, SurfaceID surf_no, bool transparent);
static void PutBitmap3(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no)
{
	//TODO: draw queueing
	//TODO: don't render if transparent
#ifdef TWO_SCREENS
	int temp = x;
	x = WINDOW_HEIGHT - y;
	y = temp;

	if((gCounter & 1) == 0) // bottom screen
	{
		
	}
	else
	{
		y -= WINDOW_WIDTH / 2;
	}

#endif

	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	int textureid = surf[surf_no].textureid;
	//if(!surf[surf_no].textureid) {textureid = gAtlas16Color1;}
////
	RECT srcRect = *rect;

	srcRect.top += surf[surf_no].yoffset;
	srcRect.bottom += surf[surf_no].yoffset;

	if(surf[surf_no].paletteType == GL_RGB16)
	{
		srcRect.left += surf[surf_no].xoffset;
		srcRect.right += surf[surf_no].xoffset;
	}
	else
	{
		// To compensate for the texture size being halved when turning from 16 to 256 color
		srcRect.left += surf[surf_no].xoffset / 2;
		srcRect.right += surf[surf_no].xoffset / 2;
	}

	if(rcView->left > x)
	{
		srcRect.left += rcView->left - x;
		x = rcView->left;
	}

	if(rcView->top > y)
	{
		srcRect.top += rcView->top - y;
		y = rcView->top;
	}

	int width = srcRect.right - srcRect.left;
	int height = srcRect.bottom - srcRect.top;
	if(x + width > rcView->right)
	{
		srcRect.right -= x + width - rcView->right;
	}

	if(y + height > rcView->bottom)
	{
		srcRect.bottom -= y + height - rcView->bottom;
	}
////

	//glSprite(x, y, rect, gAtlas16Color1, 0);
	glSprite(x, y, &srcRect, surf[surf_no].textureid, surf[surf_no].paletteOffset, surf[surf_no].paletteType);
}
void PutBitmap4(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no);
int GetSurfPixel(int x, int y, int surf_no);
void SetSurf2Pixels(int x, int y, int surf_no, char color);
void Surface2Surface(int x, int y, RECT *rect, int to, int from);
void Surface2Texture(int x, int y, RECT *rect, int to, int from);
unsigned long GetCortBoxColor(unsigned long col);
void CortBox(RECT *rect, unsigned long col);
void CortBoxAlpha(RECT *rect, uint32_t col, u32 alpha);
void CortBox2(RECT *rect, unsigned long col, SurfaceID surf_no);
int RestoreSurfaces(void);
void InitTextObject();
int GetTextSpacing(const char *text);
void PutText(RECT* rcView, int x, int y, const char *text, unsigned long color);
void PutText2(int x, int y, const char *text, unsigned long color, SurfaceID surf_no);
void EndTextObject(void);
BOOL CopyDataToTexture(int paletteType, int textureid, int surf_no,  int xoffset, int yoffset, RECT* rect);

void glBegin2D( void );