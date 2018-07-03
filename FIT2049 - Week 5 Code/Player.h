#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Camera.h"
#include "InputController.h"
#include "Bullet.h"
#include "Mesh.h"
#include "Velocity.h"
#include <vector>

class Player : public GameObject
{
private:
	Camera* m_cam;
	InputController* m_input;

	int m_healthPoints;

	Mesh* m_bulletMesh;
	float m_moveSpeed;
	float m_rotationSpeed;
	float m_lookAtXRotation;
	bool m_onGround;

	int m_score;

public:
	Player(Camera* cam, InputController* input);

	Velocity* m_velocity;

	void LoadBullet(Mesh* mesh);

	void Update(float timestep);

	int GetHP() { return m_healthPoints; }
	int GetScore() { return m_score; }
	
	float GetLookAt() { return m_lookAtXRotation; }
	
	void WasHit() { m_healthPoints--; }
	void HitWall();
	void onGround(bool value) { m_onGround = value; }
	Mesh* GetBullet() { return m_bulletMesh;}
	
	void AddScore(int add) { m_score += add; }


};

#endif