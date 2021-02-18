#pragma once
#ifndef _STATES_H_
#define _STATES_H_
#include <map>
#include <SDL_mixer.h>
#include "Engine.h"
#include "GameObject.h"
#include "SDL.h"

class State //Abstract base class
{
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume();
	virtual ~State() = default;
protected:
	State(){}
	map<std::string, GameObject*> m_objects;
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	
};

class PauseState : public State
{
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();

};

class GameState : public State
{
public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
private:
	//SDL_FRect m_box;
	SDL_Texture* m_pBGTexture, * m_pTexture, * m_pEText;
	Sprite m_bg, m_bg2;
	Player m_player;
	int m_speed = 5; //In-Class Initialization, not normal.
	int timer = 0;
	vector<Bullet*> m_bullets;
	vector<Enemy*> m_enemy;
	vector<enBullet*> m_eBullet;
	//Effects
	Mix_Chunk* m_PShot;
	Mix_Chunk* m_EShot;
	Mix_Chunk* m_Explosion;
	//Music track
	Mix_Music* m_Bgm;

};
#endif
