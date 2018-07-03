#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "Bullet.h"
#include "StateMachine.h"

class Enemy : public GameObject
{
private:
	enum ActionStates
	{
		NO_STATE = 0,
		IDLE_STATE,
		HUNTING_STATE,
		ATTACKING_STATE,
		STATE_COUNT
	};

	enum EnemyType
	{
		TRIANGLE,
		DIAMOND
	};
	
	float m_movespeed;
	float m_lookAtXRotation;
	int m_coolTime;

	EnemyType m_type;

	StateMachine<ActionStates, Enemy>* m_stateMachine;

	GameObject* m_target;

	bool m_hasWeapon;

	void Idle_OnEnter();
	void Idle_OnUpdate(float timestep);
	void Idle_Draw();
	void Idle_OnExit();

	void Hunt_OnEnter();
	void Hunt_OnUpdate(float timestep);
	void Hunt_Draw();
	void Hunt_OnExit();
	
	void Attack_OnEnter();
	void Attack_OnUpdate(float timestep);
	void Attack_Draw();
	void Attack_OnExit();

public:
	Enemy(Mesh* mesh);
	Enemy(Mesh* mesh, Vector3 position);

	~Enemy();

	float GetMovementSpeed() { return m_movespeed; }
	void SetMovementSpeed(float speed) { m_movespeed = speed; }
	void SetTarget(GameObject* target) { m_target = target; }
	void SetType(int type);

	void Update(float timestep);

	bool GetDeadOrAlive();
	bool CanShoot();

	void WasHit() { isDead = true; }
};

#endif // !ENEMY_H