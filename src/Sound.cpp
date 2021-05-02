// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

// Some of the original source code for this file can be found here:
// https://github.com/shbow/organya/blob/master/source/Sound.cpp

/*
TODO - Code style
Pixel's code was *extremely* Windows-centric, to the point of using
things like ZeroMemory and LPCSTR instead of standard things like
memset and const char*. For now, the decompilation is accurate despite
not using these since they're just macros that evaluate to the portable
equivalents.
*/

#include "Sound.h"

#include <algorithm>
#include <math.h>
#include <stdint.h>
#include <string.h>


#include "Organya.h"
#include "PixTone.h"

#include "nds.h"
#include "soundFifo.h"
#include <maxmod9.h>

#include "stdio.h"

#include "NDSSoundSpec.h"

#include "Draw.h"
#include "Game.h"

#include "File.h"
#include "Main.h"

#include "fopen.h"

#include "Generic.h"

#define clamp(x, y, z) ((x > z) ? z : (x < y) ? y : x)

//static long mixer_buffer[SND_BUFFERSIZE * 2];
#define NUM_CHANNELS 16

SOUNDBUFFER* channelStates[NUM_CHANNELS] = {NULL};

s8 getFreeChannel(void)
{
	for(u8 i=0;i<NUM_CHANNELS;i++)
	{	
		if(!channelStates[i])
		{
			return i;
		}
			
	}
	return -1;
}



//Keep track of all existing sound buffers
SOUNDBUFFER *soundBuffers;


void updateChannelStates(void)
{
	//if a one-shot sound has passed all of its samples already, make channelStates[channel] free
	for (int i = 0; i < NUM_CHANNELS; i++)
	{
		SOUNDBUFFER *sound = channelStates[i];
		if(sound && sound->playing && sound->looping == false)
		{
			if(sound->timer++ > sound->endTimer) // samples in a frame plus a bit
			{
				// TODO: give organbuffer priority
				sound->playing = false;
				channelStates[sound->channelId] = NULL;
				soundKill(sound->channelId);
				sound->channelId = -1;
				sound->timer = 0;
				sound->endTimer = 0;
			}
		}
	}
}

//Sound buffer code
SOUNDBUFFER::SOUNDBUFFER(size_t bufSize, const void* data_ptr)
{
	//Set parameters
	size = bufSize;
	
	playing = false;
	looping = false;
	looped = false;

	timer = 0;
	endTimer = 0;
	
	frequency = 22050;
	volume = 127;
	pan = 63;
	samplePosition = 0;
	channelId = -1;
	adpcm = false;
	
	//Create waveform buffer
	if(!data_ptr)
	{
		data = new s8[bufSize];
		memset(data, 0x80, bufSize);
	}
	else
	{
		// skip swav header
		int padding = 36;
		data = (s8*)data_ptr+padding;
		size = size - padding;
		adpcm = true;
	}
	
	//Add to buffer list
	this->next = soundBuffers;
	soundBuffers = this;
}

SOUNDBUFFER::~SOUNDBUFFER()
{
	//Free buffer
	if (data)
		delete[] data;
	
	//Remove from buffer list
	for (SOUNDBUFFER **soundBuffer = &soundBuffers; *soundBuffer != NULL; soundBuffer = &(*soundBuffer)->next)
	{
		if (*soundBuffer == this)
		{
			*soundBuffer = this->next;
			break;
		}
	}
}

void SOUNDBUFFER::Release()
{
	//TODO: find a better and more stable(?) way to handle this function
	if(channelId != -1) 
	{
		soundKill(channelId);
		channelStates[channelId] = NULL;
	}
	delete this;
}

void SOUNDBUFFER::Lock(s8 **outBuffer, size_t *outSize)
{
	if (outBuffer != NULL)
		*outBuffer = data;

	if (outSize != NULL)
		*outSize = size;
}

void SOUNDBUFFER::Unlock()
{
	//Nothing
}

void SOUNDBUFFER::SetCurrentPosition(uint32_t dwNewPosition)
{
	samplePosition = dwNewPosition;
}

void SOUNDBUFFER::SetFrequency(uint32_t dwFrequency)
{
	frequency = dwFrequency;
	//SCHANNEL_TIMER(channelId) = SOUND_FREQ(frequency);
	if (channelId == -1) return;
	soundSetFreq(channelId, (u16)frequency);
}

float MillibelToVolume(int32_t lVolume)
{
	//Volume is in hundredths of decibels, from 0 to -10000

	lVolume = clamp(lVolume, (int32_t)-10000, (int32_t)0);
	return (float)pow(10.0, lVolume / 2000.0);
}

void SOUNDBUFFER::SetVolume(int32_t lVolume)
{
	float vol = MillibelToVolume(lVolume);
	volume = (vol * 127);
	
	//SCHANNEL_CR(channelId) = SCHANNEL_ENABLE | SOUND_FORMAT_8BIT | SOUND_VOL(lVolume);
	if (channelId == -1) return;
	soundSetVolume(channelId, volume);
}

void SOUNDBUFFER::SetPan(int32_t lPan)
{
	pan = (int)((double)lPan / 512.0 * 127.0); // pan_tbl max to nds 127 pan max
	if (channelId == -1) return;
	soundSetPan(channelId, pan);
}

int played = false;

void SOUNDBUFFER::Play(bool bLooping)
{

	if (!data) return;
	playing = true;
	looping = bLooping;

	if (channelId != -1 && size < 257 && !looping) 
	{
		//org for some reason sends a play message without looping for stopping..
		//so it is better to just cut it off here rather than start a new sound
		soundKill(channelId);
		channelStates[channelId] = NULL;
		channelId = -1;
		return;
	}
	if (channelId == -1)
	{
		channelId = getFreeChannel();
	}

	if(channelId == -1) return;
	channelStates[channelId] = this;

	//printf() pan
	SoundFormat format = (adpcm ? SoundFormat_ADPCM : SoundFormat_8Bit);
	soundPlaySampleC(data, format, (u32)size, (u16)frequency, (u8)volume, (u8)pan, looping, (u16)0, channelId);
	timer = 0;
	endTimer = size * (adpcm?2:1) / (frequency / 63);
}

void SOUNDBUFFER::Stop()
{
	playing = false;
	if(channelId != -1) soundKill(channelId);
	channelStates[channelId] = NULL;
	channelId = -1;
}

void killAllSounds()
{
	soundMicOff();

	for (int i=0;i<NUM_CHANNELS;i++)
	{
		channelStates[i] = 0;
	}

	for (SOUNDBUFFER *sound = soundBuffers; sound != NULL; sound = sound->next)
	{
		sound->playing = false;
		sound->channelId = -1;
		sound->timer = 0;
	}
}

void SOUNDBUFFER::Mix(long *stream, uint32_t samples)
{
	/*
	if (!playing) //This sound buffer isn't playing
		return;
	
	for (size_t sample = 0; sample < samples; sample++)
	{
		const double freqPosition = frequency / DSP_DEFAULT_FREQ; //This is added to position at the end
		
		//Get the in-between sample this is (linear interpolation)
		const uint8_t sample1 = ((looped || ((size_t)samplePosition) >= 1) ? data[(size_t)samplePosition] : 0x80);
		const uint8_t sample2 = ((looping || (((size_t)samplePosition) + 1) < size) ? data[(((size_t)samplePosition) + 1) % size] : 0x80);

		//Interpolate sample
		const float subPos = (float)fmod(samplePosition, 1.0);
		const uint8_t sampleA = sample1 + (sample2 - sample1) * subPos;

		//Convert sample to int8_t
		const int8_t sampleConvert = (sampleA - 0x80);

		//Mix
		stream[sample * 2] += (long)(sampleConvert * volume * volume_l);
		stream[sample * 2 + 1] += (long)(sampleConvert * volume * volume_r);
		
		//Increment position
		samplePosition += freqPosition;
		
		if (samplePosition >= size)
		{
			if (looping)
			{
				samplePosition = fmod(samplePosition, size);
				looped = true;
			}
			else
			{
				samplePosition = 0.0;
				playing = false;
				looped = false;
				break;
			}
		}
	}*/
	
}

//Sound things
SOUNDBUFFER* lpSECONDARYBUFFER[SE_MAX];

int gVBlankCounter = 0;
void DoOrganya(void)
{
	gOrgTimer += SND_BUFFERSIZE;
	
	if (gOrgTimer > gOrgSamplePerStep)
	{
		OrganyaPlayData();
		gOrgTimer %= gOrgSamplePerStep;
	}
	updateChannelStates();
	gVBlankCounter++;
}

mm_stream mystream;
mm_ds_system sys;

bool InitDirectSound()
{
	//Init sound library
	/*
	AESND_Init();
	AESND_Pause(false);
	
	//Set-up stream
	AESND_RegisterAudioCallback(StreamCallback);
	*/
	//Start organya
	
	//----------------------------------------------------------------
	// initialize maxmod without any soundbank (unusual setup)
	//----------------------------------------------------------------
	
	//SetYtrigger( 0 );
	//irqEnable( IRQ_VCOUNT );
	
	StartOrganya();

	soundEnable();
	irqEnable(IRQ_VBLANK);
	irqSet(IRQ_VBLANK, DoOrganya);

	return true;
}

void EndDirectSound()
{
	//End organya
	EndOrganya();
}

//Sound effects playing
void PlaySoundObject(int no, int mode)
{
	if (lpSECONDARYBUFFER[no])
	{
		if (mode == -1)
		{
			lpSECONDARYBUFFER[no]->Play(true);
		}
		else if ( mode )
		{
			if ( mode == 1 )
			{
				lpSECONDARYBUFFER[no]->Stop();
				lpSECONDARYBUFFER[no]->SetCurrentPosition(0);
				lpSECONDARYBUFFER[no]->Play(false);
			}
		}
		else
		{
			lpSECONDARYBUFFER[no]->Stop();
		}
	}
}

void ChangeSoundFrequency(int no, uint32_t rate)
{
	if (lpSECONDARYBUFFER[no])
		lpSECONDARYBUFFER[no]->SetFrequency(10 * rate + 100);
}

void ChangeSoundVolume(int no, int32_t volume)
{
	if (lpSECONDARYBUFFER[no])
		lpSECONDARYBUFFER[no]->SetVolume(8 * volume - 2400);
}

void ChangeSoundPan(int no, int32_t pan)
{
	if (lpSECONDARYBUFFER[no])
		lpSECONDARYBUFFER[no]->SetPan(10 * (pan - 256));
}

size_t MakePixToneObject(const PIXTONEPARAMETER *ptp, int ptp_num, int no)
{
	int sample_count = 0;
	for (int i = 0; i < ptp_num; ++i)
	{
		if (ptp[i].size > sample_count)
			sample_count = ptp[i].size;
	}

	unsigned char *pcm_buffer = (unsigned char*)malloc(sample_count);
	unsigned char *mixed_pcm_buffer = (unsigned char*)malloc(sample_count);
	memset(pcm_buffer, 0, sample_count);
	memset(mixed_pcm_buffer, 0, sample_count);

	for (int i = 0; i < ptp_num; ++i)
	{
		if (!MakePixelWaveData(&ptp[i], pcm_buffer))
		{
			free(pcm_buffer);
			free(mixed_pcm_buffer);
			return -1;
		}

		for (int j = 0; j < ptp[i].size; ++j)
		{
            if ((signed char)pcm_buffer[j] + (signed char)mixed_pcm_buffer[j] < -0x7F)
                mixed_pcm_buffer[j] = -0x7F;
            else if ((signed char)pcm_buffer[j] + (signed char)mixed_pcm_buffer[j] > 0x7F)
                mixed_pcm_buffer[j] = 0x7F;
            else
                mixed_pcm_buffer[j] = mixed_pcm_buffer[j] + pcm_buffer[j];
		}
	}

	lpSECONDARYBUFFER[no] = new SOUNDBUFFER(sample_count, NULL);

	s8 *buf;
	lpSECONDARYBUFFER[no]->Lock(&buf, NULL);
	memcpy(buf, mixed_pcm_buffer, sample_count);
	lpSECONDARYBUFFER[no]->Unlock();
	lpSECONDARYBUFFER[no]->SetFrequency(22050);

	free(pcm_buffer);
	free(mixed_pcm_buffer);

	return sample_count;
}

BOOL ReadSound(int no)
{
    //Get file path
    char path[MAX_PATH];
    sprintf(path, "%s/Wave/%03d.swav", gDataPath, no);
    
    //Open file
    FILE_e *fp = fopen_embed(path, "rb");
    if (fp == NULL)
        return FALSE;
    
    //Create buffer
#ifndef READ_FROM_SD
    lpSECONDARYBUFFER[no] = new SOUNDBUFFER(fp->size, fp->file);
#else
	int size = GetFileSizeLong(path);
    signed char *data = (signed char *)malloc(size);
	fread(data, size, 1, fp);

	lpSECONDARYBUFFER[no] = new SOUNDBUFFER(size, NULL);

	if (lpSECONDARYBUFFER[no] == NULL)
        return FALSE;

	s8 *buf;
    lpSECONDARYBUFFER[no]->Lock(&buf, NULL);
    memcpy(buf, data, size);
    lpSECONDARYBUFFER[no]->Unlock();
    lpSECONDARYBUFFER[no]->SetFrequency(22050);
	free(data);
#endif

    if (lpSECONDARYBUFFER[no] == NULL)
        return FALSE;
	
    fclose_embed(fp);    
    return TRUE;
}