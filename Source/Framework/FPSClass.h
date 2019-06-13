#ifndef _FPSCLASS_H_
#define _FPSCLASS_H_

// Linking
#pragma comment(lib, "winmm.lib")

// Includes
#include <windows.h>
#include <mmsystem.h>

class FPSClass
{
public:
	FPSClass();
	~FPSClass();

	void Initialize();
	void Frame();
	int GetFPS();

private:
	int m_fps, m_count;
	unsigned long m_startTime;
};

#endif
