#pragma once
#ifndef _TEXTUREMANAGER_
#define _TEXTUREMANAGER_

#include <map>
#include<string>
#include "SDL.h"
#include "SDL_image.h"

class TextureManager
{
public: //Methods
	static void Init();
	static void Load(const char* path,const std::string key);
	static void Unload(const std::string key);
	static SDL_Texture* GetTexture(const std::string key);
	static void Quit();

private://Properties
	static std::map< std::string, SDL_Texture*> s_textures;

private://Methods
	TextureManager(); 
	
};

#endif

