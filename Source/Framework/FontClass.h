#ifndef _FONTCLASS_H_
#define _FONTCLASS_H_

// includes
#include <d3dx11.h>
#include <D3DX10math.h>
#include <fstream>
using namespace std;

#include "TextureClass.h"

class FontClass
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	FontClass();
	~FontClass();

	bool Initialize(ID3D11Device* device, char* fontFilename, WCHAR* textureFilename);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void* vertices, char* text, float drawX, float drawY);

private:
	bool LoadFontData(char* filename);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device* device, WCHAR* textureFilename);
	void ReleaseTexture();

private:
	FontType* m_Font;
	TextureClass* m_Texture;
};
#endif
