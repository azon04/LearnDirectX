#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <math.h>

class PositionClass
{
public:
	PositionClass();
	~PositionClass();

	void SetFrameTime(float frameTime);
	void GetRotation(float& rotation);

	void TurnLeft(bool keyDown);
	void TurnRight(bool keyDown);

private:
	float m_FrameTime;
	float m_RotationY;
	float m_leftTurnSpeed, m_rightTurnSpeed;
};

#endif
