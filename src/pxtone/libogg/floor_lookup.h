/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis 'TREMOR' CODEC SOURCE CODE.   *
 *                                                                  *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis 'TREMOR' SOURCE CODE IS (C) COPYRIGHT 1994-2003    *
 * BY THE Xiph.Org FOUNDATION http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: floor dB lookup

 ********************************************************************/
#ifndef _floor_lookup_h
#define _floor_lookup_h

#include "os.h"

#ifdef _LOW_ACCURACY_
#  define XdB(n) ((((n)>>8)+1)>>1)
#else
#  define XdB(n) (n)
#endif

extern const ogg_int32_t FLOOR_fromdB_LOOKUP[256];
#endif
