#include "SystemClass.h"

SystemClass::SystemClass()
{
	m_input = nullptr;
	m_graphics = nullptr;
	m_Sound = nullptr;
	m_FPS = nullptr;
	m_CPU = nullptr;
	m_Timer = 0;
	m_Position = nullptr;
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
	result = m_input->Initialize(m_hInstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object. This object will handle rendering all the graphics for this application
	m_graphics = new GraphicsClass;
	if (!m_graphics)
	{
		return false;
	}

	// Initialize the graphic object
	result = m_graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	// Create the sound object
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object
	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound", L"Error", MB_OK);
		return false;
	}

	// Create FPS object
	m_FPS = new FPSClass;
	if (!m_FPS)
	{
		return false;
	}

	m_FPS->Initialize();

	// Create CPU Object
	m_CPU = new CPUClass;
	if (!m_CPU)
	{
		return false;
	}

	m_CPU->Initialize();

	// Create Timer object
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer Object.", L"Error", MB_OK);
		return false;
	}

	// Create the position object
	m_Position = new PositionClass;
	if (!m_Position)
	{
		return false;
	}

	return result;
}

void SystemClass::Shutdown()
{
	// Release the position object
	if (m_Position)
	{
		delete m_Position;
		m_Position = nullptr;
	}

	// Release the timer object
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	// Release the cpu object
	if (m_CPU)
	{
		m_CPU->Shutdown();
		delete m_CPU;
		m_CPU = nullptr;
	}

	// Release the FPS object
	if (m_FPS)
	{
		delete m_FPS;
		m_FPS = nullptr;
	}

	// Release the sound object
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = false;
	}
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
		m_input->Shutdown();
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

		// Check if the user pressed escape and wants to quit
		if (m_input->IsEscapePressed())
		{
			done = true;
		}
	}
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Handle any message the switch statement didn't handle
	return DefWindowProc(hWnd, message, wParam, lParam);
}

bool SystemClass::Frame()
{
	bool result;
	int mouseX, mouseY;
	bool keyDown;
	float rotationY;

	// Update the system stats
	m_Timer->Frame();
	m_FPS->Frame();
	m_CPU->Frame();

	// Do the input frame processing
	result = m_input->Frame();
	if (!result)
	{
		return false;
	}

	// Set the frame time for calculating the updated position
	m_Position->SetFrameTime(m_Timer->GetTime());

	// Check if the left and right arrow key has been pressed, if so rotate the camera accordingly
	keyDown = m_input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	keyDown = m_input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	// Get the location of the mouse from the input object
	m_input->GetMouseLocation(mouseX, mouseY);

	// Get the current view point rotation
	m_Position->GetRotation(rotationY);

	// Do the frame processing for the graphics object
	result = m_graphics->Frame(m_FPS->GetFPS(), m_CPU->GetCPUPercentage() , m_Timer->GetTime(), rotationY);
	if (!result)
	{
		return false;
	}
	
	// Render the graphics to the screen
	result = m_graphics->Render(m_Timer->GetTime(), mouseX, mouseY);
	if (!result)
	{
		return false;
	}

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
