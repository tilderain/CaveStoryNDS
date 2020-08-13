#include "Sound.h"

#include <algorithm>
#include <math.h>
#include <stdint.h>
#include <string.h>


#include "Organya.h"
#include "PixTone.h"

#include "nds.h"
#include <maxmod9.h>

#include "stdio.h"

#include "NDSSoundSpec.h"

#define clamp(x, y, z) ((x > z) ? z : (x < y) ? y : x)

static long mixer_buffer[SND_BUFFERSIZE * 2];

//Keep track of all existing sound buffers
SOUNDBUFFER *soundBuffers;

//Sound buffer code
SOUNDBUFFER::SOUNDBUFFER(size_t bufSize)
{
	//Set parameters
	size = bufSize;
	
	playing = false;
	looping = false;
	looped = false;
	
	frequency = 0.0;
	volume = 1.0;
	volume_l = 1.0;
	volume_r = 1.0;
	samplePosition = 0.0;
	
	//Create waveform buffer
	data = new uint8_t[bufSize];
	memset(data, 0x80, bufSize);
	
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
	delete this;
}

void SOUNDBUFFER::Lock(uint8_t **outBuffer, size_t *outSize)
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
	frequency = (double)dwFrequency;
}

float MillibelToVolume(int32_t lVolume)
{
	//Volume is in hundredths of decibels, from 0 to -10000
	lVolume = clamp(lVolume, (int32_t)-10000, (int32_t)0);
	return (float)pow(10.0, lVolume / 2000.0);
}

void SOUNDBUFFER::SetVolume(int32_t lVolume)
{
	volume = MillibelToVolume(lVolume);
}

void SOUNDBUFFER::SetPan(int32_t lPan)
{
	volume_l = MillibelToVolume(-lPan);
	volume_r = MillibelToVolume(lPan);
}

void SOUNDBUFFER::Play(bool bLooping)
{
	playing = true;
	looping = bLooping;
}

void SOUNDBUFFER::Stop()
{
	playing = false;
}

void SOUNDBUFFER::Mix(long *stream, uint32_t samples)
{
	
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
	}
	
}

//Sound things
SOUNDBUFFER* lpSECONDARYBUFFER[SE_MAX];

mm_word StreamCallback(mm_word length, mm_addr dest, mm_stream_formats format)
{
	
	s16 *stream = (s16*)dest;
	int len = length;
	//len very important
	uint32_t samples = len;

	for (unsigned int i = 0; i < len * 2; ++i)
		mixer_buffer[i] = 0;

	//Mix sounds to primary buffer
	for (SOUNDBUFFER *sound = soundBuffers; sound != NULL; sound = sound->next)
		sound->Mix(mixer_buffer, samples);

	for (unsigned int i = 0; i < len * 2; ++i)
		stream[i] = (int16_t)(clamp(mixer_buffer[i], -0xFF, 0xFF) << 8);

	//im not sure if this actually does anything
	DC_FlushRange(dest, len);
	
	//Play organya
	gOrgTimer += samples;
	
	if (gOrgTimer > gOrgSamplePerStep)
	{
		OrganyaPlayData();
		gOrgTimer %= gOrgSamplePerStep;
	}
	
	return length;
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

	sys.mod_count 			= 0;
	sys.samp_count			= 0;
	sys.mem_bank			= 0;
	sys.fifo_channel		= FIFO_MAXMOD;
	mmInit( &sys );
	

	mystream.sampling_rate	= DSP_DEFAULT_FREQ;					// sampling rate = 25khz
	mystream.buffer_length	= SND_BUFFERSIZE;						// buffer length = 1200 samples
	mystream.callback		= StreamCallback;		// set callback function
	mystream.format			= MM_STREAM_16BIT_STEREO;	// format = stereo 16-bit
	mystream.timer			= MM_TIMER0;				// use hardware timer 0
	mystream.manual			= true;					// use manual filling
	mmStreamOpen( &mystream );
	
	SetYtrigger( 0 );
	irqEnable( IRQ_VCOUNT );
	
	//StartOrganya();

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
	memset(pcm_buffer, 0x80, sample_count);
	memset(mixed_pcm_buffer, 0x80, sample_count);

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
			if (pcm_buffer[j] + mixed_pcm_buffer[j] - 0x100 < -0x7F)
				mixed_pcm_buffer[j] = 0;
			else if (pcm_buffer[j] + mixed_pcm_buffer[j] - 0x100 > 0x7F)
				mixed_pcm_buffer[j] = 0xFF;
			else
				mixed_pcm_buffer[j] += pcm_buffer[j] + -0x80;
		}
	}

	lpSECONDARYBUFFER[no] = new SOUNDBUFFER(sample_count);

	unsigned char *buf;
	lpSECONDARYBUFFER[no]->Lock(&buf, NULL);
	memcpy(buf, mixed_pcm_buffer, sample_count);
	lpSECONDARYBUFFER[no]->Unlock();
	lpSECONDARYBUFFER[no]->SetFrequency(22050);

	free(pcm_buffer);
	free(mixed_pcm_buffer);

	return sample_count;
}
