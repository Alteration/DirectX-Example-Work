#include "Player.h"

Player::Player(Camera* cam, InputController* input) : GameObject()
{
	m_cam = cam;
	m_input = input;
	m_moveSpeed = 2.0f;
	m_rotationSpeed = 10.0f;
	m_lookAtXRotation = 0;
	m_healthPoints = 15;
	m_score = 0;
	m_velocity = new Velocity();
	m_onGround = true;
}

void Player::LoadBullet(Mesh* mesh)
{
	m_bulletMesh = mesh;
}

void Player::HitWall(){
	m_velocity->deadStop();
}

void Player::Update(float timestep)
{
	m_rotY -= (m_input->GetMouseXDelta() * m_rotationSpeed) * timestep;
	float xTempRot = (m_input->GetMouseYDelta() * m_rotationSpeed) * timestep;
	if (!(abs((m_lookAtXRotation + xTempRot)) > 1.45f))
	{
		m_lookAtXRotation += xTempRot;
	}

	if (abs(m_rotY) > 6.284876f){
		m_rotY = 0;
	}

	Vector3 movementV(0, 0, 0);
	Vector3 forward(0, 0, 1);
	Vector3 left(1, 0, 0);

	Matrix yRotation = Matrix::CreateRotationY(m_rotY);

	forward = Vector3::Transform(forward, yRotation);
	left = Vector3::Transform(left, yRotation);

	Vector3 temp;
	if (m_cam->GetPosition().y <= 1){
		m_velocity->onGround(true);
		m_onGround = true;
		m_velocity->axisVelocity.y = 0;
	}
	else{
		m_velocity->onGround(false);
		m_onGround = false;
	}

	if (m_input->GetKeyDown(VK_SHIFT))
	{
		timestep *= 2;
	}
	if ((m_input->GetKeyUp(VK_SPACE) && m_onGround))
	{
		movementV += (Vector3(0, 10, 0) * m_moveSpeed);
		m_onGround = false;
		m_velocity->onGround(false);
		//JUMPING
	}
	if (m_input->GetKeyDown('W'))
	{
		movementV += (forward);
		//m_position += (forward * m_moveSpeed) * timestep;
	}
	if (m_input->GetKeyDown('S'))
	{
		movementV -= (forward);
		//m_position -= (forward * m_moveSpeed) * timestep;
	}
	if (m_input->GetKeyDown('A'))
	{
		movementV += (left);
		//m_position += (left * m_moveSpeed) * timestep;
	}
	if (m_input->GetKeyDown('D'))
	{
		movementV -= (left);
		//m_position -= (left * m_moveSpeed) * timestep;
	}
	m_velocity->increaseVelocity(movementV, m_rotY, timestep);
	m_position += m_velocity->getVelocity();

	Matrix xRotation = Matrix::CreateRotationX(m_lookAtXRotation);
	Matrix lookAtRotation = xRotation * yRotation;
	Vector3 camPos = m_position + Vector3(0, 1, 0);
	Vector3 lookAt(0, 1, 10);
	lookAt = Vector3::Transform(lookAt, lookAtRotation);
	lookAt += m_position;
	
	m_cam->SetPosition(camPos);
	m_cam->SetLookAt(lookAt);
	m_velocity->dragOn();
}