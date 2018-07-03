#include "SpinningObject.h"

SpinningObject::SpinningObject() : GameObject()
{
	m_spinningSpeed = 0.0f;
}
SpinningObject::SpinningObject(Mesh* mesh, float spinSpeed) : 
	GameObject(mesh)
{
	m_spinningSpeed = spinSpeed;
}
SpinningObject::SpinningObject(Mesh* mesh, Vector3 position, float spinSpeed) : 
	GameObject(mesh, position)
{
	m_spinningSpeed = spinSpeed;
}
SpinningObject::SpinningObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float spinSpeed) : 
	GameObject(mesh, position, rotX, rotY, rotZ)
{
	m_spinningSpeed = spinSpeed;
}
SpinningObject::SpinningObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ, float spinSpeed) :
	GameObject(mesh, position, rotX, rotY, rotZ, scaleX, scaleY, scaleZ)
{
	m_spinningSpeed = spinSpeed;
}
SpinningObject::~SpinningObject()
{

}

void SpinningObject::Update(float timestep)
{
	m_rotY += m_spinningSpeed * timestep;
}