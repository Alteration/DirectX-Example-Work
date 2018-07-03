#ifndef VELOCITY_H
#define VELOCITY_H

#include "DirectXTK\SimpleMath.h"
#include <math.h>
using namespace DirectX::SimpleMath;

class Velocity
{
private:
	enum DIRECTIONS {
		NORTH,
		EAST,
		SOUTH,
		WEST
	};
	DIRECTIONS myDirection;
	float switchingPoint = 1.573744;
	float TERMINAL_VELOCITY = 0.05f;
	float storedTimestep;
	float directionalityY;
	float drag;
	bool m_onGround = true;
public:
	Velocity() { axisVelocity = Vector3(0, 0, 0); drag = 0.9; };
	~Velocity();
	Vector3 axisVelocity;
	void increaseVelocity(Vector3 inc, float dir, float timestep);
	void decreaseVelocity(Vector3 dec);
	void onGround(bool status);
	void dragOn();
	void deadStop() { axisVelocity = Vector3(0, axisVelocity.y, 0); }
	//void setDirection(float dir) { directionalityY = dir; }
	Vector3 getVelocity() { return axisVelocity; }
};

#endif // !VELOCITY_H
