#include "PositionClass.h"

PositionClass::PositionClass()
{
	m_FrameTime = 0.0f;
	m_RotationY = 0.0f;
	m_leftTurnSpeed = 0.0f;
	m_rightTurnSpeed = 0.0f;
}

PositionClass::~PositionClass()
{

}

void PositionClass::SetFrameTime(float frameTime)
{
	m_FrameTime = frameTime;
}

void PositionClass::GetRotation(float& rotation)
{
	rotation = m_RotationY;
}

void PositionClass::TurnLeft(bool keyDown)
{
	// if the key is pressed increase the speed at which the camera turn left. if not slow down the turn speed
	if (keyDown)
	{
		m_leftTurnSpeed += m_FrameTime * 0.01f;
		if (m_leftTurnSpeed > (m_FrameTime * 0.15f))
		{
			m_leftTurnSpeed = m_FrameTime * 0.15f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_FrameTime * 0.005f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed
	m_RotationY -= m_leftTurnSpeed;
	if (m_RotationY < 0.0f)
	{
		m_RotationY += 360.0f;
	}
}

void PositionClass::TurnRight(bool keyDown)
{
	// if the key is pressed increase the speed at which the camera turn left. if not slow down the turn speed
	if (keyDown)
	{
		m_rightTurnSpeed += m_FrameTime * 0.01f;
		if (m_rightTurnSpeed > (m_FrameTime * 0.15f))
		{
			m_rightTurnSpeed = m_FrameTime * 0.15f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_FrameTime * 0.005f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	// Update the rotation using the turning speed
	m_RotationY += m_rightTurnSpeed;
	if (m_RotationY > 360.0f)
	{
		m_RotationY -= 360.0f;
	}
}
