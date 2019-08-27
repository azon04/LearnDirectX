#include "ModelClass.h"

ModelClass::ModelClass()
{
	m_vertextBuffer = nullptr;
	m_indexBuffer = nullptr;
	m_textureArray = nullptr;
	m_model = nullptr;
}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFileName, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	// Load in the model data
	bool result = LoadModel(modelFileName);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffer that hold the geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTextures(device, textureFilename1, textureFilename2);

	return result;
}

void ModelClass::Shutdown()
{
	// Release the model texture
	ReleaseTextures();

	// Release the vertex and index buffer
	ShutdownBuffers();

	// Release the model data
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

ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return m_textureArray->GetTextureArray();
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
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
	for (int i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = D3DXVECTOR2(m_model[i].tu, m_model[i].tv);
		indices[i] = i;
	}

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

bool ModelClass::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	bool result;

	// Create the texture object
	m_textureArray = new TextureArrayClass;
	if (!m_textureArray)
	{
		return false;
	}

	// Initialize the texture object
	result = m_textureArray->Initialize(device, filename1, filename2);

	return result;
}

void ModelClass::ReleaseTextures()
{
	if (m_textureArray)
	{
		m_textureArray->Shutdown();
		delete m_textureArray;
		m_textureArray = nullptr;
	}
}

bool ModelClass::LoadModel(char* modelFileName)
{
	ifstream fin;
	char input;
	int i;

	// open model file
	fin.open(modelFileName);

	// if it couldnot open the file then exit
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read the vertex count
	fin >> m_vertexCount;

	// Set the number or indices to be the same as the vertex count
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in
	m_model = new ModelType[m_vertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data
	for (i = 0; i < m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file
	fin.close();

	return true;

}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}
