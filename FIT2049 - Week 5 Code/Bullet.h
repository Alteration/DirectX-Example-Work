#ifndef BULLET_H
#define BULLET_H

#include "GameObject.h"
class Bullet : public GameObject
{
private:
	float m_speed;
	bool m_destroyed;
	float m_lookAtXRotation;
	//Vector3 m_direction;
public:
	Bullet();
	Bullet(Mesh* mesh, Vector3 position, Vector3 direction, float lookX);
	virtual ~Bullet();

	void Update(float timestep);

	//bool GetDeadOrAlive();
	void Dead() { isDead = true; }
};

#endif // !BULLET_H



