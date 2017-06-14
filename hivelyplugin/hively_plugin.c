#include "hvl_replay.h"

#include <stdio.h>

void init()
{
	hvl_InitReplayer();
}

void* load(const char *fileName)
{
	return (void*)hvl_LoadTune(fileName, 44100, 0);

}

void unload(void *song)
{
	hvl_FreeTune((struct hvl_tune*)song);
}

int getSamples(void* song, short* target, int targetSize)
{

	int frameSize = ((44100*2)/50); // 2 channels, shorts -- 1764 bytes == 882 stereo samples == 441 frames

	printf("TARGET %p %p %d\n", song, target, targetSize);
	fflush(stdout);
	signed char *ptr = (signed char*)target;
	signed char *targetEnd = ptr + targetSize * 2;
	while(ptr + frameSize*2 <= targetEnd) {
		printf("Decoding to %d bytes to %p\n", frameSize*2, ptr);
		fflush(stdout);
		hvl_DecodeFrame((struct hvl_tune*)song, ptr, ptr+2, 4);
		ptr += frameSize*2;
	}
	return (short*)ptr - target;
}
