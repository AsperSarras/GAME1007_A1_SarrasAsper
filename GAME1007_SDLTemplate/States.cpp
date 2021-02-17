#include "States.h"
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
	m_box = { 100.0f,300.0f,50.0f,	50.0f };
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
		StateManager::ChangeState(new TitleState());// Action to change state
	else if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
		StateManager::PushState(new PauseState());//Add New PauseState
	m_box.x += 50.0f * Engine::Instance().m_deltaTime; //50 pixel per sec
}

void GameState::Render()
{
	cout << "Rendering Game state" << endl;
	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().getRenderer());
	SDL_SetRenderDrawColor(Engine::Instance().getRenderer(), 255, 255, 255, 255);
	SDL_RenderFillRectF(Engine::Instance().getRenderer(), &m_box);
	if(dynamic_cast<GameState*>(StateManager::GetStates().back()))//Check to see if current State is of type GameState
	State::Render();

}

void GameState::Exit()
{
	cout << "Exiting Game state" << endl;
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
