#ifndef _FUSTRUM_CLASS_H_
#define _FUSTRUM_CLASS_H_

#include <D3DX10math.h>

class FrustumClass
{
public:
	FrustumClass();
	~FrustumClass();

	void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);

	bool CheckPoint(float x, float y, float z);
	bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);
	bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

private:
	D3DXPLANE m_planes[6];
};
#endif
