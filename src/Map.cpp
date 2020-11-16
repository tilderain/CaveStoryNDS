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

			if (atrb >= 0x20)
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

			if (atrb < 0x40 || atrb >= 0x80)
				continue;

			// Draw tile
			rect.left = (gMap.data[offset] % 16) * 16;
			rect.top = (gMap.data[offset] / 16) * 16;
			rect.right = rect.left + 16;
			rect.bottom = rect.top + 16;

			PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rect, SURFACE_ID_LEVEL_TILESET);

			if (atrb == 0x43)
				PutBitmap3(&grcGame, ((i * 16) - 8) - (fx / 0x200), ((j * 16) - 8) - (fy / 0x200), &rcSnack, SURFACE_ID_NPC_SYM);
		}
	}
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
