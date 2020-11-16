/*---------------------------------------------------------------------------------

	Sound Functions

	Copyright (C) 2005
		Dave Murphy (WinterMute)

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any
	damages arising from the use of this software.

	Permission is granted to anyone to use this software for any
	purpose, including commercial applications, and to alter it and
	redistribute it freely, subject to the following restrictions:

	1.	The origin of this software must not be misrepresented; you
		must not claim that you wrote the original software. If you use
		this software in a product, an acknowledgment in the product
		documentation would be appreciated but is not required.
	2.	Altered source versions must be plainly marked as such, and
		must not be misrepresented as being the original software.
	3.	This notice may not be removed or altered from any source
		distribution.

---------------------------------------------------------------------------------*/


//This file has been modified from the original source.
//https://github.com/devkitPro/libnds/blob/master/include/nds/arm9/sound.h

#ifndef _sound_cs_h_
#define _sound_cs_h_

/*! \file sound.h
	\brief A simple sound playback library for the DS.  Provides functionality
	for starting and stopping sound effects from the ARM9 side as well as access
	to PSG and noise hardware.  Maxmod should be used in most music and sound effect
	situations.
*/


#ifdef __cplusplus
extern "C" {
#endif

#include <nds/ndstypes.h>
void soundPlaySampleC(const void* data, SoundFormat format, u32 dataSize, u16 freq, u8 volume, u8 pan, bool loop, u16 loopPoint, u8 channel);

/*! \fn int soundPlayPSG(DutyCycle cycle, u16 freq, u8 volume, u8 pan);
	\brief Pause a tone with the specified properties
	\param cycle The DutyCycle of the sound wave
	\param freq The frequency in Hz of the sample
	\param volume The channel volume.  0 to 127 (min to max)
	\param pan The channel pan 0 to 127 (left to right with 64 being centered)
	\return An integer id coresponding to the channel of playback.  This value can be used to pause, resume, or kill the sound
	as well as adjust volume, pan, and frequency
*/

#ifdef __cplusplus
}
#endif

#endif // _sound_h_
