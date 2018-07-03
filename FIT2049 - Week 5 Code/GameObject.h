#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "DirectXTK/SimpleMath.h"
#include "Direct3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Collisions.h"

using namespace DirectX::SimpleMath;

class GameObject
{
protected:
	Vector3 m_position;
	float m_rotX, m_rotY, m_rotZ;
	float m_scaleX, m_scaleY, m_scaleZ;
	
	bool isDead;
	Mesh* m_mesh;
private:
	//static CBoundingBox* g_worldBounds;
public:
	GameObject();
	GameObject(Mesh* mesh);
	GameObject(Mesh* mesh, Vector3 position);
	GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ);
	GameObject(Mesh* mesh, Vector3 position, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
	virtual ~GameObject();

	Vector3 GetPosition() { return m_position; }
	Vector3 GetRotation() { return Vector3(m_rotX, m_rotY, m_rotZ); }
	float GetXRotation() { return m_rotX; }
	float GetYRotation() { return m_rotY; }
	float GetZRotation() { return m_rotZ; }
	float GetXScale() { return m_scaleX; }
	float GetYScale() { return m_scaleY; }
	float GetZScale() { return m_scaleZ; }
	Mesh* GetMesh() { return m_mesh; }
	bool GetIsDead() { return isDead; }
	//CBoundingBox* GetWorldBounds() { return g_worldBounds; }

	void SetPosition(Vector3 pos) { m_position = pos; }
	void SetXRotation(float xRot) { m_rotX = xRot; }
	void SetYRotation(float yRot) { m_rotY = yRot; }
	void SetZRotation(float zRot) { m_rotZ = zRot; }
	void SetXScale(float xScale) { m_scaleX = xScale; }
	void SetYScale(float yScale) { m_scaleY = yScale; }
	void SetZScale(float zScale) { m_scaleZ = zScale; }

	//void SetWorldBounds(CBoundingBox* bounds) { g_worldBounds = bounds; }

	virtual void Update(float timestep) = 0;
	virtual void Render(Direct3D* direct3D, Camera* currentCamera);
};

#endif