// SPDX-License-Identifier: MIT
//
// Copyright (c) 2008-2011, 2019, Antonio Niño Díaz
//
// This file is part of Nitro Engine


#include "NitroEngineBMP.h"
#include "File.h"

#include "stdlib.h"

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

	if (infoheader->bits != 8 && infoheader->bits != 4) {
		printf("Unsupported depth for GL_RGB256 conversion (%d)",
			      infoheader->bits);
		return FALSE;
	}

	// Decode
	int colornumber = (infoheader->bits == 8) ? 256 : 16;
	u8 *PALETTEDATA = (u8 *)infoheader + sizeof(NE_BMPInfoHeader);
	u8 *IMAGEDATA = (u8 *)header + header->offset;

	// First, we read the palette

	surf[surf_no].palette = (u16*)malloc(colornumber*2);

	int i = 0;
	while (i < colornumber) {
		u8 r, g, b;
		g = PALETTEDATA[(i << 2) + 1] & 0xFF;
		r = PALETTEDATA[(i << 2) + 2] & 0xFF;
		b = PALETTEDATA[(i << 2) + 0] & 0xFF;
		surf[surf_no].palette[i] = RGB15(r >> 3, g >> 3, b >> 3);
		i++;
	}


	if (create_surface)
	{
		MakeSurface_Generic(sizex, sizey, surf_no);
	}


	surf[surf_no].data = (BUFFER_PIXEL*)malloc(sizex * sizey);

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
				}
			}
		} else {
			for (y = 0; y < sizey; y++) {
				for (x = 0; x < sizex; x++) {
					surf[surf_no].data[y * sizex + x].color =
					    IMAGEDATA[(sizex * (sizey - y - 1)) + x];
				}
			}
		}
	} else { //colornumber == 16
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

					surf[surf_no].data[y * sizex + x].color = value;
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
					surf[surf_no].data[y * sizex + x].color = value;
				}
			}
		}
	}



	return TRUE;

}