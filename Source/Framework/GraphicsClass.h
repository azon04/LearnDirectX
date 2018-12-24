#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"

const bool FULLSCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render();

private:

	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
#if USING_TEXTURE
	TextureShaderClass* m_TextureShader;
#else
	ColorShaderClass* m_ColorShader;
#endif
};

#endif
