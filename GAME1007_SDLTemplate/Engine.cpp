#include "Engine.h"
#include "SDL_image.h"
#include <ctime>
#include <iomanip>


#include "EventManager.h"
#include "StateManager.h"
#include "States.h"
#include "TextureManager.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				TextureManager::Init();
				EventManager::Init();
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	m_startLast = SDL_GetTicks();
	StateManager::PushState(new TitleState());
	m_running = true;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
	//DeltaTimeStuff
	m_deltaTime = (m_start - m_startLast) / 1000.0f;
	m_startLast = m_start;
	cout << fixed << setprecision(20);
	cout << m_deltaTime << endl;
}

void Engine::HandleEvents()
{
	EventManager::HandleEvents();
}


bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{ 
		if (m_keystates[c] == 1)
			return true;
	}
	return false;
}

void Engine::Update()
{
	//NEW//
	StateManager::Update();
}

void Engine::Render()
{
	//NEW
	StateManager::Render();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);

}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}

//Engine* Engine::Instance() //Pointer method
//{
//	static Engine* instance = new Engine();
//	return instance;
//}

Engine& Engine::Instance() //Static method
{
	static Engine instance;
	return instance;
}


void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	StateManager::Quit();
	EventManager::Quit();
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	TextureManager::Quit();
	IMG_Quit();
	SDL_Quit();
}

bool& Engine::Running()
{
	return m_running;
}

void Engine::SetRunning(const bool b)
{
	m_running = b;
}
