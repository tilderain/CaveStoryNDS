// Released under the MIT licence.
// See LICENCE.txt for details.

#include "File.h"

#include <stddef.h>
#include <stdlib.h>
long LoadFileToMemory(const char *file_path, unsigned char **file_buffer)
{
	long returned_size = -1;
	long file_size = 0;

	FILE_e *file = fopen_embed(file_path, "rb");

	if (file != NULL)
	{
		
		#ifndef READ_FROM_SD
			file_size = file->size;
		#else
			fstat(fileno(fp), &file_descriptor);
			file_size = file_descriptor.st_size;
		#endif

			if (file_size >= 0)
			{
				*file_buffer = (unsigned char*)malloc(file_size);

				if (*file_buffer != NULL)
				{
					if (fread_embed(*file_buffer, file_size, 1, file) == 1)
						returned_size = file_size;
				}
			}
		

		fclose_embed(file);
	}

	return returned_size;
}

#ifndef READ_FROM_SD

unsigned char File_ReadU8(FILE_e *stream)
{
	unsigned char byte;
	
	fread_embed(&byte, 1, 1, stream);
	
	return byte;
}

unsigned short File_ReadLE16(FILE_e *stream)
{
	unsigned char bytes[2];

	fread_embed(bytes, 2, 1, stream);

	return (bytes[1] << 8) | bytes[0];
}

unsigned long File_ReadLE32(FILE_e *stream)
{
	unsigned char bytes[4];

	fread_embed(bytes, 4, 1, stream);

	return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

#endif

unsigned char File_ReadU8(FILE *stream)
{
	unsigned char byte;
	
	fread(&byte, 1, 1, stream);
	
	return byte;
}

unsigned short File_ReadBE16(FILE *stream)
{
	unsigned char bytes[2];

	fread(bytes, 2, 1, stream);

	return (bytes[0] << 8) | bytes[1];
}

unsigned long File_ReadBE32(FILE *stream)
{
	unsigned char bytes[4];

	fread(bytes, 4, 1, stream);

	return (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
}

unsigned short File_ReadLE16(FILE *stream)
{
	unsigned char bytes[2];

	fread(bytes, 2, 1, stream);

	return (bytes[1] << 8) | bytes[0];
}

unsigned long File_ReadLE32(FILE *stream)
{
	unsigned char bytes[4];

	fread(bytes, 4, 1, stream);

	return (bytes[3] << 24) | (bytes[2] << 16) | (bytes[1] << 8) | bytes[0];
}

void File_WriteBE16(unsigned short value, FILE *stream)
{
	for (unsigned int i = 2; i-- != 0;)
		fputc(value >> (8 * i), stream);
}

void File_WriteBE32(unsigned long value, FILE *stream)
{
	for (unsigned int i = 4; i-- != 0;)
		fputc(value >> (8 * i), stream);
}

void File_WriteLE16(unsigned short value, FILE *stream)
{
	for (unsigned int i = 0; i < 2; ++i)
		fputc(value >> (8 * i), stream);
}

void File_WriteLE32(unsigned long value, FILE *stream)
{
	for (unsigned int i = 0; i < 4; ++i)
		fputc(value >> (8 * i), stream);
}
