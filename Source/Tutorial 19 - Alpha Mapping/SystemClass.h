#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <windows.h>

#include "InputClass.h"
#include "GraphicsClass.h"
#include "SoundClass.h"
#include "FPSClass.h"
#include "CPUClass.h"
#include "TimerClass.h"
#include "PositionClass.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hInstance;
	HWND m_hwnd;

	InputClass* m_input;
	GraphicsClass* m_graphics;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif
