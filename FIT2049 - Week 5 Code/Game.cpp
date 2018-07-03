/*	FIT2049 - Example Code
*	Game.cpp
*	Created by Elliott Wilson - 2015 - Monash Univeristy
*	Implementation of Game.h
*	Modified by Jamie Smeets - 2015
*/

#include "Game.h"
#include "DirectXTK/CommonStates.h"
#include "TexturedShader.h"
#include "PhongShader.h"
#include "ToonShader.h"
#include "SpinningObject.h"
#include "StaticObject.h"

Game::Game()
{
	m_renderer = NULL;
	m_input = NULL;
	m_currentCam = NULL;
	m_shader = NULL;
	m_button = NULL;

	srand(NULL);

	m_stateMachine = new StateMachine<GameStates, Game>(this, NO_STATE);
	m_stateMachine->RegisterState(MENU_STATE, &Game::Menu_OnEnter, 
		&Game::Menu_OnUpdate, &Game::Menu_Draw, &Game::Menu_OnExit);

	m_stateMachine->RegisterState(GAMEPLAY_STATE, &Game::Gameplay_OnEnter,
		&Game::Gameplay_OnUpdate, &Game::Gameplay_Draw, &Game::Gameplay_OnExit);

	m_stateMachine->RegisterState(GAMEOVER_STATE, &Game::GameOver_OnEnter,
		&Game::GameOver_OnUpdate, &Game::GameOver_Draw, &Game::GameOver_OnExit);
}

Game::~Game()
{

}

bool Game::Initialise(Direct3D* renderer, InputController* input)
{
	m_renderer = renderer;		//Set our renderer
	m_input = input;			//Set our input controller

	//Create a new "default" camera
	m_currentCam = new Camera();

	m_lighting = new SceneLighting();
	m_lighting->SetLightDirection(Vector3(1, 0.5f, 0.3f));

	m_shader = new Shader();	//Create our shader
	//Initialise the shader, we need to pass in the shader filenames
	if (!m_shader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/PixelShader.ps", NULL))
	{
		return false;
	}

	m_texturedShader = new TexturedShader();
	if (!m_texturedShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/VertexShader.vs", L"Assets/Shaders/TexturedPixelShader.ps", NULL))
	{
		return false;
	}

	m_phongShader = new PhongShader(m_lighting, m_currentCam);
	if (!m_phongShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/PhongVertex.vs", L"Assets/Shaders/PhongPixel.ps", NULL))
	{
		return false;
	}

	//Loading our ship texture
	Texture* objectTexture = new Texture();
	if (!objectTexture->Initialise(m_renderer, L"Assets/Textures/orange.png"))
	{
		return false;
	}

	//Loading our button texture
	Texture* buttonTexture = new Texture();
	if (!buttonTexture->Initialise(m_renderer, L"Assets/Textures/Button.png"))
	{
		return false;
	}
	
	Texture* rampTexture = new Texture();
	if (!rampTexture->Initialise(m_renderer, L"Assets/Textures/ramp.png"))
	{
		return false;
	}

	Texture* levelTexture = new Texture();
	if (!levelTexture->Initialise(m_renderer, L"Assets/Textures/level.png"))
	{
		return false;
	}

	Texture* skyboxTexture = new Texture();
	if (!skyboxTexture->Initialise(m_renderer, L"Assets/Textures/skybox.png"))
	{
		return false;
	}

	Texture* bulletTexture = new Texture();
	if (!bulletTexture->Initialise(m_renderer, L"Assets/Textures/blast.png"))
	{
		return false;
	}

	Texture* rTexture = new Texture();
	if (!rTexture->Initialise(m_renderer, L"Assets/Textures/red.png"))
	{
		return false;
	}

	Texture* pTexture = new Texture();
	if (!pTexture->Initialise(m_renderer, L"Assets/Textures/purple.png"))
	{
		return false;
	}

	m_toonShader = new ToonShader(m_lighting, m_currentCam, rampTexture);
	if (!m_toonShader->Initialise(m_renderer->GetDevice(), L"Assets/Shaders/PhongVertex.vs", L"Assets/Shaders/ToonPixel.ps", NULL))
	{
		return false;
	}
	
	Mesh* level = new Mesh();
	if (!level->Initialise(m_renderer, "Assets/Meshes/level.obj", Color(1.0f, 1.0f, 1.0f, 1.0f), levelTexture, m_texturedShader))
	{
		return false;
	}

	Mesh* object = new Mesh();
	if (!object->Initialise(m_renderer, "Assets/Meshes/orangeEnemy.obj", Color(1.0f, 1.0f, 1.0f, 1.0f), objectTexture, m_phongShader))
	{
		return false;
	}

	Mesh* skybox = new Mesh();
	if (!skybox->Initialise(m_renderer, "Assets/Meshes/skybox.obj", Color(1, 1, 1, 1), skyboxTexture, m_texturedShader))
	{
		return false;
	}

	Mesh* bullet = new Mesh();
	if (!bullet->Initialise(m_renderer, "Assets/Meshes/blast.obj", Color(1, 1, 1, 1), bulletTexture, m_toonShader))
	{
		return false;
	}

	bulletMesh = bullet;

	redEnemy = new Mesh();
	if (!redEnemy->Initialise(m_renderer, "Assets/Meshes/redEnemy.obj", Color(1, 1, 1, 1), rTexture, m_toonShader))
	{
		return false;
	}

	purpleEnemy = new Mesh();
	if (!purpleEnemy->Initialise(m_renderer, "Assets/Meshes/purpleEnemy.obj", Color(1, 1, 1, 1), pTexture, m_toonShader))
	{
		return false;
	}

	StaticObject* boardObject = new StaticObject(level, Vector3::Zero);
	m_gameObjects.push_back(boardObject);

	StaticObject* skyboxObject = new StaticObject(skybox, Vector3::Zero);
	m_gameObjects.push_back(skyboxObject);

	//Create the sprite batch
	m_spriteBatch = new SpriteBatch(m_renderer->GetDeviceContext());
	//Load our Sprite Font
	m_arial = new SpriteFont(m_renderer->GetDevice(), L"Assets/Fonts/Arial-12pt.spritefont");

	m_button = new Button(128, 64, buttonTexture, L"Start Game", Vector2(500, 500), m_spriteBatch, m_arial, m_input, [this]
	{
		//Set button to change state, causes gameplay to start
		m_stateMachine->ChangeState(GAMEPLAY_STATE);
	});

	m_qbutton = new Button(128, 64, buttonTexture, L"Quit Game", Vector2(700, 500), m_spriteBatch, m_arial, m_input, [this]
	{
		//Quit the game safely.
		Shutdown();
		exit(0);
	});

	m_player = new Player(m_currentCam, m_input);
	

	m_gameObjects.push_back(m_player);

	//Set Boundings//
	m_levelBounds = new CBoundingBox(Vector3(-35, -35, -35), Vector3(35, 35, 35));
	m_negX = new CPlane(Vector3(-1, 0, 0), 35);
	m_posX = new CPlane(Vector3(1, 0, 0), 35);
	m_negZ = new CPlane(Vector3(0, 0, -1), 35);
	m_posZ = new CPlane(Vector3(0, 0, 1), 34);
	m_playerBounds = new CBoundingSphere(m_player->GetPosition(), 2);
	//End Boundings

	m_player->LoadBullet(bullet);

	for (unsigned int i = 0; i < 15 ; i++)
	{
		//NEED TO STOP ENEMIES FROM SPAWNING IN EACH OTHER
		int type = rand() %2;
		int negatex = rand() %2;
		int negatez = rand() % 2;
		
		if (negatex == 0)
		{
			negatex == -1;
		}
		if (negatez == 0)
		{
			negatez = -1;
		}

		int xpos = (rand() % 30) * negatex;
		int zpos = (rand() % 30) * negatez;
		
		Enemy* tempEnemy;
		
		if (type == 0)
		{
			tempEnemy = new Enemy(redEnemy, Vector3(xpos, 1, zpos));
			tempEnemy->SetType(0);
		}
		else{
			tempEnemy = new Enemy(purpleEnemy, Vector3(xpos, 1, zpos));
			tempEnemy->SetType(1);
		}
		tempEnemy->SetTarget(m_player);

		m_enemyBounds.push_back(new CBoundingBox(tempEnemy->GetPosition() - Vector3(1, 1, 1), tempEnemy->GetPosition() + Vector3(1, 1, 1)));
		m_gameObjects.push_back(tempEnemy);
		m_enemyPopulation.push_back(tempEnemy);
	}
	
	m_stateMachine->ChangeState(MENU_STATE);
	
	//CreateThread(NULL, 0, thread_func, new thread_data(0), 0, 0);

	return true;
}

void Game::Menu_OnEnter()
{

}
void Game::Menu_OnUpdate(float timestep)
{
	if (m_input->GetKeyUp(VK_RETURN))
	{
		m_stateMachine->ChangeState(GAMEPLAY_STATE);
	}

	m_button->Update();
	m_qbutton->Update();
}
void Game::Menu_Draw()
{
	//When we want to draw 2D objects we need to do it within the Begin and End calls of the Sprite Batch
	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());

	m_arial->DrawString(m_spriteBatch, L"Jamie's generic fps!", Vector2(200, 200));
	m_button->Render();
	m_qbutton->Render();

	m_spriteBatch->End();
}
void Game::Menu_OnExit()
{

}

void Game::Gameplay_OnEnter()
{

	m_button->SetButtonText(L"Resume");
}
void Game::Gameplay_OnUpdate(float timestep)
{
	m_currentCam->Update(timestep);		//Every update we will update the camera

	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Update(timestep);
	}

	if (m_input->GetMouseUp(0)){
		Bullet* tempB = new Bullet(m_player->GetBullet(), m_player->GetPosition() + Vector3(0,0.5,0), m_player->GetRotation(), m_player->GetLookAt());

		m_gameObjects.push_back(tempB);
		m_playerBullets.push_back(tempB);
		m_playerBBounds.push_back(new CBoundingBox(tempB->GetPosition() - Vector3(1,1,1), tempB->GetPosition() + Vector3(1,1,1)));
	}

	for (unsigned int i = 0; i < m_enemyPopulation.size(); i++)
	{
		if (m_enemyPopulation[i]->CanShoot())
		{
			//Make enemy bullet
			Bullet* tempB = new Bullet(bulletMesh, m_enemyPopulation[i]->GetPosition() + Vector3(0, 0, 0), m_enemyPopulation[i]->GetRotation(), 0.0f);

			m_gameObjects.push_back(tempB);
			m_enemyBullets.push_back(tempB);
			m_enemyBBounds.push_back(new CBoundingBox(tempB->GetPosition() - Vector3(1, 1, 1), tempB->GetPosition() + Vector3(1, 1, 1)));
		}
	}


	std::vector<GameObject*>::iterator it = m_gameObjects.begin();
	while (it != m_gameObjects.end())
	{
		if ((*it)->GetIsDead())
		{
			it = m_gameObjects.erase(it);
		}
		else{
			++it;
		}
	}

	std::vector<Bullet*>::iterator bit = m_playerBullets.begin();
	while (bit != m_playerBullets.end())
	{
		if ((*bit)->GetIsDead())
		{
			bit = m_playerBullets.erase(bit);
		}
		else{
			++bit;
		}
	}

	bit = m_enemyBullets.begin();
	while (bit != m_enemyBullets.end())
	{
		if ((*bit)->GetIsDead())
		{
			bit = m_enemyBullets.erase(bit);
		}
		else{
			++bit;
		}
	}

	for (unsigned int i = 0; i < m_enemyPopulation.size(); i++)
	{
		if (m_enemyPopulation[i]->GetDeadOrAlive())
		{
			//srand(NULL);
			int type = rand() % 10;
			int negatex = rand() % 2;
			int negatez = rand() % 2;

			if (negatex == 0)
			{
				negatex == -1;
			}
			if (negatez == 0)
			{
				negatez = -1;
			}

			int xpos = rand() % 30;
			xpos *= negatex;
			int zpos = (rand() % 30);
			zpos *= negatez;

			Enemy* tempEnemy;

			if (type < 6)
			{
				tempEnemy = new Enemy(redEnemy, Vector3(xpos, 1, zpos));
				tempEnemy->SetType(0);
			}
			else{
				tempEnemy = new Enemy(purpleEnemy, Vector3(xpos, 1, zpos));
				tempEnemy->SetType(1);
			}

			tempEnemy->SetTarget(m_player);
			m_gameObjects.push_back(tempEnemy);
			m_enemyPopulation[i] = tempEnemy;
		}
	}

	//difficulty ramp
	if (m_player->GetScore() % 50 == 0 && !used && m_player->GetScore() > 0){
		used = true;
		int type = rand() % 2;
		int negatex = rand() % 2;
		int negatez = rand() % 2;

		if (negatex == 0)
		{
			negatex == -1;
		}
		if (negatez == 0)
		{
			negatez = -1;
		}

		int xpos = (rand() % 30) * negatex;
		int zpos = (rand() % 30) * negatez;

		Enemy* tempEnemy;

		if (type == 0)
		{
			tempEnemy = new Enemy(redEnemy, Vector3(xpos, 1, zpos));
			tempEnemy->SetType(0);
		}
		else{
			tempEnemy = new Enemy(purpleEnemy, Vector3(xpos, 1, zpos));
			tempEnemy->SetType(1);
		}
		tempEnemy->SetTarget(m_player);

		m_enemyBounds.push_back(new CBoundingBox(tempEnemy->GetPosition() - Vector3(1, 1, 1), tempEnemy->GetPosition() + Vector3(1, 1, 1)));
		m_gameObjects.push_back(tempEnemy);
		m_enemyPopulation.push_back(tempEnemy);
	}

	//RUDIMENTARY COLLISION CHECKS
	UpdateCollisions();

	//Player Collides with level
	
	Vector3 tempPosition = m_player->GetPosition();
	if (!(CheckPlane((*m_negX), (*m_playerBounds)) == PLANE_IN_FRONT))
	{
		m_player->HitWall();
		tempPosition.x -= 0.02f;
		m_player->SetPosition(tempPosition);
	}
	if (!(CheckPlane((*m_posX), (*m_playerBounds)) == PLANE_IN_FRONT))
	{
		m_player->HitWall();
		tempPosition.x += 0.02f;
		m_player->SetPosition(tempPosition);
	}
	if (!(CheckPlane((*m_negZ), (*m_playerBounds)) == PLANE_IN_FRONT))
	{
		m_player->HitWall();
		tempPosition.z -= 0.02f;
		m_player->SetPosition(tempPosition);
	}
	if (!(CheckPlane((*m_posZ), (*m_playerBounds)) == PLANE_IN_FRONT))
	{
		m_player->HitWall();
		tempPosition.z += 0.02f;
		m_player->SetPosition(tempPosition);
	}

	/*
	if (CheckCollision(*m_playerBounds, *m_cubeBound)){
		if (m_player->GetPosition().y > 0 && m_player->GetPosition().y <= m_friendly[0]->GetPosition().y){
			m_player->onGround(true);
		}
		else{
			m_player->HitWall();
		}

	}
	else{
		if (m_player->GetPosition().y > 1){
			m_player->onGround(false);
		}
	}
	*/

	for (unsigned int i = 0; i < m_enemyBounds.size(); i++)
	{
		if (CheckCollision((*m_playerBounds), (*m_enemyBounds[i])))
		{
			if (iframes == 0){
				//m_player->WasHit();
				iframes = 1000;
				//possible game over
			}
			
		}

		for (unsigned int j = 0; j < m_playerBullets.size(); j++)
		{
			if (CheckCollision((*m_enemyBounds[i]), (*m_playerBBounds[j])))
			{
				m_enemyPopulation[i]->WasHit();
				m_playerBullets[j]->Dead();
				m_player->AddScore(10);
				used = false;
			}
		}
	}

	for (unsigned int i = 0; i < m_enemyBullets.size(); i++)
	{
		if (CheckCollision((*m_playerBounds), (*m_enemyBBounds[i])))
		{
			if (iframes == 0){
				//m_player->WasHit();
				m_enemyBullets[i]->Dead();
				iframes = 1000;
				//possible game over
			}
		}
	}

	for (unsigned int i = 0; i < m_playerBullets.size(); i++)
	{
		if (!(m_playerBullets[i]->GetPosition().x < m_levelBounds->GetMax().x && m_playerBullets[i]->GetPosition().x > m_levelBounds->GetMin().x) ||
			!(m_playerBullets[i]->GetPosition().z < m_levelBounds->GetMax().z && m_playerBullets[i]->GetPosition().z > m_levelBounds->GetMin().z))
		{
			m_playerBullets[i]->Dead();
		}
	}

	for (unsigned int i = 0; i < m_enemyBullets.size(); i++)
	{
		if (!(m_enemyBullets[i]->GetPosition().x < m_levelBounds->GetMax().x && m_enemyBullets[i]->GetPosition().x > m_levelBounds->GetMin().x) ||
			!(m_enemyBullets[i]->GetPosition().z < m_levelBounds->GetMax().z && m_enemyBullets[i]->GetPosition().z > m_levelBounds->GetMin().z))
		{
			m_enemyBullets[i]->Dead();
		}
	}

	
	if (m_input->GetKeyDown(VK_ESCAPE))
	{
		//PAUSE MENU
		m_stateMachine->ChangeState(MENU_STATE);
	}

	if (iframes > 0) iframes--;
	if (m_player->GetHP() <= 0) m_stateMachine->ChangeState(GAMEOVER_STATE);

	m_button->Update();					//Just like every game object we create we need to update the Button here

}
void Game::Gameplay_Draw()
{
	for (unsigned int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects[i]->Render(m_renderer, m_currentCam);
	}

	//When we want to draw 2D objects we need to do it within the Begin and End calls of the Sprite Batch
	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	std::wstring ws;
	ws = L"HP: ";
	ws.append(std::to_wstring(m_player->GetHP()));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 10));
	ws = L"Score: ";
	ws.append(std::to_wstring(m_player->GetScore()));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 35));

	//PRINT DEBUG INFORMATION
	//Player's X Position
	ws = std::to_wstring(m_player->GetPosition().x);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 60));
	//Player's Y Position
	ws = std::to_wstring(m_player->GetPosition().y);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 85));
	//Player's Z Position
	ws = std::to_wstring(m_player->GetPosition().z);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 110));
	//Player's Lookat X
	ws = std::to_wstring(m_player->GetLookAt());
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 135));
	//Player's Lookat Y
	ws = std::to_wstring(m_player->GetRotation().y);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 160));
	//Boundary Checks
	ws = std::to_wstring(CheckPlane((*m_negX), (*m_playerBounds)));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 185));
	ws = std::to_wstring(CheckPlane((*m_posX), (*m_playerBounds)));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 210));
	ws = std::to_wstring(CheckPlane((*m_negZ), (*m_playerBounds)));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 235));
	ws = std::to_wstring(CheckPlane((*m_posZ), (*m_playerBounds)));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 260));
	//Velocity Values
	ws = std::to_wstring(m_player->m_velocity->axisVelocity.x);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 285));
	ws = std::to_wstring(m_player->m_velocity->axisVelocity.y);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 310));
	ws = std::to_wstring(m_player->m_velocity->axisVelocity.z);
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 335));
	//END DEBUG PRINTS

	//m_button->Render();		//The Button rendering needs to be done within the Begin and End calls of the SpriteBatch

	m_spriteBatch->End();
}
void Game::Gameplay_OnExit()
{

}

void Game::GameOver_OnEnter()
{

}
void Game::GameOver_OnUpdate(float timestep)
{
	m_qbutton->Update();
}
void Game::GameOver_Draw()
{
	CommonStates states(m_renderer->GetDevice());
	m_spriteBatch->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());
	std::wstring ws;

	ws = L"Game Over, your score was: ";
	ws.append(std::to_wstring(m_player->GetScore()));
	m_arial->DrawString(m_spriteBatch, ws.c_str(), Vector2(10, 235));
	m_qbutton->Render();
	
	m_spriteBatch->End();
}
void Game::GameOver_OnExit()
{

}

void Game::UpdateCollisions()
{
	//MULTITHREAD LATER 2b?
	//Need to change way data is stored for multithreading as it can't access variables
	//Change to iterator?
	m_playerBounds->SetCenter(m_player->GetPosition());

	for (unsigned int i = 0; i < m_enemyPopulation.size(); i++)
	{
		m_enemyBounds[i]->SetMax(m_enemyPopulation[i]->GetPosition());// +Vector3(2, 2, 2));
		m_enemyBounds[i]->SetMin(m_enemyPopulation[i]->GetPosition());// -Vector3(2, 2, 2));
	}

	for (unsigned int i = 0; i < m_enemyBullets.size(); i++)
	{
		m_enemyBBounds[i]->SetMax(m_enemyBullets[i]->GetPosition());// +Vector3(1, 1, 1));
		m_enemyBBounds[i]->SetMin(m_enemyBullets[i]->GetPosition());// -Vector3(1, 1, 1));
	}

	for (unsigned int i = 0; i < m_playerBullets.size(); i++)
	{
		m_playerBBounds[i]->SetMax(m_playerBullets[i]->GetPosition() + Vector3(1, 1, 1));
		m_playerBBounds[i]->SetMin(m_playerBullets[i]->GetPosition() - Vector3(1, 1, 1));
	}

}

void Game::Update(float timestep)
{
	m_input->BeginUpdate();				//The input controller needs to be updated every frame
	
	m_stateMachine->Update(timestep);
	
	m_input->EndUpdate();				//We also need to tell the Input Controller when a frame is over
}

void Game::ButtonMethod()
{
	//You could do anything you like here!
	MessageBox(NULL, "This method was called by our button using a lambda expression!", "This is pretty cool!", MB_OK);
}

void Game::Render()
{
	m_renderer->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);						//Begin the scene and clear to black

	m_renderer->SetCurrentShader(NULL);

	m_stateMachine->Draw();
	
	m_renderer->EndScene();												//End the scene and present the frame

}

void Game::Shutdown()
{
	//Clean up everything!
	if (m_currentCam)
	{
		delete m_currentCam;
		m_currentCam = NULL;
	}

	if (m_shader)
	{
		m_shader->Release();
		delete m_shader;
		m_shader = NULL;
	}

	if (m_texturedShader)
	{
		m_texturedShader->Release();
		delete m_texturedShader;
		m_texturedShader = NULL;
	}

	if (m_arial)
	{
		delete m_arial;
		m_arial = NULL;
	}

	if (m_spriteBatch)
	{
		delete m_spriteBatch;
		m_spriteBatch = NULL;
	}

	if (m_button)
	{
		delete m_button;
		m_button = NULL;
	}
}