

//tcc snd.c -o snd.exe -lWinmm




#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include "def.c"

struct SndBuff
{
	HWAVEOUT device; /* device handle */
	MMRESULT result;/* for waveOut return values */
	WAVEHDR header;
	bool isOpen;
};

bool sndBuffInit(struct SndBuff * at, int sample, int bits, int channels)
{
	ZeroMemory(&(at->header), sizeof(WAVEHDR));
	WAVEFORMATEX format; /* look this up in your documentation */
	format.nSamplesPerSec = sample; /* sample rate */
	format.wBitsPerSample = bits; /* sample size */
	format.nChannels = channels; /* channels*/
	
	format.cbSize = 0; /* size of _extra_ info */
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nBlockAlign = (format.wBitsPerSample >> 3) * format.nChannels;
	format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;
	
	if(waveOutOpen(&(*at).device, WAVE_MAPPER, &format, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR) 
	{
		at->isOpen = false;
		logStr("unable to open WAVE_MAPPER device\n");
		return false;
	}
	at->isOpen = true;
	return true;
}

void sndBuffAdd(struct SndBuff * at, short *  block, int size)
{
	
	/*
	 * initialise the block header with the size
	 * and pointer.
	 */
	
	at->header.dwBufferLength = size;
	at->header.lpData = (void*)block;
	/*
	 * prepare the block for playback
	 */
	waveOutPrepareHeader(at->device, &(at->header), sizeof(WAVEHDR));
	/*
	 * write the block to the device. waveOutWrite returns immediately
	 * unless a synchronous driver is used (not often).
	 */
	waveOutWrite(at->device, &(at->header), sizeof(WAVEHDR));
	/*
	 * wait a while for the block to play then start trying
	 * to unprepare the header. this will fail until the block has
	 * played.
	 */
	Sleep(100);
	while(waveOutUnprepareHeader( at->device, &(at->header), sizeof(WAVEHDR)) == WAVERR_STILLPLAYING) Sleep(100);
}

void sndBuffFree(struct SndBuff * at)
{
	waveOutClose(at->device);
}
