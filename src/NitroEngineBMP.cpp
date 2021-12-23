// SPDX-License-Identifier: MIT
//
// Copyright (c) 2008-2011, 2019, Antonio Niño Díaz
//
// This file is part of Nitro Engine


#include "NitroEngineBMP.h"
#include "File.h"

#include "stdlib.h"

#include "nds.h"

BOOL LoadPalettedBMP(void* file_buffer, SurfaceID surf_no, bool create_surface)
{

	NE_BMPHeader *header = (NE_BMPHeader*)file_buffer;
	NE_BMPInfoHeader *infoheader = (NE_BMPInfoHeader *)((u8 *)header +
						sizeof(NE_BMPHeader));
	if (header->type != 0x4D42) {
		printf("Not a BMP file");
		return FALSE;
	}

	int sizex = infoheader->width;
	int sizey = infoheader->height;
	if (sizex > 1024 || sizey > 1024) {
		printf("BMP file too big (%d, %d)", sizex, sizey);
		return FALSE;
	}

	if (infoheader->compression != 0) {
		printf("Compressed BMP not supported");
		return FALSE;
	}

	if (infoheader->bits != 8 && infoheader->bits != 4 && infoheader->bits != 1) {
		printf("Unsupported depth for GL_RGB256 conversion (%d)",
			      infoheader->bits);
		return FALSE;
	}

	// Decode
	int colornumber = 0;
	if(infoheader->bits == 8)
		colornumber = 256;
	else if(infoheader->bits == 4)
		colornumber = 16;
	else
		colornumber = 2;

	printf("Size %d W: %d H: %d\n", colornumber, sizex, sizey);

	u8 *PALETTEDATA = (u8 *)infoheader + sizeof(NE_BMPInfoHeader);
	u8 *IMAGEDATA = (u8 *)header + header->offset;

	// First, we read the palette

	surf[surf_no].palette = (u16*)malloc(colornumber*2);

	int i = 0;
	signed short firstBlank = -1;
	bool swap0WithFirstBlank = false;
	while (i < colornumber) {
		u8 r, g, b;
		g = PALETTEDATA[(i << 2) + 1] & 0xFF;
		r = PALETTEDATA[(i << 2) + 2] & 0xFF;
		b = PALETTEDATA[(i << 2) + 0] & 0xFF;
		surf[surf_no].palette[i] = RGB15(r >> 3, g >> 3, b >> 3);
		if(firstBlank == -1 && surf[surf_no].palette[i] == 0)
			firstBlank = i;
		i++;
	}
	// if color 0 is a color, manually swap it with the nearest 00 00 00
	if(surf[surf_no].palette[0] != 0)
	{
		if(firstBlank != -1)
		{
			swap0WithFirstBlank = true;
			surf[surf_no].palette[firstBlank] = surf[surf_no].palette[0];
			surf[surf_no].palette[0] = 0;
		}
	}

	surf[surf_no].w = sizex; surf[surf_no].h = sizey;
	
	if (create_surface)
	{
		MakeSurface_Generic(sizex, sizey, surf_no);
	}
	u8 *buffer = NULL;
	if(colornumber <= 16)
		buffer = (u8*)malloc(sizex * sizey);

	surf[surf_no].data = (BUFFER_PIXEL*)malloc(sizex * sizey);

	GL_TEXTURE_TYPE_ENUM paletteType;
	switch (colornumber)
	{

		case 16:
			paletteType = GL_RGB16;
			break;
		case 256:
			paletteType = GL_RGB256;
			break;
		default:
			paletteType = GL_RGB16;
			break;
	}

	surf[surf_no].paletteType = paletteType;
	surf[surf_no].palettesize = colornumber;

	// Then, the image
	int y, x;
	if (colornumber == 256) {
		// For BMPs with width not multiple of 4
		int disalign = sizex & 3;

		if (disalign) {
			disalign = 4 - disalign;

			for (y = 0; y < sizey; y++) {
				for (x = 0; x < sizex; x++) {
					surf[surf_no].data[y * sizex + x].color =
					    IMAGEDATA[(sizex * (sizey - y - 1)) + x +
						      (((disalign) * (sizey - y - 1)) * 1)];
					if(swap0WithFirstBlank)
					{
						char color = surf[surf_no].data[y * sizex + x].color;
						if(color == firstBlank)
						{
							surf[surf_no].data[y * sizex + x].color = 0;
						}
						else if(color == 0)
						{
							surf[surf_no].data[y * sizex + x].color = firstBlank;
						}

					}

				}
			}
		} else {
			for (y = 0; y < sizey; y++) {
				for (x = 0; x < sizex; x++) {
					surf[surf_no].data[y * sizex + x].color =
					    IMAGEDATA[(sizex * (sizey - y - 1)) + x];
					if(swap0WithFirstBlank)
					{
						char color = surf[surf_no].data[y * sizex + x].color;
						if(color == firstBlank)
						{
							surf[surf_no].data[y * sizex + x].color = 0;
						}
						else if(color == 0)
						{
							surf[surf_no].data[y * sizex + x].color = firstBlank;
						}
					}
				}
			}
		}
	} else if (colornumber == 16) { //colornumber == 16
		// For BMPs with width not multiple of 8
		int disalign = sizex & 7;

		if (disalign) {
			disalign = 8 - disalign;
			for (y = 0; y < sizey; y++) {
				for (x = 0; x < sizex; x++) {
					u32 value;
					u32 srcidx = ((sizex * (sizey - y - 1) + x) + (disalign * (sizey - y - 1))) >> 1;

					if (x & 1) {
						value = IMAGEDATA[srcidx] & 0x0F;
					} else {
						value = (IMAGEDATA[srcidx] >> 4) & 0x0F;
					}

					buffer[y * sizex + x] = value;
				}
			}
		} else {
			for (y = 0; y < sizey; y++) {
				for (x = 0; x < sizex; x++) {
					u32 value;
					u32 srcidx = (sizex * (sizey - y - 1) + x) >> 1;
					if (x & 1) {
						value = IMAGEDATA[srcidx] & 0x0F;
					} else {
						value = (IMAGEDATA[srcidx] >> 4) & 0x0F;
					}
					buffer[y * sizex + x] = value;
				}
			}
		}
	} else //colornumber == 2
	{
		int linesize = ((sizex + 31) / 32) * 4;

		for(int y = sizey - 1; y >= 0; y--)
    		{
        	for(int x = 0; x < sizex; x++)
        	{
        	    int pos = y * linesize + x / 8;
        	    int bit = 1 << (7 - x % 8);
        	    int v = (IMAGEDATA[pos] & bit) > 0;
        	    //printf("%d", v);
				
				buffer[(sizey - y - 1) * (sizex) + (x)] = v;
        	}
        //printf("end\n");
    }

	}

	//finally convert the 256 color to 16 color
	if(colornumber <= 16)
	{
		for (int y = 0; y < sizey; y++)
		{
			for (int x = 0; x < sizex; x++)
			{
				int pos;
				char color;
				pos = (y*sizex) + x;
				char col1 = buffer[pos*2];
				char col2 = buffer[pos*2+1];

				if(swap0WithFirstBlank)
				{
					if(col1 == 0)
						col1 = firstBlank;
					else if(col1 == firstBlank)
						col1 = 0;
					if(col2 == 0)
						col2 = firstBlank;
					else if(col2 == firstBlank)
						col2 = 0;
					
				}
				color = (col1 | col2 << 4);
				surf[surf_no].data[ pos ].color = color;
			}
		}
		free(buffer);
	}


	
	return TRUE;

}