#include "LightClass.h"

LightClass::LightClass()
{

}

LightClass::~LightClass()
{

}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor = D3DXVECTOR4(r, g, b, a);
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}

D3DXVECTOR3 LightClass::GetDirection()
{
	return m_direction;
}

