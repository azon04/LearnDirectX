#include "TextureClass.h"

TextureClass::TextureClass()
{
	m_texture = nullptr;
}

TextureClass::~TextureClass()
{

}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	// Load the texture in
	// #TODO fine a way to read the file independently
	HRESULT result = D3DX11CreateShaderResourceViewFromFile(device, filename, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureClass::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}
