#ifndef SPINNINGOBJECT_H
#define SPINNINGOBJECT_H

#include "GameObject.h"

class SpinningObject : public GameObject
{
private:
	float m_spinningSpeed;

public:
	SpinningObject();
	SpinningObject(Mesh* mesh, float spinSpeed);
	SpinningObject(Mesh* mesh, Vector3 position, float spinSpeed);
	SpinningObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float spinSpeed);
	SpinningObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, float spinSpeed);
	virtual ~SpinningObject();

	float GetSpinningSpeed() { return m_spinningSpeed; }
	void SetSpinningSpeed(float spinningSpeed) { m_spinningSpeed = spinningSpeed; }

	void Update(float timestep);
};

#endif