/******************************************************************************
	Easy GL2D
	Relminator 2010 
	Richard Eric M. Lope BSN RN
	http://rel.betterwebber.com
	A very small and simple DS rendering lib using the 3d core to render 2D stuff
******************************************************************************/

//Modified version of Easy Gl2D by relsoft
//https://github.com/devkitPro/libnds/blob/master/source/arm9/gl2d.c

#include "gl2d.h"

#include "WindowsWrapper.h"
#include "nds.h"

 v16 g_depth = 0;

int gCurrentTexture = 0;
 int gCurPaletteOffset = 0;
 int gCurTexType = 0;

void glScreen2D( void )
{
		
	// initialize gl
	glInit();
	
	//enable textures
	glEnable( GL_TEXTURE_2D );
	
	// enable antialiasing
	glEnable( GL_ANTIALIAS );
		
	// setup the rear plane
	glClearColor( 0, 0, 0, 31 ); // BG must be opaque for AA to work
	glClearPolyID( 63 ); // BG must have a unique polygon ID for AA to work
	
	glClearDepth( GL_MAX_DEPTH );

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);
	
	
	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 70, 256.0 / 192.0, 1, 200 );
	
	gluLookAt(	0.0, 0.0, 1.0,		//camera possition 
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up

	glMaterialf( GL_AMBIENT, RGB15(31,31,31) );
	glMaterialf( GL_DIFFUSE, RGB15(31,31,31) );
	glMaterialf( GL_SPECULAR, BIT(15) | RGB15(31,31,31) );
	glMaterialf( GL_EMISSION, RGB15(31,31,31) );

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();

	//not a real gl function and will likely change
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_BACK );
	
	
}



void SetOrtho( void )		
{

	glMatrixMode( GL_PROJECTION );     // set matrixmode to projection
	glLoadIdentity();				 // reset
	glOrthof32( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1 << 12, 1 << 12 );  // downscale projection matrix
	
}

void glBegin2D( void )
{

	
	// save 3d perpective projection matrix
	glMatrixMode( GL_PROJECTION );   
    glPushMatrix();
	
	// save 3d modelview matrix for safety
	glMatrixMode( GL_MODELVIEW );
    glPushMatrix();
	
	
	//what?!! No glDisable(GL_DEPTH_TEST)?!!!!!!
	glEnable( GL_BLEND );
	glEnable( GL_TEXTURE_2D );
	glDisable( GL_ANTIALIAS );		// disable AA
	glDisable( GL_OUTLINE );			// disable edge-marking

	glColor( 0xFFFF ); 				// max color
	
	glPolyFmt( POLY_ALPHA(31) | POLY_CULL_NONE );;  // no culling

	SetOrtho();
	
	glMatrixMode( GL_TEXTURE );      	// reset texture matrix just in case we did some funky stuff with it
	glLoadIdentity();
	
	glMatrixMode( GL_MODELVIEW );		// reset modelview matrix. No need to scale up by << 12
	glLoadIdentity();

	g_depth = 0; 	// set depth to 0. We need this var since we cannot disable depth testing

	glBegin( GL_QUADS );

}

void glEnd2D( void )
{

	// restore 3d matrices and set current matrix to modelview
	glMatrixMode( GL_PROJECTION );    
    glPopMatrix( 1 );
	glMatrixMode( GL_MODELVIEW );
    glPopMatrix( 1 );

}



void glSpriteTransparent( int x1, int y1, RECT *rect, int textureID, int paletteOffset, int texType, u32 alpha)
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

	glPolyFmt(POLY_ALPHA(alpha) | POLY_CULL_NONE );
	glBegin( GL_QUADS );	

	gl_texture_data *tex = NULL;
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
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
	g_depth++;
	glBegin( GL_QUADS );	
}


void glSpriteSizeParam( int x1, int y1, int width, int height, RECT *rect, int textureID, int paletteOffset, int texType)
{

	if(x1 + width < 0) return;
	if(y1 + height < 0) return;

	int x2 = x1 + width;
	int y2 = y1 + height;

	int rectWidth = rect->right - rect->left;
	int rectHeight = rect->bottom - rect->top;


 	int	u2 = rect->left + rectWidth;
 	int v2 = rect->top + rectHeight;

    if ( textureID != gCurrentTexture )
    {
        glBindTexture( GL_TEXTURE_2D, textureID);
        gCurrentTexture = textureID;
		gCurTexType = -1;
    }
	
	gl_texture_data *tex = NULL;
	uint32 texFormat;
	if(gCurTexType != texType && (tex = (gl_texture_data*)DynamicArrayGet( &glGlob->texturePtrs, textureID)))
	{
		texFormat = tex->texFormat;
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


void glBoxFilled( int x1, int y1, int x2, int y2, int color )
{

	x2++;
	y2++;
	
	glBindTexture( 0, 0 );
	glColor( color );
		gxVertex3i( x1, y1, g_depth );		// use 3i for first vertex so that we increment HW depth
		gxVertex2i( x1, y2 );				// no need for 3 vertices as 2i would share last depth call
		gxVertex2i( x2, y2 );
		gxVertex2i( x2, y1 );
	glColor( 0x7FFF );
	g_depth++;
	gCurrentTexture = 0;
	gCurPaletteOffset = 0;
	gCurTexType = -1;
}



void glBoxFilledTransparent( int x1, int y1, int x2, int y2, int color, u32 alpha)
{

	x2++;
	y2++;
	
	glBindTexture( 0, 0 );
	glPolyFmt(POLY_ALPHA(alpha) | POLY_CULL_NONE );
	glBegin( GL_QUADS ); //why is this NEEDED NINTENDO WHY
	glColor( color );
		gxVertex3i( x1, y1, g_depth );		// use 3i for first vertex so that we increment HW depth
		gxVertex2i( x1, y2 );				// no need for 3 vertices as 2i would share last depth call
		gxVertex2i( x2, y2 );
		gxVertex2i( x2, y1 );
	glColor( 0x7FFF );
	g_depth++;
	gCurrentTexture = 0;
	gCurPaletteOffset = 0;
	gCurTexType = -1;

	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_NONE );
	glBegin( GL_QUADS );

}
