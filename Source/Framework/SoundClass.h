#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

// Linking
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

// Includes
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

class SoundClass
{
private:

	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytePerSeconds;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

public:
	SoundClass();
	~SoundClass();

	bool Initialize(HWND hwnd);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND hwnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* filename, IDirectSoundBuffer8** buffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** buffer);
	
	bool PlayWaveFile();

private:
	IDirectSound8 * m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;
	IDirectSoundBuffer8* m_secondaryBuffer1;
	IDirectSoundBuffer8* m_secondaryBuffer2;
};

#endif // !_SOUNDCLASS_H__
