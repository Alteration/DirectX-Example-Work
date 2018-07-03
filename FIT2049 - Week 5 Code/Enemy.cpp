#include "Enemy.h"

Enemy::Enemy(Mesh* mesh) : GameObject(mesh)
{
	m_hasWeapon = false;
	isDead = false;
	m_movespeed = 2.0f;

	m_stateMachine = new StateMachine<ActionStates, Enemy>(this, IDLE_STATE);
	m_stateMachine->RegisterState(IDLE_STATE, &Enemy::Idle_OnEnter,
		&Enemy::Idle_OnUpdate, &Enemy::Idle_Draw ,&Enemy::Idle_OnExit);

	m_stateMachine->RegisterState(HUNTING_STATE, &Enemy::Hunt_OnEnter,
		&Enemy::Hunt_OnUpdate, &Enemy::Hunt_Draw, &Enemy::Hunt_OnExit);

	m_stateMachine->RegisterState(ATTACKING_STATE, &Enemy::Attack_OnEnter,
		&Enemy::Attack_OnUpdate, &Enemy::Attack_Draw, &Enemy::Attack_OnExit);
}

Enemy::Enemy(Mesh* mesh, Vector3 position) : GameObject(mesh, position)
{
	m_hasWeapon = false;
	isDead = false;
	m_movespeed = 2.0f;

	m_stateMachine = new StateMachine<ActionStates, Enemy>(this, IDLE_STATE);
	m_stateMachine->RegisterState(IDLE_STATE, &Enemy::Idle_OnEnter,
		&Enemy::Idle_OnUpdate, &Enemy::Idle_Draw, &Enemy::Idle_OnExit);

	m_stateMachine->RegisterState(HUNTING_STATE, &Enemy::Hunt_OnEnter,
		&Enemy::Hunt_OnUpdate, &Enemy::Hunt_Draw, &Enemy::Hunt_OnExit);

	m_stateMachine->RegisterState(ATTACKING_STATE, &Enemy::Attack_OnEnter,
		&Enemy::Attack_OnUpdate, &Enemy::Attack_Draw, &Enemy::Attack_OnExit);
}

Enemy::~Enemy()
{

}

void Enemy::Idle_OnEnter() {  }
void Enemy::Idle_OnUpdate(float timestep)
{
	if (m_target != nullptr){
		if (m_type != TRIANGLE) m_stateMachine->ChangeState(HUNTING_STATE);
		if (m_type == TRIANGLE) m_stateMachine->ChangeState(ATTACKING_STATE);
	}
}
void Enemy::Idle_Draw() {  }
void Enemy::Idle_OnExit() {  }

void Enemy::Hunt_OnEnter() {  }
void Enemy::Hunt_OnUpdate(float timestep)
{
	if (m_type == DIAMOND)
	{
		m_rotY += 1.0f*timestep;
	}
	

	//NEED TO FIND A MORE NATURAL MOVEMENT ALGORITHM
	Vector3 tPos = m_target->GetPosition();
	if (tPos.x < m_position.x)
	{
		m_position -= Vector3(m_movespeed * timestep, 0, 0);
	}
	else{
		m_position += Vector3(m_movespeed * timestep, 0, 0);
	}
	
	if (tPos.z < m_position.z)
	{
		m_position -= Vector3(0, 0, m_movespeed * timestep);
	}
	else{
		m_position += Vector3(0, 0, m_movespeed * timestep);
	}
	
}
void Enemy::Hunt_Draw() {  }
void Enemy::Hunt_OnExit() {  }

void Enemy::Attack_OnEnter() {  }
void Enemy::Attack_OnUpdate(float timestep)
{
	m_rotY += 2.5f*timestep;
	m_hasWeapon = true;
}
void Enemy::Attack_Draw() {  }
void Enemy::Attack_OnExit() {  }

void Enemy::Update(float timestep)
{
	m_stateMachine->Update(timestep);
}

void Enemy::SetType(int type)
{
	if (type == 0)
	{
		m_type = DIAMOND;
	}
	else{
		m_type = TRIANGLE;
	}
}

bool Enemy::GetDeadOrAlive()
{
	return isDead;
}

bool Enemy::CanShoot()
{
	if (m_hasWeapon && m_coolTime == 0)
	{
		m_coolTime = 1000;
		return true;
	}
	if (m_coolTime > 0)
	{
		m_coolTime--;
	}
	return false;
}