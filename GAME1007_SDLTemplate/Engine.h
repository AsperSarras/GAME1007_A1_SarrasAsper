#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "SDL.h"
#include "SDL_image.h"
#include <vector>
#include <iostream>
#include "SDL_mixer.h"

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Sprite
{
public:
	SDL_Rect m_src;  //Source rectangle
	SDL_Rect m_dst;  //Destination Rectangle
	SDL_Rect m_Rect; //Single rectangle for destination.
};

class Player : public Sprite
{
public:

	bool alive = true;
	bool CD = false;
	int count = 0;

	Player() {
		m_src = { 0,0,826,682 };
		m_dst = { 206,384,215,221 };
	}
	~Player() {
		
	}
};

class Enemy :public Sprite
{
public:
	int m_frame = 0;
	int m_spawn = 10;
	bool spawn = false;
	Enemy() 
	{
		m_src = { 0,0,489,594 };
		m_dst = { 1200,100,154,221 };//500
	}
	~Enemy() //Destructor
	{
		cout << "De-alocating bullet at " << &(*this) << endl;
	}
	
	void SetDstY(int y) {
		m_dst.y = y;
	}
	void Update()
	{
			this->m_dst.x -= 4;//Number is translation
	}

};

class Bullet : public Sprite
{
public:
	Bullet(SDL_Point spawnLoc = { 512,384 }) //Non default constructor.
	{
		cout << "Cosntructing Bullet at " << &(*this) << endl;
		this->m_Rect.x = spawnLoc.x;// 'this->' is optional
		this->m_Rect.y = spawnLoc.y;// 'this->' is optional
		this->m_Rect.h = 8;
		this->m_Rect.w = 8;
		// this-> m_Rect = { spawnLoc.x, spawnLoc.y, 4,4 }; //Short Way
	}
	~Bullet() //Destructor
	{
		cout << "De-alocating bullet at " << &(*this) << endl;
	}

	void Update()
	{
		this->m_Rect.x += 10;//Number is translation
	}
	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &m_Rect);
	}
	SDL_Rect* GetRekt() { return &m_Rect; }
};

class enBullet: public Sprite
{
private:
	//SDL_Rect m_Rect; // Single rectangle for destination.
public:
	enBullet(SDL_Point spawnLoc = { 512,384 }) //Non default constructor.
	{
		cout << "Cosntructing Bullet at " << &(*this) << endl;
		this->m_Rect.x = spawnLoc.x;// 'this->' is optional
		this->m_Rect.y = spawnLoc.y;// 'this->' is optional
		this->m_Rect.h = 8;
		this->m_Rect.w = 8;
		// this-> m_Rect = { spawnLoc.x, spawnLoc.y, 4,4 }; //Short Way
	}
	~enBullet() //Destructor
	{
		cout << "De-alocating bullet at " << &(*this) << endl;
	}
	void SetLoc(SDL_Point loc)
	{
		m_Rect.x = loc.x;
		m_Rect.y = loc.y;
	}
	void Update()
	{
		this->m_Rect.x -= 10;//Number is translation
	}
	void Render(SDL_Renderer* rend)
	{
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderFillRect(rend, &m_Rect);
	}
	SDL_Rect* GetRekt() { return &m_Rect; }
};

class Engine
{
private: // private properties.
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps,m_startLast;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pBGTexture, * m_pTexture, * m_pEText;
	Sprite m_bg,m_bg2;
	Player m_player;
	int m_speed = 5; //In-Class Initialization, not normal.
	int timer=0;
	vector<Bullet*> m_bullets;
	vector<Enemy*> m_enemy;
	vector<enBullet*> m_eBullet;
	//Effects
	Mix_Chunk* m_PShot;
	Mix_Chunk* m_EShot;
	Mix_Chunk* m_Explosion;
	//Music track
	Mix_Music* m_Bgm;

	//int m_frameCtr = 0;
	//double angle = 0.0;

private: // private method prototypes
	Engine() {} //Prevents instantiation outside class
	//~Engine() = delete; //Prevents destruction with pointers/ for pointer ways
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags); 
	void Wake();
	void HandleEvents();
	//bool KeyDown(SDL_Scancode c);
	void Update();
	void Render();
	void Sleep();
	void Clean();
	//friend class State;

public: // public method prototypes.
	int Run();
	//static Engine* Instance(); //Making it a singleton /Pointer way
	static Engine& Instance(); //Making it a singleton /Static method
	SDL_Renderer* getRenderer() { return m_pRenderer; }
	bool KeyDown(SDL_Scancode c);
	float m_deltaTime;
	Uint32 getFps() { return m_fps; }
	bool& Running();
	void SetRunning(const bool b);
};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
