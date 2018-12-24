// ModelClass.h

#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <d3dx10math.h>

#define USING_TEXTURE 1

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
	};

public:
	ModelClass();
	~ModelClass();

	bool Initialize(ID3D11Device* device, WCHAR* textureFilename);
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

private:
	ID3D11Buffer *m_vertextBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	TextureClass* m_texture;
};

#endif