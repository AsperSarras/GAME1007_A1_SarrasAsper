#include "Engine.h"
#include "SDL_image.h"
#include <ctime>
#include <iomanip>

#include "StateManager.h"
#include "States.h"
#include "TextureManager.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		//cout << "First pass." << endl;
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			//cout << "Second pass." << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				// Initialize subsystems
				//cout << "Third pass." << endl;

				TextureManager::Init();
				
				//Old
				//if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				//{
				//	m_pBGTexture = IMG_LoadTexture(m_pRenderer, "img/nBG.png");
				//	m_pTexture = IMG_LoadTexture(m_pRenderer, "img/ship3.png");
				//	m_pEText = IMG_LoadTexture(m_pRenderer, "img/ship.png");

				//}
				//else return false; //Image init faliled.
				//
				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048);
					Mix_AllocateChannels(16);
					m_PShot = Mix_LoadWAV("aud/pShoot1.wav");
					m_EShot = Mix_LoadWAV("aud/eShoot1.wav");
					m_Explosion = Mix_LoadWAV("aud/Exp.wav");
					m_Bgm = Mix_LoadMUS("aud/Bgm.mp3");
				}
				else return false; //Mixer init failed.
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
	m_bg = { {0,0,1024,768}, {0,0,1024,768} };
	m_bg2 = { {0,0,1024,768}, {1024,0,1024,768} };
	cout << "Initialization successful!" << endl;
	Mix_PlayMusic(m_Bgm, -1);
	Mix_VolumeMusic(20);
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

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
			//Shooting
		//case SDL_KEYUP:
		//	if ((event.key.keysym.sym == ' ')&& (m_player.alive == true))
		//	{
		//		//Spawn bullet
		//		m_bullets.push_back(new Bullet({ m_player.m_dst.x + 140,m_player.m_dst.y + 150 }));
		//		m_bullets.push_back(new Bullet({ m_player.m_dst.x + 140,m_player.m_dst.y + 62 }));
		//		m_bullets.shrink_to_fit();
		//		cout << m_bullets.capacity() << endl;
		//		Mix_PlayChannel(-1, m_PShot, 0);
		//	}
		}
	}
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
	
	//if (m_fps == 17)
	//{
	//	timer++;
	//}
	//Background
	//m_bg.m_dst.x -= m_speed;
	//m_bg2.m_dst.x -= m_speed;
	//
	//if (m_bg.m_dst.x <= -1024)
	//{
	//	m_bg.m_dst.x = 0;
	//	m_bg2.m_dst.x = 1024;
	//}
	//Keys
	//if (KeyDown(SDL_SCANCODE_W) && m_player.m_dst.y > 0)
	//	m_player.m_dst.y -= m_speed;
	//else if (KeyDown(SDL_SCANCODE_S) && m_player.m_dst.y<HEIGHT- m_player.m_dst.h)
	//	m_player.m_dst.y += m_speed;

	//if (KeyDown(SDL_SCANCODE_A) && m_player.m_dst.x>0)
	//	m_player.m_dst.x -= m_speed;
	//else if (KeyDown(SDL_SCANCODE_D) && m_player.m_dst.x < WIDTH - m_player.m_dst.w)
	//	m_player.m_dst.x += m_speed;
	
	////Enemy spawn
	//if ( timer == 200) {
	//	m_enemy.push_back(new Enemy);
	//	int yRange = HEIGHT - m_enemy.back()->m_dst.h;
	//	int randY = rand() % yRange;
	//	m_enemy.back()->SetDstY(randY);
	//	m_enemy.shrink_to_fit();
	//	cout << m_enemy.capacity() << endl;
	//	timer = 0;
	//}
	////Bullet update and collision
	//for (unsigned i = 0; i < m_bullets.size(); i++) //size() is actual filled numbers of elements
	//	m_bullets[i]->Update();

	//for (unsigned i = 0; i < m_bullets.size(); i++){ 
	//	if (m_bullets[i]->GetRekt()->x >= 1024) //Off screen
	//	{
	//		delete m_bullets[i]; //Flag for re alocation
	//		m_bullets[i] = nullptr; //Wrangle your dangle(?)
	//		m_bullets.erase(m_bullets.begin() + i);
	//		m_bullets.shrink_to_fit();
	//		Mix_PlayChannel(-1, m_Explosion, 0);
	//		break;
	//	}
	//}
	//for (unsigned i = 0; i < m_bullets.size(); i++) {
	//	for (unsigned u = 0; u < m_enemy.size(); u++) {
	//		if (SDL_HasIntersection(m_bullets[i]->GetRekt(), &m_enemy[u]->m_dst)) {
	//			
	//			delete m_bullets[i]; //Flag for re alocation
	//			m_bullets[i] = nullptr; //Wrangle your dangle(?)
	//			m_bullets.erase(m_bullets.begin() + i);
	//			m_bullets.shrink_to_fit();

	//			delete m_enemy[u]; //Flag for re alocation
	//			m_enemy[u] = nullptr; //Wrangle your dangle(?)
	//			m_enemy.erase(m_enemy.begin() + u);
	//			m_enemy.shrink_to_fit();

	//			Mix_PlayChannel(-1, m_Explosion, 0);
	//			break;

	//		}
	//	}
	//}
	////Enemy Update and collision
	//for (unsigned i = 0; i < m_enemy.size(); i++) //size() is actual filled numbers of elements
	//	m_enemy[i]->Update();
	//
	//for (unsigned i = 0; i < m_enemy.size(); i++) {
	//	if (m_enemy[i]->m_dst.x <= -(m_enemy[i]->m_dst.w)) //Off screen
	//	{
	//		delete m_enemy[i]; //Flag for re alocation
	//		m_enemy[i] = nullptr; //Wrangle your dangle(?)
	//		m_enemy.erase(m_enemy.begin() + i);
	//		m_enemy.shrink_to_fit();
	//		Mix_PlayChannel(-1, m_Explosion, 0);
	//		break;
	//	}
	//}
	////Crash collision
	//for (unsigned u = 0; u < m_enemy.size(); u++) {
	//	if (SDL_HasIntersection(&m_enemy[u]->m_dst, &m_player.m_dst)&&(m_player.alive == true)) {

	//		//Need to work on destroying m_player
	//		SDL_DestroyTexture(m_pTexture);
	//		Mix_PlayChannel(-1, m_Explosion, 0);
	//		m_player.alive = false;
	//		break;
	//	}
	//}

	////Enemy bullets spawn, update and collision
	//for (unsigned i = 0; i < m_eBullet.size(); i++) //size() is actual filled numbers of elements
	//	m_eBullet[i]->Update();
	//for (unsigned i = 0; i < m_enemy.size(); i++) {
	//	if (timer == 80)
	//	{
	//		//Spawn bullet
	//		m_eBullet.push_back(new enBullet({ m_enemy[i]->m_dst.x + 25,m_enemy[i]->m_dst.y + 150 }));
	//		m_eBullet.push_back(new enBullet({ m_enemy[i]->m_dst.x + 25,m_enemy[i]->m_dst.y + 62 }));
	//		m_eBullet.shrink_to_fit();
	//		cout << m_eBullet.capacity() << endl;
	//		Mix_PlayChannel(-1, m_EShot, 0);
	//	}
	//}
	//for (unsigned i = 0; i < m_eBullet.size(); i++) {
	//	if (m_eBullet[i]->GetRekt()->x <= -(m_eBullet[i]->GetRekt()->w)) //Off screen
	//	{
	//		delete m_eBullet[i]; //Flag for re alocation
	//		m_eBullet[i] = nullptr; //Wrangle your dangle(?)
	//		m_eBullet.erase(m_eBullet.begin() + i);
	//		m_eBullet.shrink_to_fit();
	//		Mix_PlayChannel(-1, m_Explosion, 0);
	//		break;
	//	}
	//	for (unsigned u = 0; u < m_eBullet.size(); u++) {
	//		if (SDL_HasIntersection(m_eBullet[i]->GetRekt(), &m_player.m_dst) && (m_player.alive == true)) {

	//			delete m_eBullet[i]; //Flag for re alocation
	//			m_eBullet[i] = nullptr; //Wrangle your dangle(?)
	//			m_eBullet.erase(m_eBullet.begin() + i);
	//			m_eBullet.shrink_to_fit();

	//			//Need to work on destroying m_player
	//			SDL_DestroyTexture(m_pTexture);
	//			Mix_PlayChannel(-1, m_Explosion, 0);
	//			m_player.alive = false;
	//			break;
	//		}
	//	}
	//}
}

void Engine::Render()
{
	//NEW
	StateManager::Render();
	//OLD
	//SDL_SetRenderDrawColor(m_pRenderer, 0, 128, 255, 255);
	//SDL_RenderClear(m_pRenderer);
	//SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg.m_src, &m_bg.m_dst);
	//SDL_RenderCopy(m_pRenderer, m_pBGTexture, &m_bg2.m_src, &m_bg2.m_dst);

	//SDL_RenderCopyEx(m_pRenderer, m_pTexture, &m_player.m_src, &m_player.m_dst, 90, NULL, SDL_FLIP_NONE);
	//	
	//for (unsigned i = 0; i < m_enemy.size(); i++) {
	//	SDL_RenderCopy(m_pRenderer, m_pEText, &m_enemy[i]->m_src, &m_enemy[i]->m_dst);
	//}
	//for (unsigned i = 0; i < m_bullets.size(); i++) {
	//	m_bullets[i]->Render(m_pRenderer);
	//}
	//for (unsigned i = 0; i < m_eBullet.size(); i++) {
	//	m_eBullet[i]->Render(m_pRenderer);
	//}
	//
	//SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
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
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pBGTexture);
	SDL_DestroyTexture(m_pTexture);
	Mix_FreeChunk(m_PShot);
	Mix_FreeChunk(m_EShot);
	Mix_FreeChunk(m_Explosion);
	Mix_FreeMusic(m_Bgm);
	TextureManager::Quit();
	IMG_Quit();
	SDL_Quit();
}


