// ModelClass.h

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#include <fstream>
using namespace std;

#define USING_TEXTURE 1
#define USING_LIGHT 1 && USING_TEXTURE

#include "TextureClass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
#if USING_TEXTURE
		D3DXVECTOR2 texture;
#else
		D3DXVECTOR4 color;
#endif
#if USING_LIGHT
		D3DXVECTOR3 normal;
#endif
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

	bool Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename);
	void Shutdown();
	void Render(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device* device);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext* deviceContext);

	bool LoadTexture(ID3D11Device* device, WCHAR* filename);
	void ReleaseTexture();

	bool LoadModel(char* modelFileName);
	void ReleaseModel();

private:
	ID3D11Buffer *m_vertextBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_texture;
	ModelType* m_model;
};

#endif