#pragma once

#include <stdio.h>

#ifdef READ_FROM_SD
#define FILE_e FILE
#define fopen_embed fopen
#define fclose_embed fclose
#define fread_embed fread
#else

typedef struct{
	const unsigned char *file;
	const unsigned char *point;
	unsigned int size;
	unsigned int position;
}FILE_e;

FILE_e *fopen_embed(const char *file, const char *mode);
void fclose_embed(FILE_e *fp);
size_t fread_embed(void *ptr, size_t size, size_t nmemb, FILE_e *fp);

#endif