#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WindowsWrapper.h"

#include "CommonDefines.h"
#include "Draw.h"
#include "Main.h"
#include "Input.h"
#include "Tags.h"

#include "nds.h"
#include <maxmod9.h>

#include "malloc.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 

#include "lodepng.h"

#include "gl2d.h"

#define COLOR(r,g,b)  ((r) | (g)<<5 | (b)<<10)

RECT grcGame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
RECT grcFull = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SURFACE surf[SURFACE_ID_MAX];

PrintConsole bottomScreen;


static unsigned int nextPowerOf2(unsigned int n)  
{  
    unsigned power = 1; 
    for(;;)
	{
		if(n <= power) return power;
		power <<= 1;
	}
}  

//Draw to screen
BOOL Flip_SystemTask()
{
	//Update inputs
	UpdateInput();
	
	mmStreamUpdate();

	glEnd2D();

	glFlush(0);
	swiWaitForVBlank();

	glBegin2D();
	
	return TRUE;
}

BOOL StartDirectDraw()
{
	
	//-----------------------------------------------------------------
	// Initialize the graphics engines
	//-----------------------------------------------------------------
	videoSetMode( MODE_5_3D );


    vramSetBankA( VRAM_A_TEXTURE );     
	vramSetBankB( VRAM_B_TEXTURE );
	
	vramSetBankD( VRAM_D_TEXTURE );
	
	vramSetBankE(VRAM_E_TEX_PALETTE);  // Allocate VRAM bank for all the palettes
	glScreen2D();
	glEnable(GL_TEXTURE_2D);

	glBegin2D();
	
	return TRUE;
}

void EndDirectDraw()
{
	//TODO
}

void ReleaseSurface(SurfaceID s)
{
	free(surf[s].data);
}

BOOL MakeSurface_Generic(int bxsize, int bysize, SurfaceID surf_no)
{
	if (surf_no < SURFACE_ID_MAX)
	{
		free(surf[surf_no].data);

		surf[surf_no].w = nextPowerOf2(bxsize);
		surf[surf_no].h = nextPowerOf2(bysize);
		surf[surf_no].data = (BUFFER_PIXEL*)malloc(nextPowerOf2(bxsize) * nextPowerOf2(bysize) * sizeof(BUFFER_PIXEL));
		surf[surf_no].palette = (u16*)malloc(256*(sizeof(u16)));
	}

	return TRUE;
}



BOOL LoadBitmap(FILE *fp, SurfaceID surf_no, bool create_surface)
{
	struct stat file_descriptor;
	long file_size;
	
	fstat(fileno(fp), &file_descriptor);
	file_size = file_descriptor.st_size;

	unsigned char *file_buffer = (unsigned char*)malloc(file_size);
	fread(file_buffer, file_size, 1, fp);
	
	unsigned int bitmap_width, bitmap_height;
	unsigned char *bitmap_pixels;

	LodePNGState state;
	lodepng_state_init(&state);

	state.decoder.color_convert = false;
	
	lodepng_decode(&bitmap_pixels, &bitmap_width, &bitmap_height, &state, file_buffer, file_size);

	printf("Size %d\n", state.info_png.color.palettesize);

	/*for (int i = 0; i < state.info_png.color.palettesize; i++)
	{
		printf("Color %d", state.info_png.color.palette[i*4]);
		printf(" %d", state.info_png.color.palette[i*4+1]);
		printf(" %d", state.info_png.color.palette[i*4+2]);
		printf(" %d\n", state.info_png.color.palette[i*4+3]);
	}*/
	
	free(file_buffer);

	if (create_surface)
	{
		MakeSurface_Generic(bitmap_width, bitmap_height, surf_no);
	}
		
	else
	{
		memset(surf[surf_no].data, 0, surf[surf_no].w * surf[surf_no].h * sizeof(BUFFER_PIXEL));
		glDeleteTextures(1, &surf[surf_no].textureid);
	}

	GL_TEXTURE_TYPE_ENUM paletteType;
	switch (state.info_png.color.palettesize)
	{
		case 2:
		case 4:
			paletteType = GL_RGB4;
			break;
		case 16:
			paletteType = GL_RGB16;
			break;
		case 256:
			paletteType = GL_RGB256;
			break;
		default:
			paletteType = GL_RGB16;
			printf("UHOHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
			break;
	}
	
	for (int y = 0; y < bitmap_height; y++)
	{
		for (int x = 0; x < bitmap_width; x++)
		{
			int pos;
			char color;
			pos = (y*bitmap_width) + x;

			switch(paletteType)
			{
				case GL_RGB4:
					color = (bitmap_pixels[pos*4] | bitmap_pixels[pos*4+1] << 2 | bitmap_pixels[pos*4+2] << 4 | bitmap_pixels[pos*4+3] << 6);
				break;
				case GL_RGB16:
					color = (bitmap_pixels[pos*2] | bitmap_pixels[pos*2+1] << 4);
				break;
				case GL_RGB256:
					color = (bitmap_pixels[pos]);
				break;
				case GL_RGB:
					uint8_t r,g,b;
						pos = (y*bitmap_width) + x;
						r = bitmap_pixels[pos*3] / 8;
						g = bitmap_pixels[pos*3+1] / 8;
						b = bitmap_pixels[pos*3+2] / 8;
					color = (bitmap_pixels[pos]);
				break;

			}

			surf[surf_no].data[ (y*surf[surf_no].w)+x ].color = color;
		}
	}
	
	int textureid;
	glGenTextures(1, &textureid);
	glBindTexture(0, textureid);

	int texW, texH;
	switch(surf[surf_no].w)
	{
		case 16: texW= TEXTURE_SIZE_16; break;
		case 32: texW= TEXTURE_SIZE_32; break;
		case 64: texW= TEXTURE_SIZE_64; break;
		case 128: texW= TEXTURE_SIZE_128; break;
		case 256: texW= TEXTURE_SIZE_256; break;
		case 512: texW= TEXTURE_SIZE_512; break;
		default: texW= TEXTURE_SIZE_256;
	}
	switch(surf[surf_no].h)
	{
		case 16: texH= TEXTURE_SIZE_16; break;
		case 32: texH= TEXTURE_SIZE_32; break;
		case 64: texH= TEXTURE_SIZE_64; break;
		case 128: texH= TEXTURE_SIZE_128; break;
		case 256: texH= TEXTURE_SIZE_256; break;
		case 512: texH= TEXTURE_SIZE_512; break;
		default: texH= TEXTURE_SIZE_256;
	}
	//if(surf_no == SURFACE_ID_ARMS_IMAGE || surf_no == SURFACE_ID_ITEM_IMAGE) paletteType = GL_RGB256;

	glTexImage2D(0,0, paletteType, texW, texH, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
		(u8*)surf[surf_no].data);

	u16* palette = (u16*)malloc(state.info_png.color.palettesize*2);
	for (int i = 0; i < state.info_png.color.palettesize; i++)
	{
		uint8_t r,g,b;
		r = state.info_png.color.palette[i*4] / 8;
		g = state.info_png.color.palette[i*4+1] / 8;
		b = state.info_png.color.palette[i*4+2] / 8;
		palette[i] = COLOR(r,g,b);
	}

	//TODO: palette length
	glColorTableEXT( 0, 0, 256, 0, 0, palette );

	//printf("Texture Id %d \n", textureid);

	surf[surf_no].palette = palette;
	surf[surf_no].paletteType = paletteType;
	surf[surf_no].palettesize = state.info_png.color.palettesize;
	surf[surf_no].textureid = textureid;

	free(bitmap_pixels);
	if(create_surface)
		free(surf[surf_no].data);
	fclose(fp);
	
	return TRUE;
}

BOOL LoadBitmap_File(const char *name, SurfaceID surf_no, bool create_surface)
{
	printf("LoadBitmap_File %s\n", name);
	printf("Memory: %d %d %d\n", mallinfo().arena, mallinfo().uordblks, mallinfo().fordblks);
	//Attempt to load PNG
	
	char path[MAX_PATH];
	sprintf(path, "%s/%s.png", gDataPath, name);
	
	FILE *fp = fopen(path, "rb");
	if (fp)
	{
		printf("Loading surface (as .png) from %s for surface id %d\n", path, surf_no);
		if (LoadBitmap(fp, surf_no, create_surface))
			return TRUE;
	}
	
	printf("Failed to open file %s\n", name);
	return FALSE;
}

BOOL LoadBitmap_Resource(const char *res, SurfaceID surf_no, bool create_surface)
{
	//TODO
	return TRUE;
}

BOOL MakeSurface_File(const char *name, SurfaceID surf_no)
{
	return LoadBitmap_File(name, surf_no, true);
}

BOOL MakeSurface_Resource(const char *res, SurfaceID surf_no)
{
	return LoadBitmap_Resource(res, surf_no, true);
}

BOOL ReloadBitmap_File(const char *name, SurfaceID surf_no)
{
	return LoadBitmap_File(name, surf_no, false);
}

BOOL ReloadBitmap_Resource(const char *res, SurfaceID surf_no)
{
	return LoadBitmap_Resource(res, surf_no, false);
}

void BackupSurface(SurfaceID surf_no, RECT *rect)
{
	//TODO:
	for (int fx = rect->left; fx < rect->right; fx++)
	{
		for (int fy = rect->top; fy < rect->bottom; fy++)
		{
			int dx = fx - rect->left;
			int dy = fy - rect->top;
			
			if (dx < 0 || dy < 0)
				continue;
			if (dx >= surf[surf_no].w || dy >= surf[surf_no].h)
				continue;
			
			//BUFFER_PIXEL *fromPixel = &screenBuffer[fy * WINDOW_WIDTH + fx];
			//SET_BUFFER_PIXEL(surf[surf_no].data, surf[surf_no].w, dx, dy, fromPixel->r, fromPixel->g, fromPixel->b);
		}
	}
}

static void DrawBitmap(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no, bool transparent)
{
	//TODO: draw queueing
	//TODO: don't render if transparent
	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	glSprite(x, y, rect, surf[surf_no].textureid, 0);
}

void PutBitmap3(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no) //Transparency
{
	DrawBitmap(rcView, x, y, rect, surf_no, true);
}

void PutBitmap4(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no) //No Transparency
{
	DrawBitmap(rcView, x, y, rect, surf_no, false);
}

void Surface2Surface(int x, int y, RECT *rect, int to, int from)
{
	//TODO
	if (surf[from].data && surf[to].data)
	{
		//Clip our rect
		RECT renderRect;
		renderRect.left = (x < 0) ? (rect->left + (0 - x)) : rect->left;
		renderRect.top = (y < 0) ? (rect->top + (0 - y)) : rect->top;
		renderRect.right = ((x + rect->right - rect->left) >= surf[to].w) ? rect->right - ((x + rect->right - rect->left) - surf[to].w) : rect->right;
		renderRect.bottom = ((y + rect->bottom - rect->top) >= surf[to].h) ? rect->bottom - ((y + rect->bottom - rect->top) - surf[to].h) : rect->bottom;
		
		for (int fx = renderRect.left; fx < renderRect.right; fx++)
		{
			for (int fy = renderRect.top; fy < renderRect.bottom; fy++)
			{
				int dx = x + (fx - rect->left);
				int dy = y + (fy - rect->top);
				
				//BUFFER_PIXEL *pixel = &surf[from].data[fy * surf[from].w + fx];
				//if (pixel->r == 0 && pixel->g == 0 && pixel->b == 0) //Surface2Surface is always color keyed
				//	continue;
				//SET_BUFFER_PIXEL(surf[to].data, surf[to].w, dx, dy, pixel->r, pixel->g, pixel->b);
			}
		}
	}
}

unsigned long GetCortBoxColor(unsigned long col)
{
	// TODO
	return col;
}

void CortBox(RECT *rect, uint32_t col)
{
	const unsigned char col_red = col & 0x0000FF;
	const unsigned char col_green = (col & 0x00FF00) >> 8;
	const unsigned char col_blue = (col & 0xFF0000) >> 16;
	//const BUFFER_PIXEL colPixel = {col_red, col_green, col_blue};
	
	for (int y = (rect->top < 0 ? 0 : rect->top); y < (rect->bottom >= WINDOW_HEIGHT ? WINDOW_HEIGHT : rect->bottom); y++)
	{
		//memcpy(screenBuffer, &colPixel, sizeof(BUFFER_PIXEL));
		for (int x = (rect->left < 0 ? 0 : rect->left); x < (rect->right >= WINDOW_WIDTH ? WINDOW_WIDTH : rect->right); x++)
		{
			//SET_BUFFER_PIXEL(screenBuffer, WINDOW_WIDTH, x, y, col_red, col_green, col_blue);
		}
	}
}

void CortBox2(RECT *rect, uint32_t col, SurfaceID surf_no)
{
	if (surf[surf_no].data)
	{
		const unsigned char col_red = col & 0x0000FF;
		const unsigned char col_green = (col & 0x00FF00) >> 8;
		const unsigned char col_blue = (col & 0xFF0000) >> 16;
		
		for (int y = (rect->top < 0 ? 0 : rect->top); y < (rect->bottom >= surf[surf_no].h ? surf[surf_no].h : rect->bottom); y++)
		{
			for (int x = (rect->left < 0 ? 0 : rect->left); x < (rect->right >= surf[surf_no].w ? surf[surf_no].w : rect->right); x++)
			{
				//SET_BUFFER_PIXEL(surf[surf_no].data, surf[surf_no].w, x, y, col_red, col_green, col_blue);
			}
		}
	}
}

void InitTextObject()
{
	MakeSurface_File("Font", SURFACE_ID_FONT);
}

void PutText(int x, int y, const char *text, uint32_t color)
{
	RECT rcCharacter;
	RECT *rcView = &grcFull;
	RECT *rect = &rcCharacter;
	RECT renderRect;
	
	//Get our surface colour
	int r = (color & 0xFF0000) >> 16;
	int g = (color & 0x00FF00) >>  8;
	int b = (color & 0x0000FF) >>  0;
	
	for (int i = 0; i < strlen(text); i++)
	{
		rcCharacter.left = (text[i] % 0x20) * 12;
		rcCharacter.top = (text[i] / 0x20 - 1) * 12;
		rcCharacter.right = rcCharacter.left + 12;
		rcCharacter.bottom = rcCharacter.top + 12;
		
		if (surf[SURFACE_ID_FONT].data)
		{
			//Clip our rect
			renderRect.left = (x < rcView->left) ? (rect->left + (rcView->left - x)) : rect->left;
			renderRect.top = (y < rcView->top) ? (rect->top + (rcView->top - y)) : rect->top;
			renderRect.right = ((x + rect->right - rect->left) >= rcView->right) ? rect->right - ((x + rect->right - rect->left) - rcView->right) : rect->right;
			renderRect.bottom = ((y + rect->bottom - rect->top) >= rcView->bottom) ? rect->bottom - ((y + rect->bottom - rect->top) - rcView->bottom) : rect->bottom;
			
			for (int fx = renderRect.left; fx < renderRect.right; fx++)
			{
				for (int fy = renderRect.top; fy < renderRect.bottom; fy++)
				{
					int dx = (x + 5 * i) + (fx - rect->left);
					int dy = y + (fy - rect->top);
					
					BUFFER_PIXEL *pixel = &surf[SURFACE_ID_FONT].data[fy * surf[SURFACE_ID_FONT].w + fx];
					//if (pixel->r == 0 && pixel->g == 0 && pixel->b == 0)
					//	continue;
					//SET_BUFFER_PIXEL(screenBuffer, WINDOW_WIDTH, dx, dy, r, g, b);
				}
			}
		}
	}
}

void PutText2(int x, int y, const char *text, uint32_t color, SurfaceID surf_no)
{
	RECT rcCharacter;
	RECT *rcView = &grcFull;
	RECT *rect = &rcCharacter;
	RECT renderRect;
	
	//Get our surface colour
	int r = (color & 0xFF0000) >> 16;
	int g = (color & 0x00FF00) >>  8;
	int b = (color & 0x0000FF) >>  0;
	
	for (int i = 0; i < strlen(text); i++)
	{
		rcCharacter.left = (text[i] % 0x20) * 12;
		rcCharacter.top = (text[i] / 0x20 - 1) * 12;
		rcCharacter.right = rcCharacter.left + 12;
		rcCharacter.bottom = rcCharacter.top + 12;
		
		if (surf[SURFACE_ID_FONT].data && surf[surf_no].data)
		{
			//Clip our rect
			renderRect.left = (x < 0) ? (rect->left + (0 - x)) : rect->left;
			renderRect.top = (y < 0) ? (rect->top + (0 - y)) : rect->top;
			renderRect.right = ((x + rect->right - rect->left) >= surf[surf_no].w) ? rect->right - ((x + rect->right - rect->left) - surf[surf_no].w) : rect->right;
			renderRect.bottom = ((y + rect->bottom - rect->top) >= surf[surf_no].h) ? rect->bottom - ((y + rect->bottom - rect->top) - surf[surf_no].h) : rect->bottom;
			
			for (int fx = renderRect.left; fx < renderRect.right; fx++)
			{
				for (int fy = renderRect.top; fy < renderRect.bottom; fy++)
				{
					int dx = (x + 5 * i) + (fx - rect->left);
					int dy = y + (fy - rect->top);
					
					BUFFER_PIXEL *pixel = &surf[SURFACE_ID_FONT].data[fy * surf[SURFACE_ID_FONT].w + fx];
					//if (pixel->r == 0 && pixel->g == 0 && pixel->b == 0) //Surface2Surface is always color keyed
					//	continue;
					//SET_BUFFER_PIXEL(surf[surf_no].data, surf[surf_no].w, dx, dy, r, g, b);
				}
			}
		}
	}
}

void EndTextObject()
{
	ReleaseSurface(SURFACE_ID_FONT);
}
