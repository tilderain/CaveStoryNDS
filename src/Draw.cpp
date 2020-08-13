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

int gTempTextureId;
int gTextureLoaded = 0;

int gTextureWidth = TEXTURE_SIZE_1024;
int gTextureHeight = TEXTURE_SIZE_512;

void* gCurrentPalette;

static unsigned int nextPowerOf2(unsigned int n)  
{  
    unsigned power = 1; 
    for(;;)
	{
		if(n <= power) return power;
		power <<= 1;
	}
}  

uint16* vramGetBank(uint16 *addr) {
//---------------------------------------------------------------------------------
	if(addr >= VRAM_A && addr < VRAM_B)
		return VRAM_A;
	else if(addr >= VRAM_B && addr < VRAM_C)
		return VRAM_B;
	else if(addr >= VRAM_C && addr < VRAM_D)
		return VRAM_C;
	else if(addr >= VRAM_D && addr < VRAM_E)
		return VRAM_D;
	else if(addr >= VRAM_E && addr < VRAM_F)
		return VRAM_E;
	else if(addr >= VRAM_F && addr < VRAM_G)
		return VRAM_F;
	else if(addr >= VRAM_G && addr < VRAM_H)
		return VRAM_G;
	else if(addr >= VRAM_H && addr < VRAM_I)
		return VRAM_H;
	else return VRAM_I;
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

	glGenTextures(1, &gTempTextureId);
	glBindTexture(0, gTempTextureId);
	glTexImage2D(0,0, GL_RGB16, gTextureWidth, gTextureHeight, 0,
		GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
		NULL);
	
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

		surf[surf_no].w = bxsize;
		surf[surf_no].h = bysize;
		surf[surf_no].data = (BUFFER_PIXEL*)malloc(bxsize * bysize * sizeof(BUFFER_PIXEL));
		surf[surf_no].palette = (u16*)malloc(256*(sizeof(u16)));
	}

	return TRUE;
}

uint8* vramBlock_examineSpecial( s_vramBlock *mb, uint8 *addr, uint32 size, uint8 align ) {
	// Simple validity tests
	if( !addr || !mb->firstEmpty || !size || align >= 8 )
		return NULL;
	// Start with the first empty block
	struct s_SingleBlock *block = mb->firstEmpty;

	// Set these value to 0/NULL (should only be filled in with valid data in case of error), and copy the address to start checking
	mb->lastExamined = NULL;
	mb->lastExaminedAddr = NULL;
	mb->lastExaminedSize = 0;
	uint8 *checkAddr = addr;

	// If the address is within a valid block, examine if it will initially fit in it
	while( block && checkAddr >= block->AddrSet + block->blockSize )
		block = block->node[ 3 ];
	if( !block ) return NULL;

	// Move the address up if before the first valid block
	if( checkAddr < block->AddrSet )
		checkAddr = block->AddrSet;

	uint8* bankLock[ 5 ] = { 0x0 };
	uint32 bankSize[ 5 ] = { 0x0 };
	uint32 curBank = 0;

	// Values that hold which banks to examine
	uint32 isNotMainBank = ( checkAddr >= (uint8*)VRAM_E ? 1 : 0 );
	uint32 vramCtrl = ( isNotMainBank ? VRAM_EFG_CR : VRAM_CR );
	int vramLock = glGlob->vramLock[ isNotMainBank ];
	uint32 i = 0, iEnd = ( isNotMainBank ? 3 : 4 );

	// Fill in the array with only those banks that are not set for Textures or Texture Palettes
	for( ; i < iEnd; i++ ) {
		if((( vramCtrl & 0x83 ) != 0x83 ) || ( vramLock & 0x1 )) {		// VRAM_ENABLE | ( VRAM_x_TEXTURE | VRAM_x_TEX_PALETTE )
			if( isNotMainBank ) {
				bankLock[ curBank ] = ( i == 0 ? (uint8*)VRAM_E : (uint8*)VRAM_F + (( i - 1 ) * 0x4000 ));
				bankSize[ curBank ] = ( i == 0 ? 0x10000 : 0x4000 );
			} else {
				bankLock[ curBank ] = (uint8*)VRAM_A + ( i * 0x20000 );
				bankSize[ curBank ] = 0x20000;
			}
			curBank++;
		}
		vramCtrl >>= 8;
		vramLock >>= 1;
	}
	curBank = 0;

	// Retrieve the available area from this block using the address given
	uint32 curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet );
	do {
		// Do address adjustments based on locked banks
		if( bankLock[ curBank ] ) {
			// Skip to corresponding bank that address is in
			while( bankLock[ curBank ] && checkAddr >= ( bankLock[ curBank ] + bankSize[ curBank ] ))
				curBank++;
			do {
				// Examine is address is within locked bank, and push it to next bank if needed
				if( bankLock[ curBank ] && checkAddr >= bankLock[ curBank ] && checkAddr < bankLock[ curBank ] + bankSize[ curBank ] )
					checkAddr = bankLock[ curBank ] + bankSize[ curBank ];
				else
					break;
			} while( bankLock[ ++curBank ] != NULL );
			// Continue block and address adjustments
			while( block && checkAddr >= block->AddrSet + block->blockSize )
				block = block->node[ 3 ];
			if( !block )	return NULL;
			if( checkAddr < block->AddrSet )
				checkAddr = block->AddrSet;
			// Adjust the blocks available size based on address location within said block
			if( bankLock[ curBank ] && bankLock[ curBank ] < block->AddrSet + block->blockSize )
				curBlockSize = (uint32)bankLock[ curBank ] - (uint32)checkAddr;
			else
				curBlockSize = block->blockSize - ((uint32)checkAddr - (uint32)block->AddrSet );
		}

		// Obtained an aligned address, and adjust the available area that can be used
		uint8* aligned_checkAddr = (uint8*)(((uint32)checkAddr + ((1<<align)-1)) & (~((1<<align)-1)));
		uint32 excess = ((uint32)aligned_checkAddr - (uint32)checkAddr);
		curBlockSize -= excess;
		if( curBlockSize >= size ) {
			mb->lastExamined = block;
			mb->lastExaminedAddr = aligned_checkAddr;
			mb->lastExaminedSize = size;
			return aligned_checkAddr;
		} else {
			if( bankLock[ curBank ] && bankLock[ curBank ] < block->AddrSet + block->blockSize ) {
				checkAddr = bankLock[ curBank ] + bankSize[ curBank ];
				curBlockSize = 0;
			} else {
				block = block->node[ 3 ];
				if( !block )	return NULL;
				checkAddr = block->AddrSet;
				curBlockSize = block->blockSize;
			}
		}
	} while( block != NULL );
	return NULL;
}

uint8*
vramBlock__allocateBlock( s_vramBlock *mb, struct s_SingleBlock *block, uint8 *addr, uint32 size ) {
	// Initial tests to ensure allocation is valid
	if( !size || !addr || !block || block->indexOut || addr < block->AddrSet || ( addr + size ) > ( block->AddrSet + block->blockSize )) return 0;

	// Get pointers to the various blocks, as those may change from allocation
	struct s_SingleBlock **first = &mb->firstBlock, **alloc = &mb->firstAlloc, **empty = &mb->firstEmpty;
	// The nodes in the test block array will change as examinations of pre/post blocks are done
	struct s_SingleBlock *testBlock[ 4 ] = { block->node[ 0 ], block->node[ 1 ], block->node[ 2 ], block->node[ 3 ] };

	// Boolean comparisons ( for determining if an empty block set for allocation should be split once, twice, or not at all )
	uint32 valComp[ 2 ] = { addr != block->AddrSet, addr + size < block->AddrSet + block->blockSize };

	int i = 0;
	for( ; i < 2; i++ ) {
		// Generate a new block if condition requires it, based on earlier comparison
		if( valComp[ i ] ) {
			// If comparison check is true, then empty block is split in two empty blocks. Addresses set, block sizes corrected, and
			//			nodes linked between the two. This can be done up to two times, resulting in 3 empty blocks sequentially.
			//			The middle in this case will be the true allocated block. If split once in total, either block will be
			//			the true block. Also done is examination of the first block and first empty block, which will be set as well
			struct s_SingleBlock *newBlock = ( struct s_SingleBlock* )malloc( sizeof( struct s_SingleBlock ));
			newBlock->indexOut = 0;
			newBlock->AddrSet = block->AddrSet + ( i * size );

			if( i ) {
				newBlock->blockSize = block->blockSize - size;
				block->blockSize = size;
			} else {
				newBlock->blockSize = (uint32)addr - (uint32)block->AddrSet;
				block->AddrSet = addr;
				block->blockSize -= newBlock->blockSize;
				if( block == *first )
					*first = newBlock;
			}

			// To get an idea of what the nodes represent, the first two in the array refer to the immediate pre/post blocks, which
			//		can be either empty or allocated blocks. The last two in the array refer to the prior/next blocks of the same type
			//		(empty or allocated), which can skip over blocks of a differing type. This allows for quick examination of similar
			//		blocks while keeping the organization of all the blocks in an ordered fashion. Think of it like a doubly-doubly
			//		linked list
			newBlock->node[ 1 - i ] = block;
			newBlock->node[ i ] = testBlock[ i ];
			newBlock->node[ i + 2 ] = testBlock[ i + 2 ];

			block->node[ i ] = newBlock;
			if( testBlock[ i ] )
				testBlock[ i ]->node[ 1 - i ] = newBlock;
			if( testBlock[ i + 2 ] )
				testBlock[ i + 2 ]->node[ 3 - i ] = newBlock;

			testBlock[ i + 2 ] = newBlock;

			if( block == *empty )
				*empty = newBlock;
		}
	}

	// Finish off node linking (in this case, NULL is possible, which refers to the end of the block sequences)
	if( testBlock[ 2 ] )
		testBlock[ 2 ]->node[ 3 ] = testBlock[ 3 ];
	if( testBlock[ 3 ] )
		testBlock[ 3 ]->node[ 2 ] = testBlock[ 2 ];

	// The condition of examining the first empty block is needed, in case the comparison check early is false for both
	if( block == *empty )
		*empty = block->node[ 3 ];

	block->node[ 2 ] = testBlock[ 0 ];
	block->node[ 3 ] = testBlock[ 1 ];
	if( testBlock[ 0 ] )
		testBlock[ 0 ]->node[ 3 ] = block;
	else
		*alloc = block;
	if( testBlock[ 1 ] )
		testBlock[ 1 ]->node[ 2 ] = block;

	return (uint8*)block;
}

uint32
vramBlock_allocateSpecial( s_vramBlock *mb, uint8 *addr, uint32 size ) {
	// Simple validity tests. Special allocations require "examination" data
	if( !addr || !size || !mb->lastExamined || !mb->lastExaminedAddr ) return 0;
	if( mb->lastExaminedAddr != addr || mb->lastExaminedSize != size ) return 0;

	// Can only get here if prior tests passed, meaning a spot is available, and can be allocated
	struct s_SingleBlock *newBlock = (struct s_SingleBlock*)vramBlock__allocateBlock( mb,  mb->lastExamined, addr, size );
	if( newBlock ) { // with current implementation, it should never be false if it gets to here
		uint32 curBlock;
		// Use a prior index if one exists. Else, obtain a new index
		if( mb->deallocCount )
			curBlock = (uint32)DynamicArrayGet( &mb->deallocBlocks, mb->deallocCount-- );
		else
			curBlock = mb->blockCount++;
		DynamicArraySet( &mb->blockPtrs, curBlock, (void*)newBlock );
		// Clear out examination data
		mb->lastExamined = NULL;
		mb->lastExaminedAddr = NULL;
		mb->lastExaminedSize = 0;
		newBlock->indexOut = curBlock;
		return curBlock;
	}
	return 0;
}

int AssignColorPalette(uint16 width, const uint16* table)
{
	uint32 colFormatVal = 4;
	uint8* checkAddr = vramBlock_examineSpecial( glGlob->vramBlocks[ 1 ], (uint8*)VRAM_E, width << 1, colFormatVal );

	if( checkAddr ) {
		// Calculate the address, logical and actual, of where the palette will go
		uint16* baseBank = vramGetBank( (uint16*)checkAddr );
		uint32 addr = ( (uint32)checkAddr - (uint32)baseBank );
		uint8 offset = 0;

		if( baseBank == VRAM_F )
			offset = ( VRAM_F_CR >> 3 ) & 3;
		else if( baseBank == VRAM_G )
			offset = ( VRAM_G_CR >> 3 ) & 3;
		addr += (( offset & 0x1 ) * 0x4000 ) + (( offset & 0x2 ) * 0x8000 );

		addr >>= colFormatVal;
		if( colFormatVal == 3 && addr >= 0x2000 ) {
			// palette location not good because 4 color mode cannot extend past 64K texture palette space
			GFX_PAL_FORMAT = glGlob->activePalette = 0;
			return -1;
		}
		gl_palette_data *palette;
		palette = (gl_palette_data*)malloc( sizeof( gl_palette_data ));
		palette->palIndex = vramBlock_allocateSpecial( glGlob->vramBlocks[ 1 ], checkAddr, width << 1 );
		palette->vramAddr = checkAddr;
		palette->addr = addr;

		palette->connectCount = 1;
		palette->palSize = width << 1;

		// copy straight to VRAM, and assign a palette name
		uint32 tempVRAM = VRAM_EFG_CR;
		uint16 *startBank = vramGetBank( (uint16*)palette->vramAddr );
		uint16 *endBank = vramGetBank( (uint16*)((char*)palette->vramAddr + ( width << 1 ) - 1));
		do {
			if( startBank == VRAM_E ) {
				vramSetBankE( VRAM_E_LCD );
				startBank += 0x8000;
			} else if( startBank == VRAM_F ) {
				vramSetBankF( VRAM_F_LCD );
				startBank += 0x2000;
			} else if( startBank == VRAM_G ) {
				vramSetBankG( VRAM_G_LCD );
				startBank += 0x2000;
			}
		} while ( startBank <= endBank );

		swiCopy( table, palette->vramAddr, width | COPY_MODE_HWORD );
		vramRestoreBanks_EFG( tempVRAM );
		return addr;
	}
	else
	{
		GFX_PAL_FORMAT = glGlob->activePalette;
		return glGlob->activePalette;
	}
	
}

BOOL LoadBitmap(FILE *fp, SurfaceID surf_no, bool create_surface)
{

	//if (gTextureLoaded) return TRUE;
	//gTextureLoaded = 1;

	if (surf_no == SURFACE_ID_LEVEL_TILESET || surf_no == SURFACE_ID_TEXT_BOX)
	{

	}
	else
	{
		return TRUE;
	}
	


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
			return TRUE;
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

			surf[surf_no].data[ pos ].color = color;
		}
	}
	
	//int textureid;
	//glGenTextures(1, &textureid);
	//glBindTexture(0, textureid);
	/*
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
	*/
	//if(surf_no == SURFACE_ID_ARMS_IMAGE || surf_no == SURFACE_ID_ITEM_IMAGE) paletteType = GL_RGB256;

	//glTexImage2D(0,0, paletteType, texW, texH, 0,
	//	GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
	//	(u8*)surf[surf_no].data);

	BUFFER_PIXEL* tex = (BUFFER_PIXEL*)glGetTexturePointer(gTempTextureId);
	
	uint32 vramTemp = VRAM_CR;

	uint32 size = 0;
	size = 1 << (gTextureWidth + gTextureHeight + 6);

	uint16 *startBank = vramGetBank( (uint16*)tex );
	uint16 *endBank = vramGetBank(( uint16*)( (char*)tex + size - 1 ));

	do {
		if( startBank == VRAM_A )
			vramSetBankA( VRAM_A_LCD );
		else if( startBank == VRAM_B )
			vramSetBankB( VRAM_B_LCD );
		else if( startBank == VRAM_C )
			vramSetBankC( VRAM_C_LCD );
		else if( startBank == VRAM_D )
			vramSetBankD( VRAM_D_LCD );
		startBank += 0x10000;
	} while ( startBank <= endBank );

	//dmaCopyWords(0, surf[surf_no].data, tex, size);
	int w = surf[surf_no].w;

	int gW = 1 << (gTextureWidth + 3);
	int gH = 1 << (gTextureHeight + 3);

	int yoffset = 0;
	int xoffset = 0;
	int paletteOffset = 0;

	switch(surf_no)
	{
		case SURFACE_ID_TEXT_BOX:
			xoffset = 512;
			yoffset = 0;
			paletteOffset = 0;
			break;
	}

	int texDivi = 2;
	switch(paletteType)
	{
		case GL_RGB4:
			texDivi = 4;
		break;
		case GL_RGB16:
			texDivi = 2;
		break;
		case GL_RGB256:
			texDivi = 1;
		break;
	}

	for(int h = 0; h < surf[surf_no].h; h++)
	{
		dmaCopyWords(0, surf[surf_no].data+(w*h/texDivi), tex+(gW*h/texDivi)+(gW*yoffset/texDivi)+(xoffset/texDivi), w);
	}

	vramRestorePrimaryBanks(vramTemp);

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
	//glColorTableEXT( 0, 0, 256, 0, 0, palette );

	//printf("Texture Id %d \n", textureid);

	surf[surf_no].palette = palette;
	surf[surf_no].paletteType = paletteType;
	surf[surf_no].palettesize = state.info_png.color.palettesize;
	surf[surf_no].xoffset = xoffset;
	surf[surf_no].yoffset = yoffset;

	surf[surf_no].paletteOffset = AssignColorPalette(surf[surf_no].palettesize, palette);


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

int gCurPaletteOffset = 0;

static void DrawBitmap(RECT *rcView, int x, int y, RECT *rect, SurfaceID surf_no, bool transparent)
{
	//TODO: draw queueing
	//TODO: don't render if transparent
	if(x > WINDOW_WIDTH) return;
	if(y > WINDOW_HEIGHT) return;

	RECT* srcRect = rect;
	srcRect->left += surf[surf_no].xoffset;
	srcRect->top += surf[surf_no].yoffset;
	srcRect->right += surf[surf_no].xoffset;
	srcRect->bottom += surf[surf_no].yoffset;

	if(!surf[surf_no].palette) return;
	
	
	//glColorSubTableEXT( 0, 0, surf[surf_no].palettesize, 0, 0, surf[surf_no].palette );

	if(gCurPaletteOffset != surf[surf_no].paletteOffset)
	{
		GFX_PAL_FORMAT = surf[surf_no].paletteOffset;
		gCurPaletteOffset = surf[surf_no].paletteOffset;
	}

	glSprite(x, y, rect, gTempTextureId, 0);
	//glSprite(x, y, rect, surf[surf_no].textureid, 0);
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
