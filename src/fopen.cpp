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

#include "nds.h"

//File struct functions
FILE_e *fopen_embed(const char *file, const char *mode)
{
	//Get our referenced file
	const FILEREF *fileref = NULL;
	FILE_e *fp = NULL;
	int i;

	bool lzssmode = false;

	char temp[255] = "";
	
	for (i = 0; i < sizeof(files) / sizeof(FILEREF); i++)
	{
		if (!strcasecmp(file, files[i].path))
		{
			fileref = &files[i];
			break;
		}
		
		//compressed file
		strcpy(temp, file);
		strcat(temp, ".lzss");
		//printf("trying to laod %s \n", temp);
		if (!strcasecmp(temp, files[i].path))
		{
			lzssmode = true;
			fileref = &files[i];
			break;
		}
	}
	
	if (fileref == NULL)
		return NULL;
	
	//Allocate file struct and construct
	fp = (FILE_e*)malloc(sizeof(FILE_e));
	fp->compressed = false;
	if(lzssmode)
	{
		fp->compressed = true;
		u32 header = *(u32*)fileref->data;
		int size = header >> 8;

		void* data = malloc(size);
		swiDecompressLZSSWram((void*)fileref->data, data);

		fp->file = (unsigned char*)data;
		fp->point = fp->file;
		fp->size = size;

	}
	else
	{
		fp->file = fileref->data;
		fp->point = fp->file;
		fp->size = fileref->size;
	}


	fp->position = 0;
	
	return fp;
}

void fclose_embed(FILE_e *fp)
{
	//Free file struct

	if(fp->compressed)
	{
		free((void*)fp->file);
	}
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