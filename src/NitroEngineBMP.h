// SPDX-License-Identifier: MIT
//
// Copyright (c) 2008-2011, 2019, Antonio Niño Díaz
//
// This file is part of Nitro Engine


#pragma once

#include "fopen.h"
#include "Draw.h"

/*! \file   NEFormats.h
 *  \brief  Used to convert common formats into DS formats.
 */

/*! @defgroup formats Format coversion functions
 *
 * Right now, there are only functions to convert BMP images into DS textures.
 * They support BMP of 4, 8, 16 (X1RGB5) and 24 bits.
 *
 * @{
 */

/*! \struct NE_BMPHeader
 *  \brief  Header struct for a BMP file.
 */
typedef struct {
	u16 type;			/* Magic identifier            */
	u32 size;			/* File size in bytes          */
	u16 reserved1, reserved2;
	u32 offset;			/* Offset to image data, bytes */
} PACKED NE_BMPHeader;

/*! \struct NE_BMPInfoHeader
 *  \brief  Information struct of a BMP file. */
typedef struct {
	u32 size;			/* Header size in bytes      */
	u32 width, height;		/* Width and height of image */
	u16 planes;			/* Number of color planes   */
	u16 bits;			/* Bits per pixel            */
	u32 compression;		/* Compression type          */
	u32 imagesize;			/* Image size in bytes       */
	u32 xresolution, yresolution;	/* Pixels per meter          */
	u32 ncolors;			/* Number of colors         */
	u32 importantcolors;		/* Important colors         */
} PACKED NE_BMPInfoHeader;


BOOL LoadPalettedBMP(void* file_buffer, SurfaceID surf_no, bool create_surface);