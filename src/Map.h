#pragma once

#include "WindowsWrapper.h"

typedef struct MAP_DATA
{
	unsigned char *data;
	unsigned char atrb[0x100];
	short width;
	short length;
} MAP_DATA;

extern MAP_DATA gMap;

BOOL InitMapData2(void);
BOOL LoadMapData2(const char *path_map);
BOOL LoadAttributeData(const char *path_atrb);
void EndMapData(void);
void ReleasePartsImage(void);
void GetMapData(unsigned char **data, short *mw, short *ml);
__attribute__((hot))
static unsigned char GetAttribute(int x, int y)
{
	size_t a;

	//if (x < 0 || y < 0 || x >= gMap.width || y >= gMap.length)
	//	return 0;

	a = *(gMap.data + x + (y * gMap.width));	// Yes, the original code really does do this instead of a regular array access
	return gMap.atrb[a];
}
void DeleteMapParts(int x, int y);
void ShiftMapParts(int x, int y);
BOOL ChangeMapParts(int x, int y, unsigned char no);
void PutStage_Back(int fx, int fy);
void PutStage_Front(int fx, int fy);
void PutMapDataVector(int fx, int fy);
