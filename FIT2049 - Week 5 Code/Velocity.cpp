#include "Velocity.h"

void Velocity::increaseVelocity(Vector3 inc, float dir, float timestep) {
	storedTimestep = timestep;
	directionalityY = dir;

	float movement_per_timestep = 0.2f * timestep;
	axisVelocity.x += movement_per_timestep * inc.x;
	axisVelocity.y += movement_per_timestep * inc.y;
	axisVelocity.z += (movement_per_timestep / 2) * inc.z;

	if (axisVelocity.x > TERMINAL_VELOCITY) axisVelocity.x = TERMINAL_VELOCITY;
	if (axisVelocity.y > TERMINAL_VELOCITY) axisVelocity.y = TERMINAL_VELOCITY;
	if (axisVelocity.z > TERMINAL_VELOCITY) axisVelocity.z = TERMINAL_VELOCITY;
	if (axisVelocity.x < (TERMINAL_VELOCITY * -1)) axisVelocity.x = (TERMINAL_VELOCITY * -1);
	if (axisVelocity.y < (TERMINAL_VELOCITY * -1)) axisVelocity.y = (TERMINAL_VELOCITY * -1);
	if (axisVelocity.z < (TERMINAL_VELOCITY * -1)) axisVelocity.z = (TERMINAL_VELOCITY * -1);
}

void Velocity::decreaseVelocity(Vector3 dec) {
	
}

void Velocity::onGround(bool status){
	m_onGround = status;
}

//Adjust decrementing velocity depending on facing direction ( X- +x = -; X+ +x = +; Using associative rules adding a negative is a minus)
void Velocity::dragOn() {
	/*
	 * North = +Z Acceleration
	 * West = +X Acceleration
	 */

	if (axisVelocity.x > TERMINAL_VELOCITY) axisVelocity.x = TERMINAL_VELOCITY;
	if (axisVelocity.y > TERMINAL_VELOCITY) axisVelocity.y = TERMINAL_VELOCITY;
	if (axisVelocity.z > TERMINAL_VELOCITY) axisVelocity.z = TERMINAL_VELOCITY;
	if (axisVelocity.x < (TERMINAL_VELOCITY * -1)) axisVelocity.x = (TERMINAL_VELOCITY * -1);
	if (axisVelocity.y < (TERMINAL_VELOCITY * -1)) axisVelocity.y = (TERMINAL_VELOCITY * -1);
	if (axisVelocity.z < (TERMINAL_VELOCITY * -1)) axisVelocity.z = (TERMINAL_VELOCITY * -1);
	
	float fraction = drag * storedTimestep;
	
	//if (axisVelocity.x > 0 && ) axisVelocity.x -= fraction * axisVelocity.x;
	//axisVelocity = Vector3(0, 0, 0);
	if (axisVelocity.x > 0) axisVelocity.x -= fraction * abs(axisVelocity.x);
	//if (axisVelocity.y > 0) axisVelocity.y -= (fraction*2) * abs(axisVelocity.y);
	if (axisVelocity.z > 0) axisVelocity.z -= fraction * abs(axisVelocity.z);
	if (axisVelocity.x < 0) axisVelocity.x += fraction * abs(axisVelocity.x);
	//if (axisVelocity.y < 0) axisVelocity.y += (fraction * 2) * abs(axisVelocity.y);
	if (axisVelocity.z < 0) axisVelocity.z += fraction * abs(axisVelocity.z);
	if (!m_onGround){
		axisVelocity.y -= (fraction * 2) * 0.005;
	}
}