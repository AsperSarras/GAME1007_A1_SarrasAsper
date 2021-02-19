#include "Button.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "StateManager.h"

Button::Button(SDL_Rect s, SDL_FRect d, const char* k) :SpriteObject(s, d), m_state(STATE_UP), m_key(k) {}

void Button::Update()
{
	bool col = CollisionManager::PointAABBCheck(EventManager::GetMousePos(), m_dst);
	switch (m_state)
	{
	case STATE_UP:
		if (col) m_state = STATE_OVER;
		break;
	case STATE_OVER:
		if (!col) m_state = STATE_UP;
		else if (col && EventManager::MousePressed(1)) m_state = STATE_DOWN; // 1 is left mouse.
		cout << "MouseOver" << endl;
		break;
	case STATE_DOWN:
		if (EventManager::MouseReleased(1)) // Left mouse released.
		{
			if (col)
			{
				m_state = STATE_OVER;
				Execute();
			}
			else m_state = STATE_UP;
		}
		break;
	}
}

void Button::Render()
{
	//m_src.x = m_src.w * (int)m_state; 
	SDL_RenderCopyF(Engine::Instance().getRenderer(), TextureManager::GetTexture(m_key), &m_src, &m_dst);
}

PlayButton::PlayButton(SDL_Rect s, SDL_FRect d, const char* k) :Button(s, d, k) {}

void PlayButton::Execute()
{
	StateManager::ChangeState(new GameState());
}

void ResumeButton::Execute()
{
	StateManager::PopState();
}

ResumeButton::ResumeButton(SDL_Rect s, SDL_FRect d, const char* k) :Button(s, d, k) {}

void MainButton::Execute()
{
	StateManager::ChangeState(new TitleState());
}

MainButton::MainButton(SDL_Rect s, SDL_FRect d, const char* k) :Button(s, d, k) {}
