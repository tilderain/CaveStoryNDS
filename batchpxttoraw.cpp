// THIS IS DECOMPILED PROPRIETARY CODE - USE AT YOUR OWN RISK.
//
// The original code belongs to Daisuke "Pixel" Amaya.
//
// Modifications and custom code are under the MIT licence.
// See LICENCE.txt for details.

#include "src/PixTone.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "src/WindowsWrapper.h"

#include "src/Random.h"

typedef int8_t		s8;


class SOUNDBUFFER
{
	public:
		SOUNDBUFFER(size_t bufSize, const void* data_ptr);
		~SOUNDBUFFER();
		
		void Release();
		
		void Lock(s8 **buffer, size_t *size);
		void Unlock();
		
		void SetCurrentPosition(uint32_t dwNewPosition);
		void SetFrequency(uint32_t dwFrequency);
		void SetVolume(int32_t lVolume);
		void SetPan(int32_t lPan);
		void Play(bool bLooping);
		void Stop();
		
		void Mix(long *stream, uint32_t samples);
		
		SOUNDBUFFER *next;
	
	public:
		s8 *data;
		size_t size;
		
		bool playing;
		bool looping;
		bool looped;
		
		int timer;
		int endTimer;

		int frequency;
		int volume;
		int pan;
		
		int samplePosition;

		signed char channelId;

		bool adpcm;
};

SOUNDBUFFER *soundBuffers;

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

static unsigned long next = 1;

int msvc_rand(void)
{
	next = ((next) * 214013 + 2531011);
	return ((next) >> 16) & 0x7FFF;
}

void msvc_srand(unsigned int seed)
{
	next = seed;
}


SOUNDBUFFER* lpSECONDARYBUFFER[164];

signed char gWaveModelTable[6][0x100];

void MakeWaveTables(void)
{
	int i;

	int a;

	// Sine wave
	for (i = 0; i < 0x100; ++i)
	{
		gWaveModelTable[0][i] = (signed char)(sin((i * 6.283184) / 256.0) * 64.0);
		a = gWaveModelTable[0][i];	// I have no idea what this line was meant to do
	}

	// Triangle wave
	for (a = 0, i = 0; i < 0x40; ++i)
	{
		// Upwards
		gWaveModelTable[1][i] = (a * 0x40) / 0x40;
		++a;
	}
	for (a = 0; i < 0xC0; ++i)
	{
		// Downwards
		gWaveModelTable[1][i] = 0x40 - ((a * 0x40) / 0x40);
		++a;
	}
	for (a = 0; i < 0x100; ++i)
	{
		// Back up
		gWaveModelTable[1][i] = ((a * 0x40) / 0x40) - 0x40;
		++a;
	}

	// Saw up wave
	for (i = 0; i < 0x100; ++i)
		gWaveModelTable[2][i] = (i / 2) - 0x40;

	// Saw down wave
	for (i = 0; i < 0x100; ++i)
		gWaveModelTable[3][i] = 0x40 - (i / 2);

	// Square wave
	for (i = 0; i < 0x80; ++i)
		gWaveModelTable[4][i] = 0x40;
	for (; i < 0x100; ++i)
		gWaveModelTable[4][i] = -0x40;

	// White noise wave
	msvc_srand(0);
	for (i = 0; i < 0x100; ++i)
		gWaveModelTable[5][i] = (signed char)(msvc_rand() & 0xFF) / 2;
}

BOOL wave_tables_made;

BOOL MakePixelWaveData(const PIXTONEPARAMETER *ptp, unsigned char *pData)
{
	int i;
	int a, b, c, d;

	double dPitch;
	double dMain;
	double dVolume;

	double dEnvelope;
	signed char envelopeTable[0x100];

	double d1, d2, d3;

	// The Linux port added a cute optimisation here, where MakeWaveTables is only called once during the game's execution
	if (wave_tables_made != TRUE)
	{
		MakeWaveTables();
		wave_tables_made = TRUE;
	}

	memset(envelopeTable, 0, sizeof(envelopeTable));

	i = 0;

	dEnvelope = ptp->initial;
	while (i < ptp->pointAx)
	{
		envelopeTable[i] = (signed char)dEnvelope;
		dEnvelope = (((double)ptp->pointAy - ptp->initial) / ptp->pointAx) + dEnvelope;
		++i;
	}

	dEnvelope = ptp->pointAy;
	while (i < ptp->pointBx)
	{
		envelopeTable[i] = (signed char)dEnvelope;
		dEnvelope = (((double)ptp->pointBy - ptp->pointAy) / (double)(ptp->pointBx - ptp->pointAx)) + dEnvelope;
		++i;
	}

	dEnvelope = ptp->pointBy;
	while (i < ptp->pointCx)
	{
		envelopeTable[i] = (signed char)dEnvelope;
		dEnvelope = ((double)ptp->pointCy - ptp->pointBy) / (double)(ptp->pointCx - ptp->pointBx) + dEnvelope;
		++i;
	}

	dEnvelope = ptp->pointCy;
	while (i < 0x100)
	{
		envelopeTable[i] = (signed char)dEnvelope;
		dEnvelope = dEnvelope - (ptp->pointCy / (double)(0x100 - ptp->pointCx));
		++i;
	}

	dPitch = ptp->oPitch.offset;
	dMain = ptp->oMain.offset;
	dVolume = ptp->oVolume.offset;

	if (ptp->oMain.num == 0.0)
		d1 = 0.0;
	else
		d1 = 256.0 / (ptp->size / ptp->oMain.num);

	if (ptp->oPitch.num == 0.0)
		d2 = 0.0;
	else
		d2 = 256.0 / (ptp->size / ptp->oPitch.num);

	if (ptp->oVolume.num == 0.0)
		d3 = 0.0;
	else
		d3 = 256.0 / (ptp->size / ptp->oVolume.num);

	for (i = 0; i < ptp->size; ++i)
	{
		a = (int)dMain % 0x100;
		b = (int)dPitch % 0x100;
		c = (int)dVolume % 0x100;
		d = (int)((double)(i * 0x100) / ptp->size);
		pData[i] = gWaveModelTable[ptp->oMain.model][a]
		         * ptp->oMain.top
		         / 64
		         * (((gWaveModelTable[ptp->oVolume.model][c] * ptp->oVolume.top) / 64) + 64)
		         / 64
		         * envelopeTable[d]
		         / 64;

		if (gWaveModelTable[ptp->oPitch.model][b] < 0)
			dMain = d1 - d1 * 0.5 * -gWaveModelTable[ptp->oPitch.model][b] * ptp->oPitch.top / 64.0 / 64.0 + dMain;
		else
			dMain = d1 + d1 * 2.0 * gWaveModelTable[ptp->oPitch.model][b] * ptp->oPitch.top / 64.0 / 64.0 + dMain;

		dPitch = dPitch + d2;
		dVolume = dVolume + d3;
	}

	return TRUE;
}

void SOUNDBUFFER::SetFrequency(uint32_t dwFrequency)
{
	frequency = dwFrequency;
	//SCHANNEL_TIMER(channelId) = SOUND_FREQ(frequency);
	if (channelId == -1) return;
	//soundSetFreq(channelId, (u16)frequency);
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


enum
{
	SOUND_TYPE_PIXTONE,
	SOUND_TYPE_OTHER
};

static const struct
{
	int slot;
	const char *path;
	int type;
} ptp_table[] = {
	// TODO - Name the files
	{1, "PixTone/001.pxt", SOUND_TYPE_PIXTONE},
	{2, "PixTone/002.pxt", SOUND_TYPE_PIXTONE},
	{3, "PixTone/003.pxt", SOUND_TYPE_PIXTONE},
	{4, "PixTone/004.pxt", SOUND_TYPE_PIXTONE},
	{5, "PixTone/005.pxt", SOUND_TYPE_PIXTONE},
	{6, "PixTone/006.pxt", SOUND_TYPE_PIXTONE},
	{7, "PixTone/007.pxt", SOUND_TYPE_PIXTONE},
	{11, "PixTone/011.pxt", SOUND_TYPE_PIXTONE},
	{12, "PixTone/012.pxt", SOUND_TYPE_PIXTONE},
	{14, "PixTone/014.pxt", SOUND_TYPE_PIXTONE},
	{15, "PixTone/015.pxt", SOUND_TYPE_PIXTONE},
	{16, "PixTone/016.pxt", SOUND_TYPE_PIXTONE},
	{17, "PixTone/017.pxt", SOUND_TYPE_PIXTONE},
	{18, "PixTone/018.pxt", SOUND_TYPE_PIXTONE},
	{20, "PixTone/020.pxt", SOUND_TYPE_PIXTONE},
	{21, "PixTone/021.pxt", SOUND_TYPE_PIXTONE},
	{22, "PixTone/022.pxt", SOUND_TYPE_PIXTONE},
	{23, "PixTone/023.pxt", SOUND_TYPE_PIXTONE},
	{24, "PixTone/024.pxt", SOUND_TYPE_PIXTONE},
	{25, "PixTone/025.pxt", SOUND_TYPE_PIXTONE},
	{26, "PixTone/026.pxt", SOUND_TYPE_PIXTONE},
	{27, "PixTone/027.pxt", SOUND_TYPE_PIXTONE},
	{28, "PixTone/028.pxt", SOUND_TYPE_PIXTONE},
	{29, "PixTone/029.pxt", SOUND_TYPE_PIXTONE},
	{30, "PixTone/030.pxt", SOUND_TYPE_PIXTONE},
	{31, "PixTone/031.pxt", SOUND_TYPE_PIXTONE},
	{32, "PixTone/032.pxt", SOUND_TYPE_PIXTONE},
	{33, "PixTone/033.pxt", SOUND_TYPE_PIXTONE},
	{34, "PixTone/034.pxt", SOUND_TYPE_PIXTONE},
	{35, "PixTone/035.pxt", SOUND_TYPE_PIXTONE},
	{37, "PixTone/037.pxt", SOUND_TYPE_PIXTONE},
	{38, "PixTone/038.pxt", SOUND_TYPE_PIXTONE},
	{39, "PixTone/039.pxt", SOUND_TYPE_PIXTONE},
	{40, "PixTone/040.pxt", SOUND_TYPE_PIXTONE},
	{41, "PixTone/041.pxt", SOUND_TYPE_PIXTONE},
	{42, "PixTone/042.pxt", SOUND_TYPE_PIXTONE},
	{43, "PixTone/043.pxt", SOUND_TYPE_PIXTONE},
	{44, "PixTone/044.pxt", SOUND_TYPE_PIXTONE},
	{45, "PixTone/045.pxt", SOUND_TYPE_PIXTONE},
	{46, "PixTone/046.pxt", SOUND_TYPE_PIXTONE},
	{47, "PixTone/047.pxt", SOUND_TYPE_PIXTONE},
	{48, "PixTone/048.pxt", SOUND_TYPE_PIXTONE},
	{49, "PixTone/049.pxt", SOUND_TYPE_PIXTONE},
	{50, "PixTone/050.pxt", SOUND_TYPE_PIXTONE},
	{51, "PixTone/051.pxt", SOUND_TYPE_PIXTONE},
	{52, "PixTone/052.pxt", SOUND_TYPE_PIXTONE},
	{53, "PixTone/053.pxt", SOUND_TYPE_PIXTONE},
	{54, "PixTone/054.pxt", SOUND_TYPE_PIXTONE},
	{55, "PixTone/055.pxt", SOUND_TYPE_PIXTONE},
	{56, "PixTone/056.pxt", SOUND_TYPE_PIXTONE},
	{57, "PixTone/057.pxt", SOUND_TYPE_PIXTONE},
	{58, "PixTone/058.pxt", SOUND_TYPE_PIXTONE},
	{59, "PixTone/059.pxt", SOUND_TYPE_PIXTONE},
	{60, "PixTone/060.pxt", SOUND_TYPE_PIXTONE},
	{61, "PixTone/061.pxt", SOUND_TYPE_PIXTONE},
	{62, "PixTone/062.pxt", SOUND_TYPE_PIXTONE},
	{63, "PixTone/063.pxt", SOUND_TYPE_PIXTONE},
	{64, "PixTone/064.pxt", SOUND_TYPE_PIXTONE},
	{65, "PixTone/065.pxt", SOUND_TYPE_PIXTONE},
	{70, "PixTone/070.pxt", SOUND_TYPE_PIXTONE},
	{71, "PixTone/071.pxt", SOUND_TYPE_PIXTONE},
	{72, "PixTone/072.pxt", SOUND_TYPE_PIXTONE},
	{100, "PixTone/100.pxt", SOUND_TYPE_PIXTONE},
	{101, "PixTone/101.pxt", SOUND_TYPE_PIXTONE},
	{102, "PixTone/102.pxt", SOUND_TYPE_PIXTONE},
	{103, "PixTone/103.pxt", SOUND_TYPE_PIXTONE},
	{104, "PixTone/104.pxt", SOUND_TYPE_PIXTONE},
	{105, "PixTone/105.pxt", SOUND_TYPE_PIXTONE},
	{106, "PixTone/106.pxt", SOUND_TYPE_PIXTONE},
	{107, "PixTone/107.pxt", SOUND_TYPE_PIXTONE},
	{108, "PixTone/108.pxt", SOUND_TYPE_PIXTONE},
	{109, "PixTone/109.pxt", SOUND_TYPE_PIXTONE},
	{110, "PixTone/110.pxt", SOUND_TYPE_PIXTONE},
	{111, "PixTone/111.pxt", SOUND_TYPE_PIXTONE},
	{112, "PixTone/112.pxt", SOUND_TYPE_PIXTONE},
	{113, "PixTone/113.pxt", SOUND_TYPE_PIXTONE},
	{114, "PixTone/114.pxt", SOUND_TYPE_PIXTONE},
	{115, "PixTone/115.pxt", SOUND_TYPE_PIXTONE},
	{116, "PixTone/116.pxt", SOUND_TYPE_PIXTONE},
	{117, "PixTone/117.pxt", SOUND_TYPE_PIXTONE},
	{150, "PixTone/150.pxt", SOUND_TYPE_PIXTONE},
	{151, "PixTone/151.pxt", SOUND_TYPE_PIXTONE},
	{152, "PixTone/152.pxt", SOUND_TYPE_PIXTONE},
	{153, "PixTone/153.pxt", SOUND_TYPE_PIXTONE},
	{154, "PixTone/154.pxt", SOUND_TYPE_PIXTONE},
	{155, "PixTone/155.pxt", SOUND_TYPE_PIXTONE}
};

// Original decompiled from `PTone103.exe` - has since been modified
static BOOL LoadPixToneFile(const char *filename, PIXTONEPARAMETER *pixtone_parameters)
{
	BOOL success = FALSE;

	FILE *fp = fopen(filename, "r");

	if (fp != NULL)
	{
		fseek(fp, 0, SEEK_END);
		const size_t file_size = ftell(fp);
		rewind(fp);

		char *file_buffer = (char*)malloc(file_size);
		fread(file_buffer, 1, file_size, fp);

		fclose(fp);

		char *p = file_buffer;

		for (unsigned int i = 0; i < 4; ++i)
		{
			float freq;
			int increment;
			sscanf(p, "use  :%d\n%n", &pixtone_parameters[i].use, &increment);
			p += increment;
			sscanf(p, "size :%d\n%n", &pixtone_parameters[i].size, &increment);
			p += increment;
			sscanf(p, "main_model   :%d\n%n", &pixtone_parameters[i].oMain.model, &increment);
			p += increment;
			sscanf(p, "main_freq    :%f\n%n", &freq, &increment);
			p += increment;
			pixtone_parameters[i].oMain.num = freq;
			sscanf(p, "main_top     :%d\n%n", &pixtone_parameters[i].oMain.top, &increment);
			p += increment;
			sscanf(p, "main_offset  :%d\n%n", &pixtone_parameters[i].oMain.offset, &increment);
			p += increment;
			sscanf(p, "pitch_model  :%d\n%n", &pixtone_parameters[i].oPitch.model, &increment);
			p += increment;
			sscanf(p, "pitch_freq   :%f\n%n", &freq, &increment);
			p += increment;
			pixtone_parameters[i].oPitch.num = freq;
			sscanf(p, "pitch_top    :%d\n%n", &pixtone_parameters[i].oPitch.top, &increment);
			p += increment;
			sscanf(p, "pitch_offset :%d\n%n", &pixtone_parameters[i].oPitch.offset, &increment);
			p += increment;
			sscanf(p, "volume_model :%d\n%n", &pixtone_parameters[i].oVolume.model, &increment);
			p += increment;
			sscanf(p, "volume_freq  :%f\n%n", &freq, &increment);
			p += increment;
			pixtone_parameters[i].oVolume.num = freq;
			sscanf(p, "volume_top   :%d\n%n", &pixtone_parameters[i].oVolume.top, &increment);
			p += increment;
			sscanf(p, "volume_offset:%d\n%n", &pixtone_parameters[i].oVolume.offset, &increment);
			p += increment;
			sscanf(p, "initialY:%d\n%n", &pixtone_parameters[i].initial, &increment);
			p += increment;
			sscanf(p, "ax      :%d\n%n", &pixtone_parameters[i].pointAx, &increment);
			p += increment;
			sscanf(p, "ay      :%d\n%n", &pixtone_parameters[i].pointAy, &increment);
			p += increment;
			sscanf(p, "bx      :%d\n%n", &pixtone_parameters[i].pointBx, &increment);
			p += increment;
			sscanf(p, "by      :%d\n%n", &pixtone_parameters[i].pointBy, &increment);
			p += increment;
			sscanf(p, "cx      :%d\n%n", &pixtone_parameters[i].pointCx, &increment);
			p += increment;
			sscanf(p, "cy      :%d\n\n%n", &pixtone_parameters[i].pointCy, &increment);
			p += increment;
		}

		free(file_buffer);

		success = TRUE;
	}

	return success;
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


#include <dirent.h> 
#include <stdio.h> 

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}
#include <ctype.h>

void slice(const char *str, char *result, size_t start, size_t end)
{
    strncpy(result, str + start, end - start);
}

int main(int argc, char **argv) {

	printf("conversion utility to convert pxt to raw\n");
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	if (d) {
	  while ((dir = readdir(d)) != NULL) 
	  {
	    //printf("%s\n", dir->d_name);
		char name[256] = "\0";
		strcpy(name, dir->d_name);
		char ext[256] = "\0"; 
		strcpy(ext, get_filename_ext(name));
		//printf("%s\n", ext);

		if(!strcasecmp("pxt", ext))
		{		
			printf("%s ", dir->d_name);
			PIXTONEPARAMETER pixtone_parameters[4];		
			if (LoadPixToneFile(name, pixtone_parameters))
			{
				int ptp_num = 0;
				while (pixtone_parameters[ptp_num].use && ptp_num < 4)
					++ptp_num;		
				MakePixToneObject(pixtone_parameters, ptp_num, 0);


				char num[255] = "\0";
				slice(name, num, 2, 4);
				//printf("%s\n", num);
    			int x;
    			sscanf(num, "%x", &x);

				char path[255] = "\0";
    			sprintf(path, "%03u.raw", x);
				printf("%s\n", path);
				FILE* fp = fopen(path, "wb");
				fwrite(lpSECONDARYBUFFER[0]->data, lpSECONDARYBUFFER[0]->size, 1, fp);
				fclose(fp);
			}
		}

	  }
	  closedir(d);
	}
	return 0;
}