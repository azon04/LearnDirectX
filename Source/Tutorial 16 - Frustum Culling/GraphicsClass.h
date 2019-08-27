#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

#include <windows.h>
#include "D3DClass.h"
#include "CameraClass.h"
#include "ModelClass.h"
#include "ColorShaderClass.h"
#include "TextureShaderClass.h"
#include "LightShaderClass.h"
#include "LightClass.h"
#include "BitmapClass.h"
#include "TextClass.h"
#include "InputClass.h"
#include "FrustumClass.h"
#include "ModelListClass.h"

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
	bool Frame(int fps, int cpu, float frameTime, float rotationY);
	bool Render(float delta, int mouseX, int mouseY);

private:

	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass* m_Model;
	TextureShaderClass* m_TextureShader;
#if USING_LIGHT
	LightShaderClass* m_LightShader;
	LightClass* m_light;
#else
	ColorShaderClass* m_ColorShader;
#endif

	BitmapClass* m_Bitmap;
	TextClass* m_Text;

	ModelListClass* m_ModelList;
	FrustumClass* m_Frustum;
};

#endif
