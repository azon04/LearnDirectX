#include "TextClass.h"

TextClass::TextClass()
{
	m_FontShader = nullptr;
	m_Font = nullptr;

	m_sentence1 = nullptr;
	m_sentence2 = nullptr;
}

TextClass::~TextClass()
{

}

bool TextClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, D3DXMATRIX baseViewMatrix)
{
	bool result;

	// Store screen width and height
	m_screenHeight = screenHeight;
	m_screenWidth = screenWidth;

	// Store the base view matrix
	m_baseViewMatrix = baseViewMatrix;

	// Create Font Object
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object
	result = m_Font->Initialize(device, "./Data/FontTextureDesc.txt", L"./Data/FontTexture.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the first sentence
	result = InitializeSentence(&m_sentence1, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence1, "Hello", 100, 100, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the second sentence
	result = InitializeSentence(&m_sentence2, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence2, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Initialize the third sentence
	result = InitializeSentence(&m_sentence3, 16, device);
	if (!result)
	{
		return false;
	}

	// Now update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence3, "Goodbye", 100, 200, 1.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

void TextClass::Shutdown()
{
	// Release the first sentence
	ReleaseSentence(&m_sentence1);

	// Release the second sentence
	ReleaseSentence(&m_sentence2);

	// Release the font shader object
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = nullptr;
	}

	// Release the font object
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = nullptr;
	}
}

bool TextClass::Render(ID3D11DeviceContext* deviceContext, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	bool result;

	// Draw the first sentence
	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the second sentence
	result = RenderSentence(deviceContext, m_sentence2, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	// Draw the third sentence
	result = RenderSentence(deviceContext, m_sentence3, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetMousePosition(int mouseX, int mouseY, ID3D11DeviceContext* deviceContext)
{
	char tempStr[16];
	char mouseString[16];
	bool result;

	// Convert mouseX integer to string format
	_itoa_s(mouseX, tempStr, 10);

	// Setup the mouseX string
	strcpy_s(mouseString, "Mouse X:");
	strcat_s(mouseString, tempStr);

	// Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence1, mouseString, 20, 20, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	// Convert mouseY integer to string format
	_itoa_s(mouseY, tempStr, 10);

	// Setup the mouseY string
	strcpy_s(mouseString, "Mouse Y:");
	strcat_s(mouseString, tempStr);

	// Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence2, mouseString, 20, 40, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetPrintBuffer(InputClass* input, ID3D11DeviceContext* deviceContext)
{
	char temp[16];
	int charIndex = 2;
	temp[0] = 'k';
	temp[1] = ':';
	for (int i = 0; i < 256; i++)
	{
		if (input->IsKeyPressed(i))
		{
			temp[charIndex++] = MapVirtualKey(i, MAPVK_VSC_TO_VK);
		}
	}

	temp[charIndex] = '\0';
	bool result = UpdateSentence(m_sentence3, temp, 20, 60, 1.0f, 1.0f, 1.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetFPS(int fps, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char fpsString[16];
	float red, green, blue;
	bool result;

	// Truncate the fps tp below 10000
	if (fps >= 10000)
	{
		fps = 9999;
	}

	// Convert the fps integer to string format
	_itoa_s(fps, tempString, 10);

	// Setup the fps string
	strcpy_s(fpsString, "FPS: ");
	strcat_s(fpsString, tempString);

	// if fps is 160 or above, set the fps color to green
	if (fps >= 60)
	{
		red = 0.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// if the fps below 60 set the fps color to yellow
	if (fps < 60)
	{
		red = 1.0f;
		green = 1.0f;
		blue = 0.0f;
	}

	// if fps is below 30 set the fps color to red
	if (fps < 30)
	{
		red = 1.0f;
		green = 0.0f;
		blue = 0.0f;
	}

	// Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence1, fpsString, 20, 20, red, green, blue, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetCPU(int cpu, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char cpuString[16];
	bool result;

	// Convert the cpu integer to string format
	_itoa_s(cpu, tempString, 10);

	// Setup the cpu string
	strcpy_s(cpuString, "CPU: ");
	strcat_s(cpuString, tempString);
	strcat_s(cpuString, "%");

	// Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence2, cpuString, 20, 40, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::SetRenderCount(int renderCount, ID3D11DeviceContext* deviceContext)
{
	char tempString[16];
	char countString[16];
	bool result;

	// Convert the cpu integer to string format
	_itoa_s(renderCount, tempString, 10);

	// Setup the cpu string
	strcpy_s(countString, "Count: ");
	strcat_s(countString, tempString);

	// Update the sentence vertex buffer with the new string information
	result = UpdateSentence(m_sentence3, countString, 20, 60, 0.0f, 1.0f, 0.0f, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}

bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create new sentence object
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffer to null
	(*sentence)->vertexBuffer = nullptr;
	(*sentence)->indexBuffer = nullptr;

	// Set the maximum length of the sentence
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array
	(*sentence)->vertexCount = maxLength * 6;

	// set the number of indexes in the index array
	(*sentence)->indexCount = maxLength * 6;

	// Create the vertex array
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize vertex array to zeroes at first
	memset(vertices, 0, sizeof(VertexType) * (*sentence)->vertexCount);

	// Initialize index array
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}


	// Set up the description of the dynamic vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;
	
	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release vertex array as it is no longer needed
	delete[] vertices;

	// Release index array as it is no longer needed
	delete[] indices;

	return true;
}

bool TextClass::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue, ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store the color of the sentence
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create vertex array
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeroes at first
	memset(vertices, 0, sizeof(VertexType) * sentence->vertexCount);

	// Calculate the starting location to draw the sentence on the screen to
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// use the font class to build the vertex array from the sentence text and sentence draw location
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex texture buffer so it can be written to
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into vertex buffer
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed
	delete[] vertices;

	return true;
}

void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		// Release the sentence vertex buffer
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = nullptr;
		}

		// Release the sentence index buffer
		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = nullptr;
		}

		// Release the sentence
		delete *sentence;
		*sentence = nullptr;
	}
}

bool TextClass::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, D3DXMATRIX worldMatrix, D3DXMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	D3DXVECTOR4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case, triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color
	pixelColor = D3DXVECTOR4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader
	result = m_FontShader->Render(deviceContext, sentence->indexCount, worldMatrix, m_baseViewMatrix, orthoMatrix, m_Font->GetTexture(), pixelColor);
	if (!result)
	{
		return false;
	}

	return true;
}

