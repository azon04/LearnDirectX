#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertextBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_texture = nullptr;
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, WCHAR* textureFilename)
{
	// Initialize the vertex and index buffer that hold the geometry for the triangle
	bool result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);

	return result;
}

void ModelClass::Shutdown()
{
	// Release the model texture
	ReleaseTexture();

	// Release the vertex and index buffer
	ShutdownBuffers();
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put vertex and index buffers on the graphic pipeline to prepare them for drawing
	RenderBuffers(deviceContext);
}

int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_texture->GetTexture();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	// set the number of vertices in the vertex array
	m_vertexCount = 4;

	// Set the number of indices in the index array
	m_indexCount = 6;
	
	// Create vertex array
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data
	vertices[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f); // Bottom Left
#if USING_TEXTURE
	vertices[0].texture = D3DXVECTOR2(0.0f, 1.0f);
#else
	vertices[0].color = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
#endif
#if USING_LIGHT
	vertices[0].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
#endif

	vertices[1].position = D3DXVECTOR3(-1.0f, 1.0f, 0.0f); // Top Left
#if USING_TEXTURE
	vertices[1].texture = D3DXVECTOR2(0.0f, 0.0f);
#else
	vertices[1].color = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
#endif
#if USING_LIGHT
	vertices[1].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
#endif

	vertices[2].position = D3DXVECTOR3(1.0f, -1.0f, 0.0f); // Bottom Right
#if USING_TEXTURE
	vertices[2].texture = D3DXVECTOR2(1.0f, 1.0f);
#else
	vertices[2].color = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
#endif
#if USING_LIGHT
	vertices[2].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
#endif

	vertices[3].position = D3DXVECTOR3(1.0f, 1.0f, 0.0f); // Top Right
#if USING_TEXTURE
	vertices[3].texture = D3DXVECTOR2(1.0f, 0.0f);
#else
	vertices[3].color = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
#endif
#if USING_LIGHT
	vertices[3].normal = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
#endif

	// Load the index array with data
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	indices[3] = 2;
	indices[4] = 1;
	indices[5] = 3;

	// Set the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertextBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded
	delete[] vertices;
	delete[] indices;

	return true;
}

void ModelClass::ShutdownBuffers()
{
	if (m_vertextBuffer)
	{
		m_vertextBuffer->Release();
		m_vertextBuffer = nullptr;
	}

	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = nullptr;
	}
}

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered
	deviceContext->IASetVertexBuffers(0, 1, &m_vertextBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be renderer from this vertex buffer, in this case triangles
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	// Create the texture object
	m_texture = new TextureClass;
	if (!m_texture)
	{
		return false;
	}

	// Initialize the texture object
	result = m_texture->Initialize(device, filename);

	return result;
}

void ModelClass::ReleaseTexture()
{
	if (m_texture)
	{
		m_texture->Shutdown();
		delete m_texture;
		m_texture = nullptr;
	}
}
