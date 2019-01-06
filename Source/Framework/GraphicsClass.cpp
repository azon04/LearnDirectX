#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
#if USING_LIGHT
	m_LightShader = nullptr;
	m_light = nullptr;
#elif USING_TEXTURE
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
	result = m_Model->Initialize(m_D3D->GetDevice(), "./Data/Cube.txt", L"./Data/seafloor.dds");
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

#if USING_LIGHT
	// Create the light shader object
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hWnd);
	if (!result)
	{
		return false;
	}

	// Create the light object
	m_light = new LightClass;
	if (!m_light)
	{
		return false;
	}

	// Initialize the light object
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);

#elif USING_TEXTURE
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
#if USING_LIGHT
	// Release the light object
	if (m_light)
	{
		delete m_light;
		m_light = nullptr;
	}

	// Release the light shader object
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = nullptr;
	}

#elif USING_TEXTURE
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
	static float rotation = 0.0f;
	
	// Update the rotation variable each frame
	rotation += (float)D3DX_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Render the graphics scene
	bool result = Render(rotation);
	return result;
}

bool GraphicsClass::Render(float delta)
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

	// Rotate the world matrix by the rotation value so that the triangle will spin
	D3DXMatrixRotationY(&worldMatrix, delta);

	// Put the model vertex and index buffer on the graphics pipeline to prepare them for drawing
	m_Model->Render(m_D3D->GetDeviceContext());

#if USING_LIGHT
	// Render the model using the light shader
	result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(), m_light->GetDirection(), m_light->GetDiffuseColor());
	if (!result)
	{
		return false;
	}
#elif USING_TEXTURE
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
