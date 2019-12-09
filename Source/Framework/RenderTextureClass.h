// RenderTextureClass.h

#ifndef _RENDERTEXTURECLASS_H_
#define _RENDERTEXTURECLASS_H_

// includes
#include <d3d11.h>

class RenderTextureClass
{
public:
	RenderTextureClass();
	~RenderTextureClass();

	bool Initialize(ID3D11Device* device, int textureWidth, int textureHeight);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DeviceContext* deviceContext, ID3D11DepthStencilView* depthStencilView, float r, float g, float b, float a);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:

	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;
};
#endif