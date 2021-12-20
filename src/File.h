// Released under the MIT licence.
// See LICENCE.txt for details.

#pragma once

#include <stddef.h>


#include "fopen.h"

long LoadFileToMemory(const char *file_path, unsigned char **file_buffer);

#ifndef READ_FROM_SD
unsigned char File_ReadU8(FILE_e *stream);
unsigned short File_ReadLE16(FILE_e *stream);
unsigned long File_ReadLE32(FILE_e *stream);
#endif

unsigned char File_ReadU8(FILE *stream);
unsigned short File_ReadBE16(FILE *stream);
unsigned long File_ReadBE32(FILE *stream);
unsigned short File_ReadLE16(FILE *stream);
unsigned long File_ReadLE32(FILE *stream);

void File_WriteBE16(unsigned short value, FILE *stream);
void File_WriteBE32(unsigned long value, FILE *stream);
void File_WriteLE16(unsigned short value, FILE *stream);
void File_WriteLE32(unsigned long value, FILE *stream);
