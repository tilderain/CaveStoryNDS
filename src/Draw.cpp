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

#define REG_DISPCNT_MAIN  (*(vu32*)0x04000000)
#define REG_DISPCNT_SUB   (*(vu32*)0x04001000)
#define MODE_FB0          (0x00020000)
#define VRAM_A            ((u16*)0x6800000)
#define VRAM_A_CR         (*(vu8*)0x04000240)
#define VRAM_ENABLE       (1<<7)
#define SCANLINECOUNTER   *(vu16 *)0x4000006

#define COLOR(r,g,b)  ((r) | (g)<<5 | (b)<<10)
#define PIXEL_ENABLE (1<<15)
#define OFFSET(r,c,w) ((r)*(w)+(c))

RECT grcGame = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
RECT grcFull = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

SURFACE surf[SURFACE_ID_MAX];



void clearScreen()
{
	memset(VRAM_A, 0, sizeof(u16)*WINDOW_WIDTH*WINDOW_HEIGHT);
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

	//clearScreen();

	//oamUpdate(&oamMain);

	//oamClear(&oamMain, 0, 0);
	
	//Write to framebuffer
	/*uint32_t *pointer = xfb[currentFramebuffer];
	
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
	{
		for (unsigned int x = 0; x < WINDOW_WIDTH; x += 2)
		{
			YUVColour colour1 = RGBToYUV(screenBuffer[y * WINDOW_WIDTH + x].r, screenBuffer[y * WINDOW_WIDTH + x].g, screenBuffer[y * WINDOW_WIDTH + x].b);
			YUVColour colour2 = RGBToYUV(screenBuffer[y * WINDOW_WIDTH + x + 1].r, screenBuffer[y * WINDOW_WIDTH + x + 1].g, screenBuffer[y * WINDOW_WIDTH + x + 1].b);
			*pointer++ = (colour1.y << 24) | (((colour1.cb + colour2.cb) >> 1) << 16) | (colour2.y << 8) | ((colour1.cr + colour2.cr) >> 1);
		}
		
		memcpy(pointer, pointer - (WINDOW_WIDTH / 2), (WINDOW_WIDTH / 2) * sizeof(*pointer));
		pointer += WINDOW_WIDTH / 2;
	}
	
	//Flush screen to our television and wait for next frame
	VIDEO_SetNextFramebuffer(xfb[currentFramebuffer]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	
	//Swap frame-buffer
	currentFramebuffer = !currentFramebuffer;*/
	glBegin2D();
	
	return TRUE;
}
PrintConsole bottomScreen;

//Initialize video and frame-buffers

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
	



	//oamInit(&oamMain, SpriteMapping_1D_32, false);
	//oamInit(&oamSub, SpriteMapping_1D_128, false);
	
	//Initialize printing screen
	//consoleInit(&bottomScreen, 3,BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	//consoleSelect(&bottomScreen);
	
	//dmaCopy(manPal, SPRITE_PALETTE, 512);
	
	//sprite_gfx_mem = oamAllocateGfx(&oamMain, SpriteSize_32x32, SpriteColorFormat_256Color);
	/*
	curGfx = 0;
	for(int i = 0; i < gfxPtrCount; i++)
	{
		u16* gfx = oamAllocateGfx(&oamMain, SpriteSize_16x16, SpriteColorFormat_256Color);
		for(int i = 0; i < 16 * 16 / 2; i++)
		{
			gfx[i] = 1 | (1 << 8);
		}
		gfxPtrs[i] = gfx;
	}

	SPRITE_PALETTE[1] = RGB15(31,0,0);*/

	return TRUE;
}

void EndDirectDraw()
{
	//Free our screenbuffer
	//free(screenBuffer);
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
		surf[surf_no].w = bxsize;
		surf[surf_no].h = bysize;
		surf[surf_no].data = (BUFFER_PIXEL*)malloc(bxsize * bysize * sizeof(BUFFER_PIXEL));
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
	
	lodepng_decode24(&bitmap_pixels, &bitmap_width, &bitmap_height, file_buffer, file_size);

	//printf("Type %d\n", state.info_png.color.colortype);
	//printf("Size %d\n", state.info_png.color.palettesize);
	
	//for (int i = 0; i < state.info_png.color.palettesize; i++)
	//{
	//	printf("Color %d", state.info_png.color.palette[i*4]);
	//	printf(" %d", state.info_png.color.palette[i*4+1]);
	//	printf(" %d", state.info_png.color.palette[i*4+2]);
	//	printf(" %d\n", state.info_png.color.palette[i*4+3]);
	//}
	
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
	
	int pos;
	//printf("Size %d\n", sizeof(bitmap_pixels) / sizeof(bitmap_pixels)[0]);

	for (int y = 0; y < bitmap_height; y++)
	{
		for (int x = 0; x < bitmap_width; x++)
		{
			uint8_t r,g,b;
			pos = (y*bitmap_width) + x;
			r = bitmap_pixels[pos*3] / 8;
			g = bitmap_pixels[pos*3+1] / 8;
			b = bitmap_pixels[pos*3+2] / 8;

			//if(pos >= bitmap_width * bitmap_height) break;
			surf[surf_no].data[pos].color = COLOR(r, g, b);

			
		}
	}
	
	int textureid;
	
	glGenTextures(1, &textureid);
	glBindTexture(0, textureid);

	int texW, texH;
	switch(bitmap_width)
	{
		case 16: texW= TEXTURE_SIZE_16; break;
		case 32: texW= TEXTURE_SIZE_32; break;
		case 64: texW= TEXTURE_SIZE_64; break;
		case 128: texW= TEXTURE_SIZE_128; break;
		case 256: texW= TEXTURE_SIZE_256; break;
		case 512: texW= TEXTURE_SIZE_512; break;
		default: texW= TEXTURE_SIZE_64;
	}
	switch(bitmap_height)
	{
		case 16: texH= TEXTURE_SIZE_16; break;
		case 32: texH= TEXTURE_SIZE_32; break;
		case 64: texH= TEXTURE_SIZE_64; break;
		case 128: texH= TEXTURE_SIZE_128; break;
		case 256: texH= TEXTURE_SIZE_256; break;
		case 512: texH= TEXTURE_SIZE_512; break;
		default: texH= TEXTURE_SIZE_64;
	}

	glTexImage2D(0,0, GL_RGB, texW, texH, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF,
		(u8*)surf[surf_no].data);
	surf[surf_no].textureid = textureid;

	glColorTableEXT( 0, 0, 0, 0, 0, 0 );

	printf("Texture Id %d \n", textureid);
	/*
	for (int x = 0; x < bitmap_height; x++)
	{
		for (int y = 0; y < bitmap_height; y++)
		{
			if (x >= surf[surf_no].w || y >= surf[surf_no].h)
				continue;
			RGBApixel pixel = bitmap_pixels[x*y];
			SET_BUFFER_PIXEL(surf[surf_no].data, surf[surf_no].w, x, y, pixel.Red, pixel.Green, pixel.Blue);
		}
	}*/

	//dmaCopy(bitmap_pixels, sprite_gfx_mem, 32*32);
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
	/*
	SDL_RWops *fp = FindResource(res);
	
	if (fp)
	{
		printf("Loading surface from resource %s for surface id %d\n", res, surf_no);
		if (LoadBitmap(fp, surf_no, create_surface))
			return TRUE;
	}
	
	printf("Failed to open resource %s\n", res);
	return FALSE;
	*/
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


void setPixel(int row, int col, u16 color) {
    VRAM_A[OFFSET(row, col, WINDOW_WIDTH)] = color;
}

void setRow(int row, int x, int rectx, int recty, int width, SURFACE surf) {
	memcpy(&VRAM_A[OFFSET(row, x, WINDOW_WIDTH)], &surf.data[(recty * surf.w) + rectx], (2*width));
}

static void DrawBitmap(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no, bool transparent)
{
	//if(curGfx) return;

	//if(surf_no != SURFACE_ID_MY_CHAR) return;

	//if(surf_no == SURFACE_ID_LEVEL_BACKGROUND) return;
	//if(surf_no == SURFACE_ID_LEVEL_TILESET) return;
	//if(surf_no == SURFACE_ID_FADE) return;

	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	//int rectW = surf[surf_no].w;
	//int rectH = surf[surf_no].h;

	//rect->left = 0;
	//rect->top = 0;

	glSprite(x, y, rect, surf[surf_no].textureid, 0);

	/*for (int yy = 0; yy < rectH; yy++)
	{
		//int rectx = rect->left;
		//int recty = rect->top + yy;

		//printf("%d ", rectx);

		//int pos = (recty * surf[surf_no].w);
		
		//printf("%d ",pos+rectx);

		//uint16_t col = surf[surf_no].data[pos + rectx].color;
		//if(col == 0) continue;
		setRow(y + yy, x, rect->left, rect->top + yy, rectW, surf[surf_no]);
	}*/


	//curGfx++;
	
	/*
	if (surf[surf_no].data)
	{
		
		
		//Clip our rect
		RECT renderRect;
		renderRect.left = (x < rcView->left) ? (rect->left + (rcView->left - x)) : rect->left;
		renderRect.top = (y < rcView->top) ? (rect->top + (rcView->top - y)) : rect->top;
		renderRect.right = ((x + rect->right - rect->left) >= rcView->right) ? rect->right - ((x + rect->right - rect->left) - rcView->right) : rect->right;
		renderRect.bottom = ((y + rect->bottom - rect->top) >= rcView->bottom) ? rect->bottom - ((y + rect->bottom - rect->top) - rcView->bottom) : rect->bottom;
		
		//Clip our draw position
		for (int fx = renderRect.left; fx < renderRect.right; fx++)
		{
			for (int fy = renderRect.top; fy < renderRect.bottom; fy++)
			{
				int dx = x + (fx - rect->left);
				int dy = y + (fy - rect->top);
				
				BUFFER_PIXEL *pixel = &surf[surf_no].data[fy * surf[surf_no].w + fx];
				if (transparent && pixel->r == 0 && pixel->g == 0 && pixel->b == 0)
					continue;
				
				//SET_BUFFER_PIXEL(screenBuffer, WINDOW_WIDTH, dx, dy, pixel->r, pixel->g, pixel->b);
			}
		}
	}*/
	
	/*
	if(curGfx >= gfxPtrCount) return;

	if(x > WINDOW_WIDTH || x < -32) return;
	if(y > WINDOW_HEIGHT || y < -32) return;

	if(surf_no == SURFACE_ID_LEVEL_BACKGROUND) return;
	if(surf_no == SURFACE_ID_LEVEL_TILESET) return;
	if(surf_no == SURFACE_ID_FADE) return;
	if(surf_no == SURFACE_ID_TEXT_BOX) return;
	if(surf_no == SURFACE_ID_FACE) return;

	oamSet(&oamMain, 
			curGfx, 
			x, 
			y, 
			0, 
			0,
			SpriteSize_16x16, 
			SpriteColorFormat_256Color, 
			gfxPtrs[curGfx], 
			-1, 
			false, 
			false,			
			false, false, 
			false	
			);              
			
	curGfx++;*/
	//oamSet(&oamMain, 0, x, y, 0, 0, SpriteSize_32x32, SpriteColorFormat_256Color, 
	//		sprite_gfx_mem, -1, false, false, false, false, false);
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
	// This comes in BGR, and goes out BGR
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
