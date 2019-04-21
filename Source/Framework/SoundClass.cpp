#include "SoundClass.h"

SoundClass::SoundClass()
{
	m_DirectSound = nullptr;
	m_primaryBuffer = nullptr;
	m_secondaryBuffer1 = nullptr;
}

SoundClass::~SoundClass()
{

}

bool SoundClass::Initialize(HWND hwnd)
{
	bool result;

	// Initialize direct sound and the primary sound buffer
	result = InitializeDirectSound(hwnd);
	if (!result)
	{
		return false;
	}

	// Load Wave audio file into a secondary buffer
	result = LoadWaveFile("./Data/sound01.wav", &m_secondaryBuffer1);
	if (!result)
	{
		return false;
	}

	result = LoadWaveFile("./Data/sound02.wav", &m_secondaryBuffer2);
	if (!result)
	{
		return false;
	}

	// Play the wave file now that is has been loaded
	result = PlayWaveFile();
	if (!result)
	{
		return false;
	}

	return true;
}

void SoundClass::Shutdown()
{
	// Release the secondary buffer
	ShutdownWaveFile(&m_secondaryBuffer1);
	ShutdownWaveFile(&m_secondaryBuffer2);

	// Shutdown the Direct Sound API
	ShutdownDirectSound();
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// Initialize the direct sound interface pointer for the default sound device
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer description
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound buffer
	// In this case it is a .wav file recorded at 44100 samples per second in 16-bit stereo (cd audio format)
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to be the wave format specified
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	// Release the primary sound buffer pointer
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = nullptr;
	}

	// Release the direct sound interface pointer
	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = nullptr;
	}
}

bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	// Open the wave file in binary
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	// Check the chunk ID is the RIFF format
	if (waveFileHeader.chunkId[0] != 'R' || waveFileHeader.chunkId[1] != 'I' ||
		waveFileHeader.chunkId[2] != 'F' || waveFileHeader.chunkId[3] != 'F')
	{
		return false;
	}

	// Check that the file format is WAVE format
	if (waveFileHeader.format[0] != 'W' || waveFileHeader.format[1] != 'A' ||
		waveFileHeader.format[2] != 'V' || waveFileHeader.format[3] != 'E')
	{
		return false;
	}

	// Check that the sub chunk Id is the fmt format
	if (waveFileHeader.subChunkId[0] != 'f' || waveFileHeader.subChunkId[1] != 'm' ||
		waveFileHeader.subChunkId[2] != 't' || waveFileHeader.subChunkId[3] != ' ')
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// Check that the wave file was recorded in stereo format
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// Ensire the wave file was recorded in 16 bit format
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header
	if (waveFileHeader.dataChunkId[0] != 'd' || waveFileHeader.dataChunkId[1] != 'a' ||
		waveFileHeader.dataChunkId[2] != 't' || waveFileHeader.dataChunkId[3] != 'a')
	{
		return false;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded into
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Text the buffer format againts the direct sound 8 interface and create the secondary buffer
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the temp buffer
	tempBuffer->Release();
	tempBuffer = nullptr;

	// Move to the beginning of wave data which starts at the end of the data chunk header
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write wave data into it
	result = (*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy wave data into the buffer
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it
	result = (*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into secondary buffer
	delete[] waveData;
	waveData = 0;

	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if (*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = nullptr;
	}
}

bool SoundClass::PlayWaveFile()
{
	HRESULT result;

	// Set position at the beginning of the sound buffer
	result = m_secondaryBuffer1->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%
	result = m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary buffer
	result = m_secondaryBuffer1->Play(0, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Set position at the beginning of the sound buffer
	result = m_secondaryBuffer2->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	// Set volume of the buffer to 100%
	result = m_secondaryBuffer2->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary buffer
	result = m_secondaryBuffer2->Play(0, 0, DSBPLAY_LOOPING);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}
