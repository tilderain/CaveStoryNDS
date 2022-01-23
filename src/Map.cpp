// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "Map.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "File.h"
#include "Main.h"
#include "NpChar.h"

#include "fopen.h"

#include "KeyControl.h"

#define PXM_BUFFER_SIZE 0x9000

MAP_DATA gMap;

const char *code_pxma = "PXM";

BOOL InitMapData2(void)
{
	gMap.data = (unsigned char*)malloc(PXM_BUFFER_SIZE);
	return TRUE;
}

BOOL LoadMapData2(const char *path_map)
{
	FILE_e *fp;
	char check[3];
	char path[MAX_PATH];

	// Get path
	sprintf(path, "%s/%s", gDataPath, path_map);

	// Open file
	fp = fopen_embed(path, "rb");
	if (fp == NULL)
		return FALSE;

	// Make sure file begins with "PXM"
	fread_embed(check, 1, 3, fp);

	if (memcmp(check, code_pxma, 3))
	{
		fclose_embed(fp);
		return FALSE;
	}

	unsigned char dum;
	fread_embed(&dum, 1, 1, fp);
	// Get width and height
	gMap.width = File_ReadLE16(fp);
	gMap.length = File_ReadLE16(fp);

	if (gMap.data == NULL)
	{
		fclose_embed(fp);
		return FALSE;
	}

	// Read tile data
	fread_embed(gMap.data, 1, gMap.width * gMap.length, fp);
	fclose_embed(fp);
	return TRUE;
}

BOOL LoadAttributeData(const char *path_atrb)
{
	FILE_e *fp;
	char path[MAX_PATH];

	// Open file
	sprintf(path, "%s/%s", gDataPath, path_atrb);

	fp = fopen_embed(path, "rb");
	if (fp == NULL)
		return FALSE;

	// Read data
	fread_embed(gMap.atrb, 1, sizeof(gMap.atrb), fp);
	fclose_embed(fp);
	return TRUE;
}

void EndMapData(void)
{
	free(gMap.data);
}

void ReleasePartsImage(void)
{
	ReleaseSurface(SURFACE_ID_LEVEL_TILESET);
}

void GetMapData(unsigned char **data, short *mw, short *ml)
{
	if (data != NULL)
		*data = gMap.data;

	if (mw != NULL)
		*mw = gMap.width;

	if (ml != NULL)
		*ml = gMap.length;
}

void DeleteMapParts(int x, int y)
{
	*(gMap.data + x + (y * gMap.width)) = 0;
}

void ShiftMapParts(int x, int y)
{
	*(gMap.data + x + (y * gMap.width)) -= 1;
}

BOOL ChangeMapParts(int x, int y, unsigned char no)
{
	int i;

	if (*(gMap.data + x + (y * gMap.width)) == no)
		return FALSE;

	*(gMap.data + x + (y * gMap.width)) = no;

	for (i = 0; i < 3; ++i)
		SetNpChar(4, x * 0x200 * 0x10, y * 0x200 * 0x10, 0, 0, 0, NULL, 0);

	return TRUE;
}

__attribute__((hot))
void PutStage_Back(int fx, int fy)
{
	//int i, j;
	RECT rect;
	//int offset;

	// Get range to draw
	int num_x = ((WINDOW_WIDTH + (16 - 1)) / 16) + 1;
	int num_y = ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1;
	int put_x = ((fx / 0x200) + 8) / 16;
	int put_y = ((fy / 0x200) + 8) / 16;

	//int atrb;

	for (int j = put_y; j < put_y + num_y; ++j)
	{
		for (int i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			int offset = (j * gMap.width) + i;
			int atrb = GetAttribute(i, j);

			if (atrb >= 0x20 || atrb == 0)
				continue;

			// Draw tile
			rect.left = (gMap.data[offset] % 16) * 16;
			rect.top = (gMap.data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, SURFACE_ID_LEVEL_TILESET);
		}
	}
}

__attribute__((hot))
void PutStage_Front(int fx, int fy)
{
	RECT rcSnack = {256, 48, 272, 64};
	//int i, j;
	RECT rect;
	//int offset;

	// Get range to draw
	//int num_x = ((WINDOW_WIDTH + (16 - 1)) / 16) + 1;
	//int num_y = ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1;
	int scroll_x = ((fx / 0x200) + 8);
	int scroll_y = ((fy / 0x200) + 8);

	int put_x = ((fx / 0x200) + 8) / 16;
	int put_y = ((fy / 0x200) + 8) / 16;

	int atrb;

	const int bgWidth = 256;

	TileMapEntry16 entry = {0};
	u16* ptr = bgGetMapPtr(gBackground0);
	u16* ptr_sub = bgGetMapPtr(gBackground0_sub);

	videoBgDisableSub(3);

	//ErrorInitConsole();

	//calculate range of tiles to place in 1st half of bg0 based off of put_x and scroll
	//scroll_x is in pixels

	int bg0_0_x = abs((bgWidth - (scroll_x % bgWidth)) / 16); 
	int bg0_0_y, bg0_1_y;

	int bg0_1_x = abs((bgWidth / 16) - bg0_0_x);
	if(scroll_y % bgWidth - bgWidth > 0)
	{
		bg0_0_y = SCREEN_HEIGHT / 16;
		bg0_1_y = 0;
	}
	else
	{
		bg0_0_y = abs((bgWidth - (scroll_y % bgWidth)) / 16);
		bg0_1_y = abs((bgWidth / 16) - bg0_0_y);
	}

	int startx = (scroll_x%bgWidth)/16;
	int starty = (scroll_y%bgWidth)/16;

	bool startOnSecondHalf = false;
	if(scroll_x % 512 >= 256) startOnSecondHalf = true;
	//topleft corner
	for(int y=starty; y < starty + bg0_0_y; y++)
	{
		for(int x=startx; x < startx + bg0_0_x; x++)
		{
				int offset = ((put_y + y - starty) * gMap.width) + (put_x + x - startx);
				int atrb = GetAttribute(x, y);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;

				int tileX = (gMap.data[offset] % 16);
				int tileY = (gMap.data[offset] / 16);

				entry.index = (tileX * 2) + (tileY*32*2);
				int gfxIndex = x*2 + (y * 32 * 2);

				if(startOnSecondHalf) gfxIndex += 0x400;
				ptr[gfxIndex] = entry.index;
				ptr[gfxIndex+1] = entry.index + 1;
				ptr[gfxIndex + 32] = entry.index + 32;
				ptr[gfxIndex+1 + 32] = entry.index + 32 +1;

				offset = ((put_y + y - starty + 12) * gMap.width) + (put_x + x - startx);

				atrb = GetAttribute(x, y + 12);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;
				tileX = (gMap.data[offset] % 16);
				tileY = (gMap.data[offset] / 16);
				entry.index = (tileX * 2) + (tileY*32*2);

				ptr_sub[gfxIndex] = entry.index;
				ptr_sub[gfxIndex+1] = entry.index + 1;
				ptr_sub[gfxIndex + 32] = entry.index + 32;
				ptr_sub[gfxIndex+1 + 32] = entry.index + 32 +1;
		}
	}
	//topright corner
	for(int y=starty; y < starty + bg0_0_y; y++)
	{
		for(int x=0; x < bg0_1_x; x++)
		{
				int offset = ((put_y + y - starty) * gMap.width) + (put_x + x + (16 - startx));
				int atrb = GetAttribute(x, y);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;

				int tileX = (gMap.data[offset] % 16);
				int tileY = (gMap.data[offset] / 16);

				entry.index = (tileX * 2) + (tileY*32*2);
				int gfxIndex = x*2 + (y * 32 * 2);

				if(!startOnSecondHalf) gfxIndex += 0x400;
				ptr[gfxIndex] = entry.index;
				ptr[gfxIndex+1] = entry.index + 1;
				ptr[gfxIndex + 32] = entry.index + 32;
				ptr[gfxIndex+1 + 32] = entry.index + 32 +1;

				offset = ((put_y + y - starty + 12) * gMap.width) + (put_x + x + (16 - startx));

				atrb = GetAttribute(x, y + 12);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;
				tileX = (gMap.data[offset] % 16);
				tileY = (gMap.data[offset] / 16);
				entry.index = (tileX * 2) + (tileY*32*2);

				ptr_sub[gfxIndex] = entry.index;
				ptr_sub[gfxIndex+1] = entry.index + 1;
				ptr_sub[gfxIndex + 32] = entry.index + 32;
				ptr_sub[gfxIndex+1 + 32] = entry.index + 32 +1;
		}
	}
	//ErrorInitConsole();
	//printf("sX:%d sY:%d x0:%d x1:%d y0:%d y1:%d\n", startx, starty, bg0_0_x, bg0_1_x, bg0_0_y, bg0_1_y);
	

	//bottom right corner
	for(int y=0; y < bg0_1_y; y++)
	{
		for(int x=0; x < bg0_1_x; x++)
		{
				int offset = ((put_y + y + (16 - starty)) * gMap.width) + (put_x + x - startx + 16);
				int atrb = GetAttribute(x, y);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;

				int tileX = (gMap.data[offset] % 16);
				int tileY = (gMap.data[offset] / 16);

				entry.index = (tileX * 2) + (tileY*32*2);

				int gfxIndex = x*2 + (y * 32 * 2) ;

				if(!startOnSecondHalf) gfxIndex += 0x400;

				ptr[gfxIndex ] = entry.index;
				ptr[gfxIndex+1 ] = entry.index + 1;
				ptr[gfxIndex + 32 ] = entry.index + 32;
				ptr[gfxIndex+1 + 32 ] = entry.index + 32 +1;

				offset = ((put_y + y + 12 + (16 - starty)) * gMap.width) + (put_x + x - startx + 16);

				atrb = GetAttribute(x, y + 12);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;
				tileX = (gMap.data[offset] % 16);
				tileY = (gMap.data[offset] / 16);
				entry.index = (tileX * 2) + (tileY*32*2);

				ptr_sub[gfxIndex ] = entry.index;
				ptr_sub[gfxIndex+1 ] = entry.index + 1;
				ptr_sub[gfxIndex + 32 ] = entry.index + 32;
				ptr_sub[gfxIndex+1 + 32 ] = entry.index + 32 +1;
		}
	}
	//bottom left corner

	for(int y=0; y < bg0_1_y; y++)
	{
		for(int x=startx; x < startx + bg0_0_x; x++)
		{
				int offset = ((put_y + y - starty + 16) * gMap.width) + (put_x + x - startx);
				int atrb = GetAttribute(x, y);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;

				int tileX = (gMap.data[offset] % 16);
				int tileY = (gMap.data[offset] / 16);

				entry.index = (tileX * 2) + (tileY*32*2);

				int gfxIndex = x*2 + (y * 32 * 2) ;

				if(startOnSecondHalf) gfxIndex += 0x400;

				ptr[gfxIndex ] = entry.index;
				ptr[gfxIndex+1 ] = entry.index + 1;
				ptr[gfxIndex + 32 ] = entry.index + 32;
				ptr[gfxIndex+1 + 32 ] = entry.index + 32 +1;

				offset = ((put_y + y + 12 - starty + 16) * gMap.width) + (put_x + x - startx);

				atrb = GetAttribute(x, y + 12);

				if (atrb < 0x40 || atrb >= 0x80)
					continue;
				tileX = (gMap.data[offset] % 16);
				tileY = (gMap.data[offset] / 16);
				entry.index = (tileX * 2) + (tileY*32*2);

				ptr_sub[gfxIndex ] = entry.index;
				ptr_sub[gfxIndex+1 ] = entry.index + 1;
				ptr_sub[gfxIndex + 32 ] = entry.index + 32;
				ptr_sub[gfxIndex+1 + 32 ] = entry.index + 32 +1;
		}
	}

	/*for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			int offset = (y * gMap.width) + x;
			int atrb = GetAttribute(x, y);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;

			int tileX = (gMap.data[offset] % 16);
			int tileY = (gMap.data[offset] / 16);

			entry.index = (tileX * 2) + (tileY*32*2);

			ptr[x*2 + (y * 32 * 2)] = entry.index;
			ptr[x*2 + (y * 32 * 2)+1] = entry.index + 1;
			ptr[x*2 + (y * 32 * 2) + 32] = entry.index + 32;
			ptr[x*2 + (y * 32 * 2)+1 + 32] = entry.index + 32 +1;

			offset = ((y + 12) * gMap.width) + x;

			atrb = GetAttribute(x, y + 12);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;
			tileX = (gMap.data[offset] % 16);
			tileY = (gMap.data[offset] / 16);
			entry.index = (tileX * 2) + (tileY*32*2);

			ptr_sub[x*2 + (y * 32 * 2)] = entry.index;
			ptr_sub[x*2 + (y * 32 * 2)+1] = entry.index + 1;
			ptr_sub[x*2 + (y * 32 * 2) + 32] = entry.index + 32;
			ptr_sub[x*2 + (y * 32 * 2)+1 + 32] = entry.index + 32 +1;
		}
	}
	for(int y = 0; y < 16; y++)
	{
		for(int x = 0; x < 16; x++)
		{
			int offset = (y * gMap.width) + x + 16;
			int atrb = GetAttribute(x, y);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;

			int tileX = (gMap.data[offset] % 16);
			int tileY = (gMap.data[offset] / 16);

			entry.index = (tileX * 2) + (tileY*32*2);

			ptr[x*2 + (y * 32 * 2) + 0x400] = entry.index;
			ptr[x*2 + (y * 32 * 2)+1 + 0x400] = entry.index + 1;
			ptr[x*2 + (y * 32 * 2) + 32 + 0x400 ] = entry.index + 32;
			ptr[x*2 + (y * 32 * 2)+1 + 32 + 0x400] = entry.index + 32 +1;

			offset = ((y + 12) * gMap.width) + x + 16;

			atrb = GetAttribute(x, y + 12);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;
			tileX = (gMap.data[offset] % 16);
			tileY = (gMap.data[offset] / 16);
			entry.index = (tileX * 2) + (tileY*32*2);

			ptr_sub[x*2 + (y * 32 * 2) + 0x400] = entry.index;
			ptr_sub[x*2 + (y * 32 * 2)+1 + 0x400] = entry.index + 1;
			ptr_sub[x*2 + (y * 32 * 2) + 32 + 0x400]  = entry.index + 32;
			ptr_sub[x*2 + (y * 32 * 2)+1 + 32 + 0x400] = entry.index + 32 +1;
		}
	}*/
	/*for (int j = put_y; j < put_y + num_y; ++j)
	{
		for (int i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			int offset = (j * gMap.width) + i;
			int atrb = GetAttribute(i, j);

			if (atrb < 0x40 || atrb >= 0x80)
				continue;

			// Draw tile
			rect.left = (gMap.data[offset] % 16) * 16;
			rect.top = (gMap.data[offset] / 16) * 16;
			//rect.right = rect.left + 16;
			//rect.bottom = rect.top + 16;

			//PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, SURFACE_ID_LEVEL_TILESET);

			//if (atrb == 0x43)
			//	PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rcSnack, SURFACE_ID_NPC_SYM);
			entry.index = rect.left * 2 + (rect.top * 32);
			int x = ((i) - 1) - (fx / (0x200 * 8));
			int y = ((j) - 1) - (fy / (0x200 * 8));
			ptr[x/2 + (y*32)] = entry.index;
			ptr[x/2 + (y*32) + 1] = entry.index + 1;
			ptr_sub[x * y] = entry.index;
		}
	}*/

	REG_BG1HOFS = scroll_x;
	REG_BG1VOFS = scroll_y;
	REG_BG1HOFS_SUB = scroll_x;
	REG_BG1VOFS_SUB = scroll_y;
}

void PutMapDataVector(int fx, int fy)
{
	int i, j;
	RECT rect;
	int offset;

	int num_x;
	int num_y;
	int put_x;
	int put_y;

	static unsigned char count = 0;

	int atrb;

	// Animate the wind
	count += 2;

	// Get range to draw
	num_x = ((WINDOW_WIDTH + (16 - 1)) / 16) + 1;
	num_y = ((WINDOW_HEIGHT + (16 - 1)) / 16) + 1;
	put_x = ((fx / 0x200) + 8) / 16;
	put_y = ((fy / 0x200) + 8) / 16;

	for (j = put_y; j < put_y + num_y; ++j)
	{
		for (i = put_x; i < put_x + num_x; ++i)
		{
			// Get attribute
			offset = (j * gMap.width) + i;
			atrb = GetAttribute(i, j);

			if (atrb != 0x80
				&& atrb != 0x81
				&& atrb != 0x82
				&& atrb != 0x83
				&& atrb != 0xA0
				&& atrb != 0xA1
				&& atrb != 0xA2
				&& atrb != 0xA3)
				continue;

			switch (atrb)
			{
				case 128:
				case 160:
					rect.left = 224 + (count % 16);
					rect.right = rect.left + 16;
					rect.top = 48;
					rect.bottom = rect.top + 16;
					break;

				case 129:
				case 161:
					rect.left = 224;
					rect.right = rect.left + 16;
					rect.top = 48 + (count % 16);
					rect.bottom = rect.top + 16;
					break;

				case 130:
				case 162:
					rect.left = 240 - (count % 16);
					rect.right = rect.left + 16;
					rect.top = 48;
					rect.bottom = rect.top + 16;
					break;

				case 131:
				case 163:
					rect.left = 224;
					rect.right = rect.left + 16;
					rect.top = 64 - (count % 16);
					rect.bottom = rect.top + 16;
					break;
			}

			PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, SURFACE_ID_CARET);
		}
	}
}
