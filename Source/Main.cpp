#include "SystemClass.h"

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	SystemClass* System;
	
	// Create the system object
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system
	bool result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object
	System->Shutdown();
	delete System;
	System = nullptr;

	return 0;
}