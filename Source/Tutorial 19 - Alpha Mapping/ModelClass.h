// ModelClass.h

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

using namespace std;

#include "TextureArrayClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	ID3D11ShaderResourceView** GetTextureArray();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3);
	void ReleaseTextures();

	bool LoadModel(char* modelFileName);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertextBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureArrayClass* m_textureArray;
	ModelType* m_model;
};

#endif