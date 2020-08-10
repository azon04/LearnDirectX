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

bool ModelClass::Initialize(ID3D11Device* device, char* modelFileName, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	// Load in the model data
	bool result = LoadModel(modelFileName);
	if (!result)
	{
		return false;
	}

	// Calculate the normal, tangent and binormal
	CalculateModelVectors();

	// Initialize the vertex and index buffer that hold the geometry for the triangle
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTextures(device, textureFilename1, textureFilename2, textureFilename3);

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
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);
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

bool ModelClass::LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	bool result;

	// Create the texture object
	m_textureArray = new TextureArrayClass;
	if (!m_textureArray)
	{
		return false;
	}

	// Initialize the texture object
	result = m_textureArray->Initialize(device, filename1, filename2, filename3);

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

void ModelClass::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;

	// Calculate the number of faces in this model
	faceCount = m_vertexCount / 3;

	// initialize the index to the model data
	index = 0;

	// Go through all the faces and calculate the tangent, binormal and normal vectors
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model
		vertex1.x = m_model[index].x;
		vertex1.y = m_model[index].y;
		vertex1.z = m_model[index].z;
		vertex1.tu = m_model[index].tu;
		vertex1.tv = m_model[index].tv;
		vertex1.nx = m_model[index].nx;
		vertex1.ny = m_model[index].ny;
		vertex1.nz = m_model[index].nz;
		index++;

		vertex2.x = m_model[index].x;
		vertex2.y = m_model[index].y;
		vertex2.z = m_model[index].z;
		vertex2.tu = m_model[index].tu;
		vertex2.tv = m_model[index].tv;
		vertex2.nx = m_model[index].nx;
		vertex2.ny = m_model[index].ny;
		vertex2.nz = m_model[index].nz;
		index++;

		vertex3.x = m_model[index].x;
		vertex3.y = m_model[index].y;
		vertex3.z = m_model[index].z;
		vertex3.tu = m_model[index].tu;
		vertex3.tv = m_model[index].tv;
		vertex3.nx = m_model[index].nx;
		vertex3.ny = m_model[index].ny;
		vertex3.nz = m_model[index].nz;
		index++;

		// Calculate the tangent and binormal of this face
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent and binormal for this face back in the model structure
		m_model[index - 1].nx = normal.x;
		m_model[index - 1].ny = normal.y;
		m_model[index - 1].nz = normal.z;
		m_model[index - 1].tx = tangent.x;
		m_model[index - 1].ty = tangent.y;
		m_model[index - 1].tz = tangent.z;
		m_model[index - 1].bx = binormal.x;
		m_model[index - 1].by = binormal.y;
		m_model[index - 1].bz = binormal.z;

		m_model[index - 2].nx = normal.x;
		m_model[index - 2].ny = normal.y;
		m_model[index - 2].nz = normal.z;
		m_model[index - 2].tx = tangent.x;
		m_model[index - 2].ty = tangent.y;
		m_model[index - 2].tz = tangent.z;
		m_model[index - 2].bx = binormal.x;
		m_model[index - 2].by = binormal.y;
		m_model[index - 2].bz = binormal.z;

		m_model[index - 3].nx = normal.x;
		m_model[index - 3].ny = normal.y;
		m_model[index - 3].nz = normal.z;
		m_model[index - 3].tx = tangent.x;
		m_model[index - 3].ty = tangent.y;
		m_model[index - 3].tz = tangent.z;
		m_model[index - 3].bx = binormal.x;
		m_model[index - 3].by = binormal.y;
		m_model[index - 3].bz = binormal.z;
	}
}

void ModelClass::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vector
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross product and multiply by the coefficient to get the tangent and binormal
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of the tangent
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the tangent and store it
	tangent.x /= length;
	tangent.y /= length;
	tangent.z /= length;

	// Calculate the length of the binormal
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the binormal and store it
	binormal.x /= length;
	binormal.y /= length;
	binormal.z /= length;
}

void ModelClass::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;

	// Calculate the cross product of the tangent and binormal which will give the normal vector
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal and store it
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;
}
