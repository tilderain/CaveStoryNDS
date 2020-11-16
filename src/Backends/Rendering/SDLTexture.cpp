// Released under the MIT licence.
// See LICENCE.txt for details.

#include "../Rendering.h"

#include <stddef.h>
#ifndef NDEBUG
#include <stdio.h>
#endif
#include <stdlib.h>

#include "SDL.h"

#include "../../WindowsWrapper.h"

#include "../../Draw.h"
#include "../../Ending.h"
#include "../../MapName.h"
#include "../../Resource.h"
#include "../../TextScr.h"

typedef struct Backend_Surface
{
	SDL_Texture *texture;
	unsigned char *pixels;
	unsigned int width;
	unsigned int height;
	BOOL lost;

	struct Backend_Surface *next;
	struct Backend_Surface *prev;
} Backend_Surface;

typedef struct Backend_Glyph
{
	SDL_Texture *texture;
	unsigned int width;
	unsigned int height;
} Backend_Glyph;

static SDL_Window *window;
static SDL_Renderer *renderer;

static Backend_Surface framebuffer;

static Backend_Surface *surface_list_head;

static void RectToSDLRect(const RECT *rect, SDL_Rect *sdl_rect)
{
	sdl_rect->x = (int)rect->left;
	sdl_rect->y = (int)rect->top;
	sdl_rect->w = (int)(rect->right - rect->left);
	sdl_rect->h = (int)(rect->bottom - rect->top);

	if (sdl_rect->w < 0)
		sdl_rect->w = 0;

	if (sdl_rect->h < 0)
		sdl_rect->h = 0;
}

Backend_Surface* Backend_Init(const char *title, int width, int height, BOOL fullscreen)
{
#ifndef NDEBUG
	puts("Available SDL2 render drivers:");

	for (int i = 0; i < SDL_GetNumRenderDrivers(); ++i)
	{
		SDL_RendererInfo info;
		SDL_GetRenderDriverInfo(i, &info);
		puts(info.name);
	}
#endif

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);

	if (window != NULL)
	{
	#ifndef _WIN32	// On Windows, we use native icons instead (so we can give the taskbar and window separate icons, like the original EXE does)
		size_t resource_size;
		const unsigned char *resource_data = FindResource("ICON_MINI", "ICON", &resource_size);
		SDL_RWops *rwops = SDL_RWFromConstMem(resource_data, resource_size);
		SDL_Surface *icon_surface = SDL_LoadBMP_RW(rwops, 1);
		SDL_SetWindowIcon(window, icon_surface);
		SDL_FreeSurface(icon_surface);
	#endif

		if (fullscreen)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	#if SDL_VERSION_ATLEAST(2,0,10)
		SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");	// We never interfere with the renderer, so don't let SDL implicitly disable batching
	#endif

		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);

		if (renderer != NULL)
		{
		#ifndef NDEBUG
			SDL_RendererInfo info;
			SDL_GetRendererInfo(renderer, &info);
			printf("Selected SDL2 render driver: %s\n", info.name);
		#endif

			int width, height;
			SDL_GetRendererOutputSize(renderer, &width, &height);
			framebuffer.texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

			if (framebuffer.texture != NULL)
			{
				framebuffer.width = width;
				framebuffer.height = height;

				return &framebuffer;
			}

			SDL_DestroyRenderer(renderer);
		}

		SDL_DestroyWindow(window);
	}

	return NULL;
}

void Backend_Deinit(void)
{
	SDL_DestroyTexture(framebuffer.texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void Backend_DrawScreen(void)
{
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, framebuffer.texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

Backend_Surface* Backend_CreateSurface(unsigned int width, unsigned int height)
{
	Backend_Surface *surface = (Backend_Surface*)malloc(sizeof(Backend_Surface));

	if (surface == NULL)
		return NULL;

	surface->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, width, height);

	if (surface->texture == NULL)
	{
		free(surface);
		return NULL;
	}

	surface->width = width;
	surface->height = height;
	surface->lost = FALSE;

	// Add to linked-list
	surface->prev = NULL;
	surface->next = surface_list_head;
	surface_list_head = surface;

	if (surface->next != NULL)
		surface->next->prev = surface;

	return surface;
}

void Backend_FreeSurface(Backend_Surface *surface)
{
	if (surface == NULL)
		return;

	// Remove from linked list
	if (surface->next != NULL)
		surface->next->prev = surface->prev;
	if (surface->prev != NULL)
		surface->prev->next = surface->next;

	SDL_DestroyTexture(surface->texture);
	free(surface);
}

BOOL Backend_IsSurfaceLost(Backend_Surface *surface)
{
	return surface->lost;
}

void Backend_RestoreSurface(Backend_Surface *surface)
{
	surface->lost = FALSE;
}

unsigned char* Backend_LockSurface(Backend_Surface *surface, unsigned int *pitch, unsigned int width, unsigned int height)
{
	if (surface == NULL)
		return NULL;

	*pitch = width * 3;

	surface->pixels = (unsigned char*)malloc(width * height * 3);

	return surface->pixels;
}

void Backend_UnlockSurface(Backend_Surface *surface, unsigned int width, unsigned int height)
{
	if (surface == NULL)
		return;

	unsigned char *buffer = (unsigned char*)malloc(width * height * 4);
	const unsigned char *src_pixel = surface->pixels;

	// Convert the SDL_Surface's colour-keyed pixels to RGBA32
	for (unsigned int y = 0; y < height; ++y)
	{
		unsigned char *buffer_pointer = &buffer[y * width * 4];

		for (unsigned int x = 0; x < width; ++x)
		{
			*buffer_pointer++ = src_pixel[0];
			*buffer_pointer++ = src_pixel[1];
			*buffer_pointer++ = src_pixel[2];

			if (src_pixel[0] == 0 && src_pixel[1] == 0 && src_pixel[2] == 0)	// Assumes the colour key will always be #000000 (black)
				*buffer_pointer++ = 0;
			else
				*buffer_pointer++ = 0xFF;

			src_pixel += 3;
		}
	}

	free(surface->pixels);

	SDL_Rect rect = {0, 0, width, height};
	SDL_UpdateTexture(surface->texture, &rect, buffer, width * 4);

	free(buffer);
}

void Backend_Blit(Backend_Surface *source_surface, const RECT *rect, Backend_Surface *destination_surface, long x, long y, BOOL colour_key)
{
	if (source_surface == NULL || destination_surface == NULL)
		return;

	SDL_Rect source_rect;
	RectToSDLRect(rect, &source_rect);

	SDL_Rect destination_rect = {(int)x, (int)y, source_rect.w, source_rect.h};

	// Blit the texture
	SDL_SetTextureBlendMode(source_surface->texture, colour_key ? SDL_BLENDMODE_BLEND : SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(renderer, destination_surface->texture);
	SDL_RenderCopy(renderer, source_surface->texture, &source_rect, &destination_rect);
}

void Backend_ColourFill(Backend_Surface *surface, const RECT *rect, unsigned char red, unsigned char green, unsigned char blue)
{
	if (surface == NULL)
		return;

	SDL_Rect sdl_rect;
	RectToSDLRect(rect, &sdl_rect);

	// Check colour-key
	if (red == 0 && green == 0 && blue == 0)
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	else
		SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);

	// Draw colour
	SDL_SetRenderTarget(renderer, surface->texture);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	SDL_RenderFillRect(renderer, &sdl_rect);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

BOOL Backend_SupportsSubpixelGlyphs(void)
{
	return FALSE;	// SDL_Textures don't have per-component alpha
}

Backend_Glyph* Backend_LoadGlyph(const unsigned char *pixels, unsigned int width, unsigned int height, int pitch, FontPixelMode pixel_mode)
{
	Backend_Glyph *glyph = (Backend_Glyph*)malloc(sizeof(Backend_Glyph));

	if (glyph == NULL)
		return NULL;

	glyph->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, width, height);

	if (glyph->texture == NULL)
	{
		free(glyph);
		return NULL;
	}

	unsigned char *buffer = (unsigned char*)malloc(width * height * 4);

	if (buffer == NULL)
	{
		SDL_DestroyTexture(glyph->texture);
		free(glyph);
		return NULL;
	}

	unsigned char *destination_pointer = buffer;

	switch (pixel_mode)
	{
		case FONT_PIXEL_MODE_LCD:
			// Unsupported
			break;

		case FONT_PIXEL_MODE_GRAY:

			for (unsigned int y = 0; y < height; ++y)
			{
				const unsigned char *source_pointer = pixels + y * pitch;

				for (unsigned int x = 0; x < width; ++x)
				{
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = *source_pointer++;
				}
			}

			break;

		case FONT_PIXEL_MODE_MONO:
			for (unsigned int y = 0; y < height; ++y)
			{
				const unsigned char *source_pointer = pixels + y * pitch;

				for (unsigned int x = 0; x < width; ++x)
				{
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = 0xFF;
					*destination_pointer++ = *source_pointer++ ? 0xFF : 0;
				}
			}

			break;
	}

	SDL_UpdateTexture(glyph->texture, NULL, buffer, width * 4);
	free(buffer);

	glyph->width = width;
	glyph->height = height;

	return glyph;
}

void Backend_UnloadGlyph(Backend_Glyph *glyph)
{
	if (glyph == NULL)
		return;

	SDL_DestroyTexture(glyph->texture);
	free(glyph);
}

void Backend_DrawGlyph(Backend_Surface *surface, Backend_Glyph *glyph, long x, long y, const unsigned char *colours)
{
	// The SDL_Texture side of things uses alpha, not a colour-key, so the bug where the font is blended
	// with the colour key doesn't occur.

	if (glyph == NULL || surface == NULL)
		return;

	SDL_Rect destination_rect = {(int)x, (int)y, (int)glyph->width, (int)glyph->height};

	// Blit the texture
	SDL_SetTextureColorMod(glyph->texture, colours[0], colours[1], colours[2]);
	SDL_SetTextureBlendMode(glyph->texture, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(renderer, surface->texture);
	SDL_RenderCopy(renderer, glyph->texture, NULL, &destination_rect);
}

void Backend_HandleRenderTargetLoss(void)
{
	for (Backend_Surface *surface = surface_list_head; surface != NULL; surface = surface->next)
		surface->lost = TRUE;
}

void Backend_HandleWindowResize(void)
{
	// No problem for us
}
