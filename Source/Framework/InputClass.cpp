#include "InputClass.h"

InputClass::InputClass()
{

}

InputClass::~InputClass()
{

}

void InputClass::Initialize()
{
	for (int i = 0; i < 256; i++)
	{
		m_keys[i] = false;
	}
}

void InputClass::KeyDown(unsigned int keyIndex)
{
	m_keys[keyIndex] = true;
}

void InputClass::KeyUp(unsigned int keyIndex)
{
	m_keys[keyIndex] = false;
}

bool InputClass::IsKeyDown(unsigned int keyIndex)
{
	return m_keys[keyIndex];
}
