// Released under the MIT licence.
// See LICENCE.txt for details.

// Dual OpenGL 3.2 and OpenGL ES 2.0 renderer

#include "../Rendering.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_OPENGLES2
#include <GLES2/gl2.h>
#else
#include <glad/glad.h>
#endif

#include "SDL.h"

#include "../../WindowsWrapper.h"

#include "../../Resource.h"

#define TOTAL_VBOS 8

#define ATTRIBUTE_INPUT_VERTEX_COORDINATES 1
#define ATTRIBUTE_INPUT_TEXTURE_COORDINATES 2

typedef enum RenderMode
{
	MODE_BLANK,
	MODE_DRAW_SURFACE,
	MODE_DRAW_SURFACE_WITH_TRANSPARENCY,
	MODE_COLOUR_FILL,
	MODE_DRAW_GLYPH,
	MODE_DRAW_GLYPH_LCD
} RenderMode;

typedef struct Backend_Surface
{
	GLuint texture_id;
	unsigned int width;
	unsigned int height;
	unsigned char *pixels;
} Backend_Surface;

typedef struct Backend_Glyph
{
	GLuint texture_id;
	unsigned int width;
	unsigned int height;
	unsigned char pixel_mode;
} Backend_Glyph;

typedef struct Coordinate2D
{
	GLfloat x;
	GLfloat y;
} Coordinate2D;

typedef struct Vertex
{
	Coordinate2D vertex_coordinate;
	Coordinate2D texture_coordinate;
} Vertex;

typedef struct VertexBufferSlot
{
	Vertex vertices[2][3];
} VertexBufferSlot;

static SDL_Window *window;
static SDL_GLContext context;

static GLuint program_texture;
static GLuint program_texture_colour_key;
static GLuint program_colour_fill;
static GLuint program_glyph_normal;
static GLuint program_glyph_subpixel_part1;
static GLuint program_glyph_subpixel_part2;

static GLint program_colour_fill_uniform_colour;
static GLint program_glyph_normal_uniform_colour;
static GLint program_glyph_subpixel_part2_uniform_colour;

#ifndef USE_OPENGLES2
static GLuint vertex_array_id;
#endif
static GLuint vertex_buffer_ids[TOTAL_VBOS];
static GLuint framebuffer_id;

static VertexBufferSlot *local_vertex_buffer;
static unsigned long local_vertex_buffer_size;
static unsigned long current_vertex_buffer_slot;

static RenderMode last_render_mode;

static Backend_Surface framebuffer;

#ifdef USE_OPENGLES2
static const GLchar *vertex_shader_plain = " \
#version 100\n \
attribute vec2 input_vertex_coordinates; \
void main() \
{ \
	gl_Position = vec4(input_vertex_coordinates.xy, 0.0, 1.0); \
} \
";

static const GLchar *vertex_shader_texture = " \
#version 100\n \
attribute vec2 input_vertex_coordinates; \
attribute vec2 input_texture_coordinates; \
varying vec2 texture_coordinates; \
void main() \
{ \
	texture_coordinates = input_texture_coordinates; \
	gl_Position = vec4(input_vertex_coordinates.xy, 0.0, 1.0); \
} \
";

static const GLchar *fragment_shader_texture = " \
#version 100\n \
precision mediump float; \
uniform sampler2D tex; \
varying vec2 texture_coordinates; \
void main() \
{ \
	gl_FragColor = texture2D(tex, texture_coordinates); \
} \
";

static const GLchar *fragment_shader_texture_colour_key = " \
#version 100\n \
precision mediump float; \
uniform sampler2D tex; \
varying vec2 texture_coordinates; \
void main() \
{ \
	vec4 colour = texture2D(tex, texture_coordinates); \
\
	if (colour.xyz == vec3(0.0f, 0.0f, 0.0f)) \
		discard; \
\
	gl_FragColor = colour; \
} \
";

static const GLchar *fragment_shader_colour_fill = " \
#version 100\n \
precision mediump float; \
uniform vec4 colour; \
void main() \
{ \
	gl_FragColor = colour; \
} \
";

static const GLchar *fragment_shader_glyph_normal = " \
#version 100\n \
precision mediump float; \
uniform sampler2D tex; \
uniform vec4 colour; \
varying vec2 texture_coordinates; \
void main() \
{ \
	gl_FragColor = colour * vec4(1.0, 1.0, 1.0, texture2D(tex, texture_coordinates).r); \
} \
";

static const GLchar *fragment_shader_glyph_subpixel_part1 = " \
#version 100\n \
precision mediump float; \
uniform sampler2D tex; \
varying vec2 texture_coordinates; \
void main() \
{ \
	gl_FragColor = texture2D(tex, texture_coordinates); \
} \
";

static const GLchar *fragment_shader_glyph_subpixel_part2 = " \
#version 100\n \
precision mediump float; \
uniform sampler2D tex; \
uniform vec4 colour; \
varying vec2 texture_coordinates; \
void main() \
{ \
	gl_FragColor = colour * texture2D(tex, texture_coordinates); \
} \
";
#else
static const GLchar *vertex_shader_plain = " \
#version 150 core\n \
in vec2 input_vertex_coordinates; \
void main() \
{ \
	gl_Position = vec4(input_vertex_coordinates.xy, 0.0, 1.0); \
} \
";

static const GLchar *vertex_shader_texture = " \
#version 150 core\n \
in vec2 input_vertex_coordinates; \
in vec2 input_texture_coordinates; \
out vec2 texture_coordinates; \
void main() \
{ \
	texture_coordinates = input_texture_coordinates; \
	gl_Position = vec4(input_vertex_coordinates.xy, 0.0, 1.0); \
} \
";

static const GLchar *fragment_shader_texture = " \
#version 150 core\n \
uniform sampler2D tex; \
in vec2 texture_coordinates; \
out vec4 fragment; \
void main() \
{ \
	fragment = texture(tex, texture_coordinates); \
} \
";

static const GLchar *fragment_shader_texture_colour_key = " \
#version 150 core\n \
uniform sampler2D tex; \
in vec2 texture_coordinates; \
out vec4 fragment; \
void main() \
{ \
	vec4 colour = texture(tex, texture_coordinates); \
\
	if (colour.xyz == vec3(0.0f, 0.0f, 0.0f)) \
		discard; \
\
	fragment = colour; \
} \
";

static const GLchar *fragment_shader_colour_fill = " \
#version 150 core\n \
uniform vec4 colour; \
out vec4 fragment; \
void main() \
{ \
	fragment = colour; \
} \
";

static const GLchar *fragment_shader_glyph_normal = " \
#version 150 core\n \
uniform sampler2D tex; \
uniform vec4 colour; \
in vec2 texture_coordinates; \
out vec4 fragment; \
void main() \
{ \
	fragment = colour * vec4(1.0, 1.0, 1.0, texture(tex, texture_coordinates).r); \
} \
";

static const GLchar *fragment_shader_glyph_subpixel_part1 = " \
#version 150 core\n \
uniform sampler2D tex; \
in vec2 texture_coordinates; \
out vec4 fragment; \
void main() \
{ \
	fragment = texture(tex, texture_coordinates); \
} \
";

static const GLchar *fragment_shader_glyph_subpixel_part2 = " \
#version 150 core\n \
uniform sampler2D tex; \
uniform vec4 colour; \
in vec2 texture_coordinates; \
out vec4 fragment; \
void main() \
{ \
	fragment = colour * texture(tex, texture_coordinates); \
} \
";
#endif
/*
static void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void* userParam)
{
	(void)source;
	(void)type;
	(void)id;
	(void)length;
	(void)userParam;

	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
		printf("OpenGL debug: %s\n", message);
}
*/
static GLuint CompileShader(const char *vertex_shader_source, const char *fragment_shader_source)
{
	GLint shader_status;

	GLuint program_id = glCreateProgram();

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_status);
	if (shader_status != GL_TRUE)
	{
		char buffer[0x200];
		glGetShaderInfoLog(vertex_shader, sizeof(buffer), NULL, buffer);
		printf("Vertex shader: %s", buffer);
		return 0;
	}

	glAttachShader(program_id, vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_status);
	if (shader_status != GL_TRUE)
	{
		char buffer[0x200];
		glGetShaderInfoLog(fragment_shader, sizeof(buffer), NULL, buffer);
		printf("Fragment shader: %s", buffer);
		return 0;
	}

	glAttachShader(program_id, fragment_shader);

	glBindAttribLocation(program_id, ATTRIBUTE_INPUT_VERTEX_COORDINATES, "input_vertex_coordinates");
	glBindAttribLocation(program_id, ATTRIBUTE_INPUT_TEXTURE_COORDINATES, "input_texture_coordinates");

	glLinkProgram(program_id);

	glGetProgramiv(program_id, GL_LINK_STATUS, &shader_status);
	if (shader_status != GL_TRUE)
	{
		char buffer[0x200];
		glGetProgramInfoLog(program_id, sizeof(buffer), NULL, buffer);
		printf("Shader linker: %s", buffer);
		return 0;
	}

	return program_id;
}

static VertexBufferSlot* GetVertexBufferSlot(void)
{
	if (current_vertex_buffer_slot >= local_vertex_buffer_size)
	{
		if (local_vertex_buffer_size == 0)
			local_vertex_buffer_size = 1;
		else
			local_vertex_buffer_size <<= 1;

		local_vertex_buffer = (VertexBufferSlot*)realloc(local_vertex_buffer, local_vertex_buffer_size * sizeof(VertexBufferSlot));
	}

	return &local_vertex_buffer[current_vertex_buffer_slot++];
}

static void FlushVertexBuffer(void)
{
	static unsigned long vertex_buffer_size[TOTAL_VBOS];
	static unsigned int current_vertex_buffer = 0;

	if (current_vertex_buffer_slot == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_ids[current_vertex_buffer]);
	glVertexAttribPointer(ATTRIBUTE_INPUT_VERTEX_COORDINATES, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, vertex_coordinate));
	glVertexAttribPointer(ATTRIBUTE_INPUT_TEXTURE_COORDINATES, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture_coordinate));

	if (local_vertex_buffer_size > vertex_buffer_size[current_vertex_buffer])
	{
		vertex_buffer_size[current_vertex_buffer] = local_vertex_buffer_size;
		glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size[current_vertex_buffer] * sizeof(VertexBufferSlot), local_vertex_buffer, GL_STREAM_DRAW);
	}
	else
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, current_vertex_buffer_slot * sizeof(VertexBufferSlot), local_vertex_buffer);
	}

	if (++current_vertex_buffer >= TOTAL_VBOS)
		current_vertex_buffer = 0;

	if (last_render_mode == MODE_DRAW_GLYPH_LCD)
	{
		// Here we're going to draw with per-component alpha.
		// Since OpenGL doesn't really support this, we have to do it manually:

		// Step one: attenuate the destination pixels by the alpha
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
		glUseProgram(program_glyph_subpixel_part1);
		glDrawArrays(GL_TRIANGLES, 0, 6 * current_vertex_buffer_slot);

		// Step two: add the new pixels on top of them
		glBlendFunc(GL_ONE, GL_ONE);
		glUseProgram(program_glyph_subpixel_part2);
	}
	else if (last_render_mode == MODE_DRAW_GLYPH)
	{
		// Here, we just use a regular alpha channel
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUseProgram(program_glyph_normal);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6 * current_vertex_buffer_slot);

	current_vertex_buffer_slot = 0;
}

Backend_Surface* Backend_Init(const char *title, int width, int height, BOOL fullscreen)
{
#ifdef USE_OPENGLES2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

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

		int window_width, window_height;
		SDL_GetWindowSize(window, &window_width, &window_height);

		context = SDL_GL_CreateContext(window);

		if (context != NULL)
		{
			if (SDL_GL_MakeCurrent(window, context) == 0)
			{
			#ifndef USE_OPENGLES2
				if (gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			#endif
				{
				#ifndef USE_OPENGLES2
					// Check if the platform supports OpenGL 3.2
					if (GLAD_GL_VERSION_3_2)
				#endif
					{
					#ifndef NDEBUG
						printf("GL_VENDOR = %s\n", glGetString(GL_VENDOR));
						printf("GL_RENDERER = %s\n", glGetString(GL_RENDERER));
						printf("GL_VERSION = %s\n", glGetString(GL_VERSION));
					#endif

						//glEnable(GL_DEBUG_OUTPUT);
						//glDebugMessageCallback(MessageCallback, 0);

						glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
						glClear(GL_COLOR_BUFFER_BIT);

					#ifndef USE_OPENGLES2
						// Set up Vertex Array Object
						glGenVertexArrays(1, &vertex_array_id);
						glBindVertexArray(vertex_array_id);
					#endif

						// Set up Vertex Buffer Objects
						glGenBuffers(TOTAL_VBOS, vertex_buffer_ids);

						// Set up the vertex attributes
						glEnableVertexAttribArray(1);

						// Set up our shaders
						program_texture = CompileShader(vertex_shader_texture, fragment_shader_texture);
						program_texture_colour_key = CompileShader(vertex_shader_texture, fragment_shader_texture_colour_key);
						program_colour_fill = CompileShader(vertex_shader_plain, fragment_shader_colour_fill);
						program_glyph_normal = CompileShader(vertex_shader_texture, fragment_shader_glyph_normal);
						program_glyph_subpixel_part1 = CompileShader(vertex_shader_texture, fragment_shader_glyph_subpixel_part1);
						program_glyph_subpixel_part2 = CompileShader(vertex_shader_texture, fragment_shader_glyph_subpixel_part2);

						if (program_texture != 0 && program_texture_colour_key != 0 && program_colour_fill != 0 && program_glyph_normal != 0 && program_glyph_subpixel_part1 != 0 && program_glyph_subpixel_part2 != 0)
						{
							// Get shader uniforms
							program_colour_fill_uniform_colour = glGetUniformLocation(program_colour_fill, "colour");
							program_glyph_normal_uniform_colour = glGetUniformLocation(program_glyph_normal, "colour");
							program_glyph_subpixel_part2_uniform_colour = glGetUniformLocation(program_glyph_subpixel_part2, "colour");

							// Set up framebuffer (used for surface-to-surface blitting)
							glGenFramebuffers(1, &framebuffer_id);
							glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);

							// Set up framebuffer screen texture (used for screen-to-surface blitting)
							glGenTextures(1, &framebuffer.texture_id);
							glBindTexture(GL_TEXTURE_2D, framebuffer.texture_id);
						#ifdef USE_OPENGLES2
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
						#else
							glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, window_width, window_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
						#endif
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
						#ifndef USE_OPENGLES2
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
						#endif

							framebuffer.width = window_width;
							framebuffer.height = window_height;

							glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer.texture_id, 0);
							glViewport(0, 0, framebuffer.width, framebuffer.height);

							return &framebuffer;
						}

						if (program_glyph_subpixel_part2 != 0)
							glDeleteProgram(program_glyph_subpixel_part2);

						if (program_glyph_subpixel_part1 != 0)
							glDeleteProgram(program_glyph_subpixel_part1);

						if (program_glyph_normal != 0)
							glDeleteProgram(program_glyph_normal);

						if (program_colour_fill != 0)
							glDeleteProgram(program_colour_fill);

						if (program_texture_colour_key != 0)
							glDeleteProgram(program_texture_colour_key);

						if (program_texture != 0)
							glDeleteProgram(program_texture);

						glDeleteBuffers(TOTAL_VBOS, vertex_buffer_ids);
					#ifndef USE_OPENGLES2
						glDeleteVertexArrays(1, &vertex_array_id);
					#endif
					}
				}
			}

			SDL_GL_DeleteContext(context);
		}

		SDL_DestroyWindow(window);
	}

	return NULL;
}

void Backend_Deinit(void)
{
	free(local_vertex_buffer);

	glDeleteTextures(1, &framebuffer.texture_id);
	glDeleteFramebuffers(1, &framebuffer_id);
	glDeleteProgram(program_glyph_subpixel_part2);
	glDeleteProgram(program_glyph_subpixel_part1);
	glDeleteProgram(program_glyph_normal);
	glDeleteProgram(program_colour_fill);
	glDeleteProgram(program_texture_colour_key);
	glDeleteProgram(program_texture);
	glDeleteBuffers(TOTAL_VBOS, vertex_buffer_ids);
#ifndef USE_OPENGLES2
	glDeleteVertexArrays(1, &vertex_array_id);
#endif
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void Backend_DrawScreen(void)
{
	FlushVertexBuffer();
	last_render_mode = MODE_BLANK;

	glUseProgram(program_texture);

	glDisable(GL_BLEND);

	// Enable texture coordinates, since this uses textures
	glEnableVertexAttribArray(2);

	// Target actual screen, and not our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, framebuffer.width, framebuffer.height);

	// Draw framebuffer to screen
	glBindTexture(GL_TEXTURE_2D, framebuffer.texture_id);

	VertexBufferSlot *vertex_buffer_slot = GetVertexBufferSlot();

	vertex_buffer_slot->vertices[0][0].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[0][0].texture_coordinate.y = 1.0f;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.y = 1.0f;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.y = 0.0f;

	vertex_buffer_slot->vertices[1][0].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[1][0].texture_coordinate.y = 1.0f;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.y = 0.0f;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.y = 0.0f;

	vertex_buffer_slot->vertices[0][0].vertex_coordinate.x = -1.0f;
	vertex_buffer_slot->vertices[0][0].vertex_coordinate.y = -1.0f;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.y = -1.0f;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.y = 1.0f;

	vertex_buffer_slot->vertices[1][0].vertex_coordinate.x = -1.0f;
	vertex_buffer_slot->vertices[1][0].vertex_coordinate.y = -1.0f;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.y = 1.0f;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.x = -1.0f;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.y = 1.0f;

	FlushVertexBuffer();

	SDL_GL_SwapWindow(window);

	// According to https://www.khronos.org/opengl/wiki/Common_Mistakes#Swap_Buffers
	// the buffer should always be cleared, even if it seems unnecessary
	glClear(GL_COLOR_BUFFER_BIT);

	// Switch back to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
}

Backend_Surface* Backend_CreateSurface(unsigned int width, unsigned int height)
{
	Backend_Surface *surface = (Backend_Surface*)malloc(sizeof(Backend_Surface));

	if (surface == NULL)
		return NULL;

	GLint previously_bound_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previously_bound_texture);

	glGenTextures(1, &surface->texture_id);
	glBindTexture(GL_TEXTURE_2D, surface->texture_id);
#ifdef USE_OPENGLES2
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
#else
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#ifndef USE_OPENGLES2
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
#endif

	glBindTexture(GL_TEXTURE_2D, previously_bound_texture);

	surface->width = width;
	surface->height = height;

	return surface;
}

void Backend_FreeSurface(Backend_Surface *surface)
{
	if (surface == NULL)
		return;

	glDeleteTextures(1, &surface->texture_id);
	free(surface);
}

BOOL Backend_IsSurfaceLost(Backend_Surface *surface)
{
	(void)surface;

	return FALSE;
}

void Backend_RestoreSurface(Backend_Surface *surface)
{
	(void)surface;
}

unsigned char* Backend_LockSurface(Backend_Surface *surface, unsigned int *pitch, unsigned int width, unsigned int height)
{
	if (surface == NULL)
		return NULL;

	surface->pixels = (unsigned char*)malloc(width * height * 3);
	*pitch = width * 3;
	return surface->pixels;
}

void Backend_UnlockSurface(Backend_Surface *surface, unsigned int width, unsigned int height)
{
	if (surface == NULL)
		return;

	GLint previously_bound_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &previously_bound_texture);

	glBindTexture(GL_TEXTURE_2D, surface->texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	free(surface->pixels);

	glBindTexture(GL_TEXTURE_2D, previously_bound_texture);
}

void Backend_Blit(Backend_Surface *source_surface, const RECT *rect, Backend_Surface *destination_surface, long x, long y, BOOL colour_key)
{
	static Backend_Surface *last_source_surface;
	static Backend_Surface *last_destination_surface;

	if (source_surface == NULL || destination_surface == NULL)
		return;

	if (rect->right - rect->left < 0 || rect->bottom - rect->top < 0)
		return;

	const RenderMode render_mode = (colour_key ? MODE_DRAW_SURFACE_WITH_TRANSPARENCY : MODE_DRAW_SURFACE);

	if (last_render_mode != render_mode || last_source_surface != source_surface || last_destination_surface != destination_surface)
	{
		FlushVertexBuffer();

		last_render_mode = render_mode;
		last_source_surface = source_surface;
		last_destination_surface = destination_surface;

		// Point our framebuffer to the destination texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, destination_surface->texture_id, 0);
		glViewport(0, 0, destination_surface->width, destination_surface->height);

		// Switch to colour-key shader if we have to
		glUseProgram(colour_key ? program_texture_colour_key : program_texture);

		glDisable(GL_BLEND);

		// Enable texture coordinates, since this uses textures
		glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, source_surface->texture_id);
	}

	const GLfloat texture_left = (GLfloat)rect->left / (GLfloat)source_surface->width;
	const GLfloat texture_right = (GLfloat)rect->right / (GLfloat)source_surface->width;
	const GLfloat texture_top = (GLfloat)rect->top / (GLfloat)source_surface->height;
	const GLfloat texture_bottom = (GLfloat)rect->bottom / (GLfloat)source_surface->height;

	const GLfloat vertex_left = (x * (2.0f / destination_surface->width)) - 1.0f;
	const GLfloat vertex_right = ((x + (rect->right - rect->left)) * (2.0f / destination_surface->width)) - 1.0f;
	const GLfloat vertex_top = (y * (2.0f / destination_surface->height)) - 1.0f;
	const GLfloat vertex_bottom = ((y + (rect->bottom - rect->top)) * (2.0f / destination_surface->height)) - 1.0f;

	VertexBufferSlot *vertex_buffer_slot = GetVertexBufferSlot();

	vertex_buffer_slot->vertices[0][0].texture_coordinate.x = texture_left;
	vertex_buffer_slot->vertices[0][0].texture_coordinate.y = texture_top;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.x = texture_right;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.y = texture_top;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.x = texture_right;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.y = texture_bottom;

	vertex_buffer_slot->vertices[1][0].texture_coordinate.x = texture_left;
	vertex_buffer_slot->vertices[1][0].texture_coordinate.y = texture_top;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.x = texture_right;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.y = texture_bottom;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.x = texture_left;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.y = texture_bottom;

	vertex_buffer_slot->vertices[0][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[0][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.y = vertex_bottom;

	vertex_buffer_slot->vertices[1][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.y = vertex_bottom;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.y = vertex_bottom;
}

void Backend_ColourFill(Backend_Surface *surface, const RECT *rect, unsigned char red, unsigned char green, unsigned char blue)
{
	static Backend_Surface *last_surface;
	static unsigned char last_red;
	static unsigned char last_green;
	static unsigned char last_blue;

	if (surface == NULL)
		return;

	if (rect->right - rect->left < 0 || rect->bottom - rect->top < 0)
		return;

	if (last_render_mode != MODE_COLOUR_FILL || last_surface != surface || last_red != red || last_green != green || last_blue != blue)
	{
		FlushVertexBuffer();

		last_render_mode = MODE_COLOUR_FILL;
		last_surface = surface;
		last_red = red;
		last_green = green;
		last_blue = blue;

		// Point our framebuffer to the destination texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, surface->texture_id, 0);
		glViewport(0, 0, surface->width, surface->height);

		glUseProgram(program_colour_fill);

		glDisable(GL_BLEND);

		// Disable texture coordinate array, since this doesn't use textures
		glDisableVertexAttribArray(2);

		glUniform4f(program_colour_fill_uniform_colour, red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);
	}

	const GLfloat vertex_left = (rect->left * (2.0f / surface->width)) - 1.0f;
	const GLfloat vertex_right = (rect->right * (2.0f / surface->width)) - 1.0f;
	const GLfloat vertex_top = (rect->top * (2.0f / surface->height)) - 1.0f;
	const GLfloat vertex_bottom = (rect->bottom * (2.0f / surface->height)) - 1.0f;

	VertexBufferSlot *vertex_buffer_slot = GetVertexBufferSlot();

	vertex_buffer_slot->vertices[0][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[0][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.y = vertex_bottom;

	vertex_buffer_slot->vertices[1][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.y = vertex_bottom;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.y = vertex_bottom;
}

BOOL Backend_SupportsSubpixelGlyphs(void)
{
	return TRUE;
}

Backend_Glyph* Backend_LoadGlyph(const unsigned char *pixels, unsigned int width, unsigned int height, int pitch, FontPixelMode pixel_mode)
{
	Backend_Glyph *glyph = (Backend_Glyph*)malloc(sizeof(Backend_Glyph));

	if (glyph != NULL)
	{
		const unsigned int destination_pitch = (width + 3) & ~3;	// Round up to the nearest 4 (OpenGL needs this)

		unsigned char *buffer = (unsigned char*)malloc(destination_pitch * height);

		if (buffer != NULL)
		{
			switch (pixel_mode)
			{
				case FONT_PIXEL_MODE_LCD:
				case FONT_PIXEL_MODE_GRAY:
					for (unsigned int y = 0; y < height; ++y)
					{
						const unsigned char *source_pointer = pixels + y * pitch;
						unsigned char *destination_pointer = buffer + y * destination_pitch;
						memcpy(destination_pointer, source_pointer, width);
					}

					break;

				case FONT_PIXEL_MODE_MONO:
					for (unsigned int y = 0; y < height; ++y)
					{
						const unsigned char *source_pointer = pixels + y * pitch;
						unsigned char *destination_pointer = buffer + y * destination_pitch;

						for (unsigned int x = 0; x < width; ++x)
							*destination_pointer++ = (*source_pointer++ ? 0xFF : 0);
					}

					break;
			}

			GLint previously_bound_texture;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &previously_bound_texture);

			glGenTextures(1, &glyph->texture_id);
			glBindTexture(GL_TEXTURE_2D, glyph->texture_id);
		#ifdef USE_OPENGLES2
			if (pixel_mode == FONT_PIXEL_MODE_LCD)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width / 3, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, buffer);
		#else
			if (pixel_mode == FONT_PIXEL_MODE_LCD)
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width / 3, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
		#endif

			free(buffer);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		#ifndef USE_OPENGLES2
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		#endif

			glBindTexture(GL_TEXTURE_2D, previously_bound_texture);

			glyph->width = (pixel_mode == FONT_PIXEL_MODE_LCD ? width / 3 : width);
			glyph->height = height;
			glyph->pixel_mode = pixel_mode;

			return glyph;
		}

		free(glyph);
	}

	return NULL;
}

void Backend_UnloadGlyph(Backend_Glyph *glyph)
{
	if (glyph == NULL)
		return;

	glDeleteTextures(1, &glyph->texture_id);
	free(glyph);
}

void Backend_DrawGlyph(Backend_Surface *surface, Backend_Glyph *glyph, long x, long y, const unsigned char *colours)
{
	static Backend_Surface *last_surface;
	static Backend_Glyph *last_glyph;
	static unsigned char last_red;
	static unsigned char last_green;
	static unsigned char last_blue;

	if (glyph == NULL || surface == NULL)
		return;

	const RenderMode render_mode = (glyph->pixel_mode == FONT_PIXEL_MODE_LCD ? MODE_DRAW_GLYPH_LCD : MODE_DRAW_GLYPH);

	if (last_render_mode != render_mode || last_surface != surface || last_glyph != glyph || last_red != colours[0] || last_green != colours[1] || last_blue != colours[2])
	{
		FlushVertexBuffer();

		last_render_mode = render_mode;
		last_surface = surface;
		last_glyph = glyph;
		last_red = colours[0];
		last_green = colours[1];
		last_blue = colours[2];

		if (glyph->pixel_mode == FONT_PIXEL_MODE_LCD)
		{
			glUseProgram(program_glyph_subpixel_part2);
			glUniform4f(program_glyph_subpixel_part2_uniform_colour, colours[0] / 255.0f, colours[1] / 255.0f, colours[2] / 255.0f, 1.0f);
		}
		else
		{
			glUseProgram(program_glyph_normal);
			glUniform4f(program_glyph_normal_uniform_colour, colours[0] / 255.0f, colours[1] / 255.0f, colours[2] / 255.0f, 1.0f);
		}

		// Point our framebuffer to the destination texture
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, surface->texture_id, 0);
		glViewport(0, 0, surface->width, surface->height);

		glEnable(GL_BLEND);

		// Enable texture coordinates, since this uses textures
		glEnableVertexAttribArray(2);

		glBindTexture(GL_TEXTURE_2D, glyph->texture_id);
	}

	const GLfloat vertex_left = (x * (2.0f / surface->width)) - 1.0f;
	const GLfloat vertex_right = ((x + glyph->width) * (2.0f / surface->width)) - 1.0f;
	const GLfloat vertex_top = (y * (2.0f / surface->height)) - 1.0f;
	const GLfloat vertex_bottom = ((y + glyph->height) * (2.0f / surface->height)) - 1.0f;

	VertexBufferSlot *vertex_buffer_slot = GetVertexBufferSlot();

	vertex_buffer_slot->vertices[0][0].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[0][0].texture_coordinate.y = 0.0f;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][1].texture_coordinate.y = 0.0f;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[0][2].texture_coordinate.y = 1.0f;

	vertex_buffer_slot->vertices[1][0].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[1][0].texture_coordinate.y = 0.0f;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.x = 1.0f;
	vertex_buffer_slot->vertices[1][1].texture_coordinate.y = 1.0f;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.x = 0.0f;
	vertex_buffer_slot->vertices[1][2].texture_coordinate.y = 1.0f;

	vertex_buffer_slot->vertices[0][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[0][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][1].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[0][2].vertex_coordinate.y = vertex_bottom;

	vertex_buffer_slot->vertices[1][0].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][0].vertex_coordinate.y = vertex_top;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.x = vertex_right;
	vertex_buffer_slot->vertices[1][1].vertex_coordinate.y = vertex_bottom;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.x = vertex_left;
	vertex_buffer_slot->vertices[1][2].vertex_coordinate.y = vertex_bottom;
}

void Backend_HandleRenderTargetLoss(void)
{
	// No problem for us
}

void Backend_HandleWindowResize(void)
{
	// No problem for us
}
