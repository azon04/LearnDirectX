#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_Light = nullptr;
	m_SpecMapShader = nullptr; 
	m_RenderTexture = nullptr;
	m_DebugWindow = nullptr;
	m_TextureShader = nullptr;
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
	m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}

	// Initialize the model object
	result = m_Model->Initialize(m_D3D->GetDevice(), "./Data/Cube.txt", L"./Data/stone02.dds", L"./Data/bump02.dds", L"./Data/spec02.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create the texture shader object
	m_SpecMapShader = new SpecMapShaderClass;
	if (!m_SpecMapShader)
	{
		return false;
	}

	// Initialize the texture shader object
	result = m_SpecMapShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the multi texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(16.0f);


	// Create the render to texture object
	m_RenderTexture = new RenderTextureClass;
	if (!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create the debug window object
	m_DebugWindow = new DebugWindowClass;
	if (!m_DebugWindow)
	{
		return false;
	}

	// Initialize the debug window object
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, 100 * screenWidth / screenHeight, 100);
	if (!result)
	{
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
		MessageBox(hWnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the texture shader object
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

	// Release the debug window object
	if (m_DebugWindow)
	{
		m_DebugWindow->Shutdown();
		delete m_DebugWindow;
		m_DebugWindow = nullptr;
	}

	// Release the render to texture object
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = nullptr;
	}

	if (m_Light)
	{
		delete m_Light;
		m_Light = nullptr;
	}

	// Release the texture shader object
	if (m_SpecMapShader)
	{
		m_SpecMapShader->Shutdown();
		delete m_SpecMapShader;
		m_SpecMapShader = nullptr;
	}

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
	// Set the position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);
	
	return true;
}

bool GraphicsClass::Render()
{
	D3DXMATRIX viewMatrix, worldMatrix, orthoMatrix;
	bool result;

	// Render the entire scene to the texture first
	result = RenderToTexture();
	if (!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Render the scene as normal to the back buffer
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering
	m_D3D->TurnZBufferOff();

	// Get the world, view and ortho matrices from the camera and d3d objects
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Put the debuf window vertex and index buffers on the graphics pipeline to prepare them for drawing
	result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 50, 50);
	if (!result)
	{
		return false;
	}

	// Render the debug window using the texture shader
	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_RenderTexture->GetShaderResourceView());
	if (!result)
	{
		return false;
	}

	// Turn the Z buffer back on now that all 2D rendering has completed
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}

bool GraphicsClass::RenderToTexture()
{
	bool result;

	// Set the render target to be render to texture
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView());

	// Clear the render texture
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), m_D3D->GetDepthStencilView(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Render the scene now and it will draw to the render to texture instead of the back buffer
	result = RenderScene();
	if (!result)
	{
		return false;
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore
	m_D3D->SetBackBufferRenderTarget();

	return true;
}

bool GraphicsClass::RenderScene()
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix;
	static float rotation = 0.0f;
	bool result;

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Update the rotation variable each frame
	rotation += (float)D3DX_PI * 0.0025f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	D3DXMatrixRotationY(&worldMatrix, rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

	// Render the mode using the bump map shader
	result = m_SpecMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTextureArray(), m_Light->GetDirection(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	return true;
}
