#include "TextureArrayClass.h"

TextureArrayClass::TextureArrayClass()
{
	m_textures[0] = nullptr;
	m_textures[1] = nullptr;
	m_textures[2] = nullptr;
}

TextureArrayClass::~TextureArrayClass()
{

}

bool TextureArrayClass::Initialize(ID3D11Device* device, WCHAR* textureFile1, WCHAR* textureFile2, WCHAR* textureFile3)
{
	HRESULT result;

	// Load the first texture
	result = D3DX11CreateShaderResourceViewFromFile(device, textureFile1, NULL, NULL, &m_textures[0], NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Load the second texture
	result = D3DX11CreateShaderResourceViewFromFile(device, textureFile2, NULL, NULL, &m_textures[1], NULL);
	if (FAILED(result))
	{
		return false;
	}

	if (textureFile3)
	{
		result = D3DX11CreateShaderResourceViewFromFile(device, textureFile3, NULL, NULL, &m_textures[2], NULL);
		if (FAILED(result))
		{
			return false;
		}
	}

	return true;
}

void TextureArrayClass::Shutdown()
{
	// Release the texture resources
	if (m_textures[0])
	{
		m_textures[0]->Release();
		m_textures[0] = nullptr;
	}

	if (m_textures[1])
	{
		m_textures[1]->Release();
		m_textures[1] = nullptr;
	}

	if (m_textures[2])
	{
		m_textures[2]->Release();
		m_textures[2] = nullptr;
	}
}

ID3D11ShaderResourceView** TextureArrayClass::GetTextureArray()
{
	return m_textures;
}
