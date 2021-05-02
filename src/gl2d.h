#pragma once

#include "WindowsWrapper.h"
#include "nds.h"
//#include "Draw.h"

extern v16 g_depth;

extern int gCurrentTexture;
extern int gCurPaletteOffset;
extern int gCurTexType;


void glScreen2D( void );
void SetOrtho( void );

void glBegin2D( void );
void glEnd2D( void );
__attribute__((hot))
static inline void gxTexcoord2i(t16 u, t16 v)
{
	GFX_TEX_COORD = (v << 20) | ( (u << 4) & 0xFFFF );
}

static inline void gxVertex3i(v16 x, v16 y, v16 z)
{
	GFX_VERTEX16 = (y << 16) | (x & 0xFFFF);
	GFX_VERTEX16 = ((uint32)(uint16)z);
}

__attribute__((hot))
static inline void gxVertex2i(v16 x, v16 y)
{
	GFX_VERTEX_XY = (y << 16) | (x & 0xFFFF);	
}
__attribute__((hot))
static void glSprite( int x1, int y1, RECT *rect, int textureID, int paletteOffset, int texType)
{

	int width = rect->right - rect->left;
	int height = rect->bottom - rect->top;

	if(width <= 0) return;
	if(height <= 0) return;

	int x2 = x1 + width;
	int y2 = y1 + height;

 	int	u2 = rect->left + width;
 	int v2 = rect->top + height;

    if ( textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, textureID);
        gCurrentTexture = textureID;
		gCurTexType = -1;
    }
	
	gl_texture_data *tex;
	if(gCurTexType != texType && (tex = (gl_texture_data*)DynamicArrayGet( &glGlob->texturePtrs, textureID)))
	{
		uint32 texFormat = tex->texFormat;
		if(texType == GL_RGB256)
		{
			texFormat &= ~(GL_RGB16 << 26);
			texFormat |= (GL_RGB256 << 26);
			texFormat &= ~(TEXTURE_SIZE_1024 << 20);
			texFormat |= (TEXTURE_SIZE_512 << 20);
			GFX_TEX_FORMAT = texFormat ;
		}
		else
		{
			texFormat &= ~(GL_RGB256 << 26);
			texFormat |= (GL_RGB16 << 26);
			texFormat &= ~(TEXTURE_SIZE_512 << 20);
			texFormat |= (TEXTURE_SIZE_1024 << 20);
			GFX_TEX_FORMAT = texFormat;
		}
		gCurTexType = texType;
		
	}

	if(gCurPaletteOffset != paletteOffset)
	{
		GFX_PAL_FORMAT = paletteOffset;
		gCurPaletteOffset = paletteOffset;
	}





#ifdef TWO_SCREENS
		gxTexcoord2i( rect->left, rect->top ); gxVertex3i( x2, y1, g_depth );	
		gxTexcoord2i( rect->left, v2 ); gxVertex2i( x1, y1 );
		gxTexcoord2i( u2, v2 ); gxVertex2i( x1, y2 );
		gxTexcoord2i( u2, rect->top ); gxVertex2i( x2, y2 );
#else
		gxTexcoord2i( rect->left, rect->top ); gxVertex3i( x1, y1, g_depth );	
		gxTexcoord2i( rect->left, v2 ); gxVertex2i( x1, y2 );
		gxTexcoord2i( u2, v2 ); gxVertex2i( x2, y2 );
		gxTexcoord2i( u2, rect->top ); gxVertex2i( x2, y1 );
#endif
	//glEnd();
	
	g_depth++;
}

void glSpriteTransparent( int x1, int y1, RECT *rect, int textureID, int paletteOffset, int texType, u32 alpha);
void glSpriteSizeParam( int x1, int y1, int width, int height, RECT *rect, int textureID, int paletteOffset, int texType);
void glBoxFilled( int x1, int y1, int x2, int y2, int color );
void glBoxFilledTransparent( int x1, int y1, int x2, int y2, int color, u32 alpha);