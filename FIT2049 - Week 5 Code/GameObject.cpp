#include "GameObject.h"

GameObject::GameObject()
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_mesh = NULL;
}
GameObject::GameObject(Mesh* mesh)
{
	m_position = Vector3::Zero;
	m_rotX = m_rotY = m_rotZ = 0;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_mesh = mesh;
}
GameObject::GameObject(Mesh* mesh, Vector3 position)
{
	m_position = position;
	m_rotX = m_rotY = m_rotZ = 0;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_mesh = mesh;
}
GameObject::GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ)
{
	m_position = position;
	m_rotX = rotX;
	m_rotY = rotY;
	m_rotZ = rotZ;
	m_scaleX = m_scaleY = m_scaleZ = 1.0f;
	m_mesh = mesh;
}
GameObject::GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ)
{
	m_position = position;
	m_rotX = rotX;
	m_rotY = rotY;
	m_rotZ = rotZ;
	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_scaleZ = scaleZ;
	m_mesh = mesh;
}
GameObject::~GameObject()
{

}

void GameObject::Render(Direct3D* direct3D, Camera* currentCamera)
{
	if (m_mesh)
	{
		Matrix world = Matrix::CreateScale(m_scaleX, m_scaleY, m_scaleZ) * Matrix::CreateFromYawPitchRoll(m_rotY, m_rotX, m_rotZ) * Matrix::CreateTranslation(m_position);
		m_mesh->Render(direct3D, world, currentCamera);
	}
}