#ifndef _TEXTUREARRAYCLASS_H_
#define _TEXTUREARRAYCLASS_H_

//includes
#include <d3d11.h>
#include <D3DX11tex.h>

class TextureArrayClass
{
public:
	TextureArrayClass();
	~TextureArrayClass();

	bool Initialize(ID3D11Device* device, WCHAR* textureFile1, WCHAR* textureFile2);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[2];
};

#endif // !_TEXTUREARRAYCLASS_H_
