#include "Bullet.h"


Bullet::Bullet() : GameObject()
{
	m_speed = 10.0f;
	m_destroyed = false;
	//m_direction = Vector3(1,0.5,0);
}

Bullet::Bullet(Mesh* mesh, Vector3 position, Vector3 direction, float lookX) : GameObject(mesh, position)
{
	m_speed = 10.0f;
	m_destroyed = false;
	//m_direction = direction;
	
	m_rotX = lookX;
	m_rotY = direction.y;
	m_rotZ = direction.z;

	m_lookAtXRotation = lookX;
	//m_position += m_direction;
}

Bullet::~Bullet()
{
}

void Bullet::Update(float timestep)
{
	Vector3 forward(0, 0, 1);
	Matrix yRotation = Matrix::CreateRotationY(m_rotY);
	Matrix xRotation = Matrix::CreateRotationX(m_lookAtXRotation);
	forward = Vector3::Transform(forward, xRotation); 
	forward = Vector3::Transform(forward, yRotation);

	m_position += (forward * m_speed) * timestep;
}
