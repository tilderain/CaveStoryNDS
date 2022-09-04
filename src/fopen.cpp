#include <cstring>
#include <stdlib.h>

#ifndef READ_FROM_SD

//File references
#include "../incbin.h"
#define INCBIN_PREFIX g

typedef struct {
	const unsigned char *data;
	unsigned int size;
	const char *path;
} FILEREF;

#include "fopen.h"

// we are in the "build" folder
#include "fopenincbins.h"

//File struct functions
FILE_e *fopen_embed(const char *file, const char *mode)
{
	//Get our referenced file
	const FILEREF *fileref = NULL;
	FILE_e *fp = NULL;
	int i;
	
	for (i = 0; i < sizeof(files) / sizeof(FILEREF); i++)
	{
		if (!strcasecmp(file, files[i].path))
		{
			fileref = &files[i];
			break;
		}
			
	}
	
	if (fileref == NULL)
		return NULL;
	
	//Allocate file struct and construct
	fp = (FILE_e*)malloc(sizeof(FILE_e));
	fp->file = fileref->data;
	fp->point = fp->file;
	fp->size = fileref->size;
	fp->position = 0;
	
	return fp;
}

void fclose_embed(FILE_e *fp)
{
	//Free file struct
	free(fp);
}

size_t fread_embed(void *ptr, size_t size, size_t nmemb, FILE_e *fp)
{
	//Lazy memcpy
	memcpy(ptr, fp->point + fp->position, size * nmemb);
	fp->position += size * nmemb;
	return nmemb;
}

#endif