#pragma once

#include "WindowsWrapper.h"
#include "nds.h"

extern RECT grcGame;
extern RECT grcFull;

extern int magnification;
extern BOOL fullscreen;

static const u8 font_space[32*3] = {
    2,2,4,8,6,8,7,2,4,4,8,6,3,6,2,4,6,3,6,6,7,6,6,6,6,6,2,3,4,8,4,5,
    8,6,6,6,6,5,5,6,6,2,5,6,5,6,6,6,6,6,6,5,6,6,6,6,6,6,5,4,4,4,4,6,
    3,6,6,6,6,6,5,6,6,2,4,5,3,8,6,6,6,6,5,5,5,6,6,6,6,6,6,5,2,5,6,4,
};

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
void CopyFaceTexture(int face);
void makeNpcSymTakeArmsSlot(bool toggle);
void DrawBitmapSizeParam(RECT *rcView, int x, int y, int width, int height, RECT *rect, SurfaceID surf_no, bool transparent);
void PutBitmap3(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no);
void PutBitmap4(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no);
void Surface2Surface(int x, int y, RECT *rect, int to, int from);
void Surface2Texture(int x, int y, RECT *rect, int to, int from);
unsigned long GetCortBoxColor(unsigned long col);
void CortBox(RECT *rect, unsigned long col);
void CortBox2(RECT *rect, unsigned long col, SurfaceID surf_no);
int RestoreSurfaces(void);
void InitTextObject();
int GetTextSpacing(const char *text);
void PutText(RECT* rcView, int x, int y, const char *text, unsigned long color);
void PutText2(int x, int y, const char *text, unsigned long color, SurfaceID surf_no);
void EndTextObject(void);
BOOL CopyDataToTexture(int paletteType, int textureid, int surf_no,  int xoffset, int yoffset, RECT* rect);

void glBegin2D( void );