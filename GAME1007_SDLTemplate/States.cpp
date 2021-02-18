﻿#include "States.h"
#include "Engine.h"
#include "StateManager.h"

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().getRenderer());
}

void State::Resume() {}

//Start Title State
TitleState::TitleState(){}

void TitleState::Enter()
{
	cout << "Entering Title state" << endl;
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
		StateManager::ChangeState(new GameState());// Action to change state
}

void TitleState::Render()
{
	cout << "Rendering Title state" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 255, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().getRenderer());
	State::Render(); 
}

void TitleState::Exit()
{
	cout << "Exiting Title state" << endl;
}

//End Title State

//Start Game State

GameState::GameState(){}

void GameState::Enter()
{
	cout << "Entering Game state" << endl;
	/*m_box = { 100.0f,300.0f,50.0f,	50.0f };*/

	m_pBGTexture = IMG_LoadTexture(Engine::Instance().getRenderer(), "img/nBG.png");
	m_pTexture = IMG_LoadTexture(Engine::Instance().getRenderer(), "img/ship3.png");
	m_pEText = IMG_LoadTexture(Engine::Instance().getRenderer(), "img/ship.png");

	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
	Mix_AllocateChannels(16);
	m_PShot = Mix_LoadWAV("aud/pShoot1.wav");
	m_EShot = Mix_LoadWAV("aud/eShoot1.wav");
	m_Explosion = Mix_LoadWAV("aud/Exp.wav");
	m_Bgm = Mix_LoadMUS("aud/Bgm.mp3");

	m_bg = { {0,0,1024,768}, {0,0,1024,768} };
	m_bg2 = { {0,0,1024,768}, {1024,0,1024,768} };
	Mix_PlayMusic(m_Bgm, -1);
	Mix_VolumeMusic(20);
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
		StateManager::ChangeState(new TitleState());// Action to change state
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
		StateManager::PushState(new PauseState());//Add New PauseState
	//m_box.x += 50.0f * Engine::Instance().m_deltaTime; //50 pixel per sec

	if ((Engine::Instance().KeyDown(SDL_SCANCODE_SPACE) && (m_player.alive == true)))
	{
			//Spawn bullet
			m_bullets.push_back(new Bullet({ m_player.m_dst.x + 140,m_player.m_dst.y + 150 }));
			m_bullets.push_back(new Bullet({ m_player.m_dst.x + 140,m_player.m_dst.y + 62 }));				m_bullets.shrink_to_fit();
			cout << m_bullets.capacity() << endl;
			Mix_PlayChannel(-1, m_PShot, 0);
	}

	if (Engine::Instance().getFps() == 17)
	{
		timer++;
	}
	//Background
	m_bg.m_dst.x -= m_speed;
	m_bg2.m_dst.x -= m_speed;
	
	if (m_bg.m_dst.x <= -1024)
	{
		m_bg.m_dst.x = 0;
		m_bg2.m_dst.x = 1024;
	}
	//Keys
	if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && m_player.m_dst.y > 0)
		m_player.m_dst.y -= m_speed;
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && m_player.m_dst.y<HEIGHT- m_player.m_dst.h)
		m_player.m_dst.y += m_speed;

	if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_player.m_dst.x>0)
		m_player.m_dst.x -= m_speed;
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_player.m_dst.x < WIDTH - m_player.m_dst.w)
		m_player.m_dst.x += m_speed;

	//Enemy spawn
	if ( timer == 200) {
		m_enemy.push_back(new Enemy);
		int yRange = HEIGHT - m_enemy.back()->m_dst.h;
		int randY = rand() % yRange;
		m_enemy.back()->SetDstY(randY);
		m_enemy.shrink_to_fit();
		cout << m_enemy.capacity() << endl;
		timer = 0;
	}
	//Bullet update and collision
	for (unsigned i = 0; i < m_bullets.size(); i++) //size() is actual filled numbers of elements
		m_bullets[i]->Update();

	for (unsigned i = 0; i < m_bullets.size(); i++){ 
		if (m_bullets[i]->GetRekt()->x >= 1024) //Off screen
		{
			delete m_bullets[i]; //Flag for re alocation
			m_bullets[i] = nullptr; //Wrangle your dangle(?)
			m_bullets.erase(m_bullets.begin() + i);
			m_bullets.shrink_to_fit();
			Mix_PlayChannel(-1, m_Explosion, 0);
			break;
		}
	}
	for (unsigned i = 0; i < m_bullets.size(); i++) {
		for (unsigned u = 0; u < m_enemy.size(); u++) {
			if (SDL_HasIntersection(m_bullets[i]->GetRekt(), &m_enemy[u]->m_dst)) {
				
				delete m_bullets[i]; //Flag for re alocation
				m_bullets[i] = nullptr; //Wrangle your dangle(?)
				m_bullets.erase(m_bullets.begin() + i);
				m_bullets.shrink_to_fit();

				delete m_enemy[u]; //Flag for re alocation
				m_enemy[u] = nullptr; //Wrangle your dangle(?)
				m_enemy.erase(m_enemy.begin() + u);
				m_enemy.shrink_to_fit();

				Mix_PlayChannel(-1, m_Explosion, 0);
				break;

			}
		}
	}
	//Enemy Update and collision
	for (unsigned i = 0; i < m_enemy.size(); i++) //size() is actual filled numbers of elements
		m_enemy[i]->Update();
	
	for (unsigned i = 0; i < m_enemy.size(); i++) {
		if (m_enemy[i]->m_dst.x <= -(m_enemy[i]->m_dst.w)) //Off screen
		{
			delete m_enemy[i]; //Flag for re alocation
			m_enemy[i] = nullptr; //Wrangle your dangle(?)
			m_enemy.erase(m_enemy.begin() + i);
			m_enemy.shrink_to_fit();
			Mix_PlayChannel(-1, m_Explosion, 0);
			break;
		}
	}
	//Crash collision
	for (unsigned u = 0; u < m_enemy.size(); u++) {
		if (SDL_HasIntersection(&m_enemy[u]->m_dst, &m_player.m_dst)&&(m_player.alive == true)) {

			//Need to work on destroying m_player
			SDL_DestroyTexture(m_pTexture);
			Mix_PlayChannel(-1, m_Explosion, 0);
			m_player.alive = false;
			break;
		}
	}

	//Enemy bullets spawn, update and collision
	for (unsigned i = 0; i < m_eBullet.size(); i++) //size() is actual filled numbers of elements
		m_eBullet[i]->Update();
	for (unsigned i = 0; i < m_enemy.size(); i++) {
		if (timer == 80)
		{
			//Spawn bullet
			m_eBullet.push_back(new enBullet({ m_enemy[i]->m_dst.x + 25,m_enemy[i]->m_dst.y + 150 }));
			m_eBullet.push_back(new enBullet({ m_enemy[i]->m_dst.x + 25,m_enemy[i]->m_dst.y + 62 }));
			m_eBullet.shrink_to_fit();
			cout << m_eBullet.capacity() << endl;
			Mix_PlayChannel(-1, m_EShot, 0);
		}
	}
	for (unsigned i = 0; i < m_eBullet.size(); i++) {
		if (m_eBullet[i]->GetRekt()->x <= -(m_eBullet[i]->GetRekt()->w)) //Off screen
		{
			delete m_eBullet[i]; //Flag for re alocation
			m_eBullet[i] = nullptr; //Wrangle your dangle(?)
			m_eBullet.erase(m_eBullet.begin() + i);
			m_eBullet.shrink_to_fit();
			Mix_PlayChannel(-1, m_Explosion, 0);
			break;
		}
		for (unsigned u = 0; u < m_eBullet.size(); u++) {
			if (SDL_HasIntersection(m_eBullet[i]->GetRekt(), &m_player.m_dst) && (m_player.alive == true)) {

				delete m_eBullet[i]; //Flag for re alocation
				m_eBullet[i] = nullptr; //Wrangle your dangle(?)
				m_eBullet.erase(m_eBullet.begin() + i);
				m_eBullet.shrink_to_fit();

				//Need to work on destroying m_player
				SDL_DestroyTexture(m_pTexture);
				Mix_PlayChannel(-1, m_Explosion, 0);
				m_player.alive = false;
				break;
			}
		}
	}
}

void GameState::Render()
{
	cout << "Rendering Game state" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().getRenderer());
	//SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 255, 255, 255, 255);
	//SDL_RenderFillRectF(Engine::Instance().getRenderer(), &m_box);

	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 0, 128, 255, 255);
	SDL_RenderClear(Engine::Instance().getRenderer());
	SDL_RenderCopy(Engine::Instance().getRenderer(), m_pBGTexture, &m_bg.m_src, &m_bg.m_dst);
	SDL_RenderCopy(Engine::Instance().getRenderer(), m_pBGTexture, &m_bg2.m_src, &m_bg2.m_dst);

	SDL_RenderCopyEx(Engine::Instance().getRenderer(), m_pTexture, &m_player.m_src, &m_player.m_dst, 90, NULL, SDL_FLIP_NONE);
		
	for (unsigned i = 0; i < m_enemy.size(); i++) {
		SDL_RenderCopy(Engine::Instance().getRenderer(), m_pEText, &m_enemy[i]->m_src, &m_enemy[i]->m_dst);
	}
	for (unsigned i = 0; i < m_bullets.size(); i++) {
		m_bullets[i]->Render(Engine::Instance().getRenderer());
	}
	for (unsigned i = 0; i < m_eBullet.size(); i++) {
		m_eBullet[i]->Render(Engine::Instance().getRenderer());
	}

	if(dynamic_cast<GameState*>(StateManager::GetStates().back()))//Check to see if current State is of type GameState
	State::Render();

}

void GameState::Exit()
{
	cout << "Exiting Game state" << endl;
	for (unsigned i = 0; i < m_bullets.size(); i++) {
		delete m_bullets[i]; //Flag for re alocation
		m_bullets[i] = nullptr; //Wrangle your dangle(?)
	}
	m_bullets.clear(); //Wipe out all elements on a vector but leaves capacity (if 10 bullets were shot the elements would be 0 but the capacity will be 10)
	m_bullets.shrink_to_fit(); //Reduces capacity to size (if it has 0 elements capacity will turn to 0)
	for (unsigned i = 0; i < m_eBullet.size(); i++) {
		delete m_eBullet[i]; //Flag for re alocation
		m_eBullet[i] = nullptr; //Wrangle your dangle(?)
	}
	m_bullets.clear(); //Wipe out all elements on a vector but leaves capacity (if 10 bullets were shot the elements would be 0 but the capacity will be 10)
	m_bullets.shrink_to_fit(); //Reduces capacity to size (if it has 0 elements capacity will turn to 0)
	for (unsigned i = 0; i < m_enemy.size(); i++) {
		delete m_enemy[i]; //Flag for re alocation
		m_enemy[i] = nullptr; //Wrangle your dangle(?)
	}
	m_enemy.clear(); //Wipe out all elements on a vector but leaves capacity (if 10 bullets were shot the elements would be 0 but the capacity will be 10)
	m_enemy.shrink_to_fit(); //Reduces capacity to size (if it has 0 elements capacity will turn to 0)

	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pTexture);
	Mix_FreeChunk(m_PShot);
	Mix_FreeChunk(m_EShot);
	Mix_FreeChunk(m_Explosion);
	Mix_FreeMusic(m_Bgm);
}

void GameState::Resume()
{
	cout << "Resuming Game state" << endl;
}

//End Game State

//Start Pause State

PauseState::PauseState() {}

void PauseState::Enter()
{
	cout << "Entering Pause state" << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_O))
	StateManager::PopState();
}

void PauseState::Render()
{
	cout << "Rendering Pause state" << endl;
	//First Render GameState
	StateManager::GetStates().front()->Render();
	//Now Render rest of PauseState
	SDL_SetRenderDrawBlendMode(Engine::Instance().getRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 0, 255, 0, 128);
	SDL_Rect rect = { 256,128,512,512 };
	SDL_RenderFillRect(Engine::Instance().getRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	cout << "Exiting Pause state" << endl;
}
