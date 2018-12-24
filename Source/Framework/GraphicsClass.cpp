#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
#if USING_TEXTURE
	m_TextureShader = nullptr;
#else
	m_ColorShader = nullptr;
#endif
}

GraphicsClass::~GraphicsClass()
{

}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hWnd)
{
	bool result = false;

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

	// Set the initial position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create  the model Object
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object
	result = m_Model->Initialize(m_D3D->GetDevice(), L"./Data/test.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

#if USING_TEXTURE
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
		MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
#else
	// Create the color shader object
	m_ColorShader = new ColorShaderClass;
	if (!m_ColorShader)
	{
		return false;
	}

	// Initializre the color shader object
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}
#endif

	return true;
}

void GraphicsClass::Shutdown()
{
#if USING_TEXTURE
	// Release the texture shader object
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}
#else
	// Release the color shader object
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = nullptr;
	}
#endif

	// Release the model object
	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = nullptr;
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
	// Render the graphics scene
	bool result = Render();
	return result;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	bool result;

	// Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffer on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

#if USING_TEXTURE
	// Render the model using the texture shader
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}
#else
	// Render the model using the color shader
	result = m_ColorShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
#endif

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}
