#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_input = nullptr;
	m_graphics = nullptr;
}

SystemClass::SystemClass(const SystemClass&)
{

}

SystemClass::~SystemClass()
{

}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	screenWidth = screenHeight = 0;

	// Initialize the windows api
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object. This object will be used to handle reading the keyboard input from the user
	m_input = new InputClass;
	if (!m_input)
	{
		return false;
	}

	// Initialize the input object
	m_input->Initialize();

	// Create the graphics object. This object will handle rendering all the graphics for this application
	m_graphics = new GraphicsClass;
	if (!m_graphics)
	{
		return false;
	}

	// Initialize the graphic object
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);

	return result;
}

void SystemClass::Shutdown()
{
	// Release the graphic object
	if (m_graphics)
	{
		m_graphics->Shutdown();
		delete m_graphics;
		m_graphics = nullptr;
	}

	// Release the input object
	if (m_input)
	{
		delete m_input;
		m_input = nullptr;
	}

	// Shutdown the window
	ShutdownWindows();
}

void SystemClass::Run()
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	// Main Loop
	done = false;
	while (!done)
	{
		// Check to see if any messages are waiting in the queue
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// translate keystroke message into the right format
			TranslateMessage(&msg);

			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}

		// check to see if it's time to quit
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// otherwise do the frame processing
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort though and find that code to run for the message given
	switch (message)
	{
		// Check if a key has been pressed on the keyboard
		case WM_KEYDOWN:
			m_input->KeyDown((unsigned int)wParam);
			return 0;
		
		// Check if a key has been released on the keyboard
		case WM_KEYUP:
			m_input->KeyUp((unsigned int)wParam);
			return 0;
	}

	// Handle any message the switch statement didn't handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SystemClass::Frame()
{
	bool result;

	// Check if the user pressed escape and wants to exit the application
	if (m_input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object
	result = m_graphics->Frame();

	return result;
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	// Set an external pointer to this object
	ApplicationHandle = this;

	// Get the instance of this application
	m_hInstance = GetModuleHandle(NULL);

	// Given the application a name
	m_applicationName = L"Engine";

	// this struct holds information for the window class
	WNDCLASSEX wc;

	// Clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"WindowClass1";

	// register the window class
	RegisterClassEx(&wc);

	// Determine the resolution of the clients dekstop screen
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	int posX;
	int posY;

	if (FULLSCREEN)
	{
		// if full screen set the screen to maximum size of the users desktop and 32bit
		DEVMODE dmScreenSettings;
		ZeroMemory(&dmScreenSettings, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPanningHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner
		posX = posY = 0;
	}
	else
	{
		// Set default width and height
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	RECT wr = { 0,0, screenWidth, screenHeight }; // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	// create the window and use the result as the handle
	m_hwnd = CreateWindowEx(NULL,
		L"WindowClass1", // name of the window class
		m_applicationName, // title of the window
		WS_OVERLAPPEDWINDOW, // Window style
		posX, // x-position of the window
		posY, // y-position of the window
		wr.right - wr.left, // width of the window
		wr.bottom - wr.top, // height of the window
		NULL, // we have no parent window, NULL
		NULL, // we aren't using menus, NULL
		m_hInstance, // applicattion handle
		NULL); // used with mutiple window, NULL

	// displace the window on the screen
	ShowWindow(m_hwnd, SW_SHOW);
}

void SystemClass::ShutdownWindows()
{
	// Fix the display settings if leaving full screen mode
	if (FULLSCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove Windows
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance
	UnregisterClass(L"WindowClass1", m_hInstance);
	m_hInstance = NULL;

	// Release the pointer to this class
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// sort though and find that code to run for the message given
	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
			// close the application entirely
			PostQuitMessage(0);
			return 0;

		case WM_CLOSE:
			// close the application entirely
			PostQuitMessage(0);
			return 0;
	}

	return ApplicationHandle->MessageHandler(hWnd, message, wParam, lParam);
}
