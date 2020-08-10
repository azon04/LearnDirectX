#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model1 = nullptr;
	m_Model2 = nullptr;
	m_TextureShader = nullptr;
	m_TransparentShader = nullptr;
}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result = false;
	D3DXMATRIX baseViewMatrix;

	// Create Direct3D object
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hWnd, FULLSCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Initialize a base view matrix with the camera for 2D user interface rendering
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->Render();
	m_Camera->GetViewMatrix(baseViewMatrix);

	// Set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create  the model Object
	m_Model1 = new ModelClass;
	if (!m_Model1)
	{
		return false;
	}

	// Initialize the model object
	result = m_Model1->Initialize(m_D3D->GetDevice(), "./Data/Square.txt", L"./Data/dirt01.dds", L"./Data/dirt01.dds", L"./Data/dirt01.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_Model2 = new ModelClass;
	if (!m_Model2)
	{
		return false;
	}

	// Initialize the model object
	result = m_Model2->Initialize(m_D3D->GetDevice(), "./Data/Square.txt", L"./Data/stone01.dds", L"./Data/stone01.dds", L"./Data/stone01.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the Clip Plane shader object.", L"Error", MB_OK);
		return false;
	}

	m_TransparentShader = new TransparentShaderClass;
	if (!m_TransparentShader)
	{
		return false;
	}

	// Initialize the texture shader object
	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the Clip Plane shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the texture shader object
	if (m_TransparentShader)
	{
		m_TransparentShader->Shutdown();
		delete m_TransparentShader;
		m_TransparentShader = nullptr;
	}

	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	// Release the model object
	if (m_Model1)
	{
		m_Model1->Shutdown();
		delete m_Model1;
		m_Model1 = nullptr;
	}

	if (m_Model2)
	{
		m_Model2->Shutdown();
		delete m_Model2;
		m_Model2 = nullptr;
	}

	// Release the camera object
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	// Release the Direct3D object
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = nullptr;
	}
}

bool GraphicsClass::Frame()
{
	// Set the position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;
	float blendAmount = 0.75f;

	// Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model1->Render(m_D3D->GetDeviceContext());

	// Render the mode using the texture shader
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model1->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model1->GetTextureArray()[0] );

	if (!result)
	{
		return false;
	}

	// Translate to the right by one unit and towards the camera by one unit
	D3DXMatrixTranslation(&worldMatrix, 1.0f, 0.0f, -1.0f);

	// Turn on alpha blending for the transparency to work
	m_D3D->TurnOnAlphaBlending();

	// put the second model on the graphics pipeline
	m_Model2->Render(m_D3D->GetDeviceContext());

	// Render the second square with the transparent shader
	result = m_TransparentShader->Render(m_D3D->GetDeviceContext(), m_Model2->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model2->GetTextureArray()[0], blendAmount);

	if (!result)
	{
		return false;
	}

	// Turn off alpha blending
	m_D3D->TurnOffAlphaBlending();

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}
