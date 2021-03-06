#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_TextureShader = nullptr;
#if USING_LIGHT
	m_LightShader = nullptr;
	m_light = nullptr;
#else
	m_ColorShader = nullptr;
#endif
	m_Bitmap = nullptr;
	m_Text = nullptr;
	m_ModelList = nullptr;
	m_Frustum = nullptr;
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
	result = m_Model->Initialize(m_D3D->GetDevice(), "./Data/Sphere.txt", L"./Data/seafloor.dds");
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
	m_light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetDirection(0.0f, 0.0f, 1.0f);
	m_light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_light->SetSpecularPower(32.0f);
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

	// Create the bitmap object
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}

	// Initialize the bitmap object
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./Data/Arrow.dds", 32, 32);
	if (!result)
	{
		return false;
	}

	// Create the text object
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hWnd, screenWidth, screenHeight, baseViewMatrix);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the text object.", L"Error", MB_OK);
		return false;
	}

	// Create the model list object
	m_ModelList = new ModelListClass;
	if (!m_ModelList)
	{
		return false;
	}

	// Initialize the model list object
	result = m_ModelList->Initialize(25);
	if (!result)
	{
		MessageBox(hWnd, L"Could not initialize the model list object", L"Error", MB_OK);
		return false;
	}

	// Create Frustum object
	m_Frustum = new FrustumClass;
	if (!m_Frustum)
	{
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	// Release the frustum object
	if (m_Frustum)
	{
		delete m_Frustum;
		m_Frustum = nullptr;
	}

	// Release the model list object
	if (m_ModelList)
	{
		m_ModelList->Shutdown();
		delete m_ModelList;
		m_ModelList = nullptr;
	}

	// Release the bitmap object
	if (m_Text)
	{
		m_Text->Shutdown();
		delete m_Text;
		m_Text = nullptr;
	}

	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = nullptr;
	}

	// Release the texture shader object
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = nullptr;
	}

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

bool GraphicsClass::Frame(int fps, int cpu, float frameTime, float rotationY)
{
	static float rotation = 0.0f;
	
	// Set the position of the camera
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Set the rotation of the camera
	m_Camera->SetRotation(0.0f, rotationY, 0.0f);

	// Update the rotation variable each frame
	rotation += (float)D3DX_PI / 2 * frameTime / 1000;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	bool result;
	
	// Set the location of the mouse
	result = m_Text->SetFPS(fps, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	result = m_Text->SetCPU(cpu, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	return result;
}

bool GraphicsClass::Render(float delta, int mouseX, int mouseY)
{
	D3DXMATRIX viewMatrix, projectionMatrix, worldMatrix, orthoMatrix;
	bool result;
	int modelCount;
	int renderCount;
	float positionX, positionY, positionZ;
	D3DXVECTOR4 color;
	float radius;
	bool renderModel;

	// Clear the buffers to begin the scene
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position
	m_Camera->Render();

	// Get the world, view, projection matrices from the camera and d3d object
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Construct the frustum
	m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);

	// Get the number of models that will be rendered
	modelCount = m_ModelList->GetModelCount();

	// Initialize the count of models that have been rendered
	renderCount = 0;

	// Go through all the models and render them  only if they can be seen by the camera view
	for (int i = 0; i < modelCount; i++)
	{
		// Get the position and color of the sphere model at this index
		m_ModelList->GetData(i, positionX, positionY, positionZ, color);

		// Set the radius of the sphere to 1.0 since this is already known
		radius = 1.0f;

		// Check if the sphere model is in the view frustum
		renderModel = m_Frustum->CheckSphere(positionX, positionY, positionZ, radius);

		// If it can be seen then render it, if not skip this model and check the next phase
		if (renderModel)
		{
			// Move the model to the location it should be rendered at
			D3DXMatrixTranslation(&worldMatrix, positionX, positionY, positionZ);

			// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing
			m_Model->Render(m_D3D->GetDeviceContext());
#if USING_LIGHT
			// Render the model using the light shader
			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
				m_light->GetDirection(), color, color,
				m_Camera->GetPosition(), m_light->GetSpecularColor(), m_light->GetSpecularPower());
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

			// Reset the original world matrix
			m_D3D->GetWorldMatrix(worldMatrix);

			// Since this model was rendered then increase the count for this frame
			renderCount++;
		}
	}

	// Set the number of models that was actually rendered this frame
	result = m_Text->SetRenderCount(renderCount, m_D3D->GetDeviceContext());
	if (!result)
	{
		return false;
	}

	// Turn off the Z buffer to begin all 2D rendering
	m_D3D->TurnZBufferOff();
	
	// Turn on the alpha blending before rendering the text
	m_D3D->TurnOnAlphaBlending();

	// reset world Matrix
	m_D3D->GetWorldMatrix(worldMatrix);

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing
// 	result = m_Bitmap->Render(m_D3D->GetDeviceContext(), mouseX, mouseY);
// 	if (!result)
// 	{
// 		return false;
// 	}

	// Render the bitmap with the texture shader
// 	result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, m_Bitmap->GetTexture());
// 	if (!result)
// 	{
// 		return false;
// 	}

	// Render text
	// Render the text strings
	result = m_Text->Render(m_D3D->GetDeviceContext(), worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Turn off the alpha blending after rendering the text
	m_D3D->TurnOffAlphaBlending();

	// Turn the Z buffer back on now that all 2D rendering has completed
	m_D3D->TurnZBufferOn();

	// Present the rendered scene to the screen
	m_D3D->EndScene();

	return true;
}
