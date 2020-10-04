#pragma once

#include <stdio.h>

typedef struct{
	const unsigned char *file;
	const unsigned char *point;
	unsigned int size;
	unsigned int position;
}FILE_e;

FILE_e *fopen_embed(const char *file, const char *mode);
void fclose_embed(FILE_e *fp);
size_t fread_embed(void *ptr, size_t size, size_t nmemb, FILE_e *fp);
