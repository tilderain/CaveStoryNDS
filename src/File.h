#pragma once

#include <stddef.h>
#include <stdio.h>

#include "fopen.h"

unsigned char* LoadFileToMemory(const char *file_path, size_t *file_size);


unsigned char File_ReadU8(FILE_e *stream);
unsigned short File_ReadLE16(FILE_e *stream);
unsigned long File_ReadLE32(FILE_e *stream);

unsigned char File_ReadU8(FILE *stream);
unsigned short File_ReadBE16(FILE *stream);
unsigned long File_ReadBE32(FILE *stream);
unsigned short File_ReadLE16(FILE *stream);
unsigned long File_ReadLE32(FILE *stream);

void File_WriteBE16(unsigned short value, FILE *stream);
void File_WriteBE32(unsigned long value, FILE *stream);
void File_WriteLE16(unsigned short value, FILE *stream);
void File_WriteLE32(unsigned long value, FILE *stream);
