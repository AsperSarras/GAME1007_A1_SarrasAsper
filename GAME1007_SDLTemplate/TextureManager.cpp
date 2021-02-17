#include "TextureManager.h"
#include "Engine.h"
#include <iostream>

void TextureManager::Init()
{
	//Initialize SDL Images subsystem.
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
		std::cout << "Image init success" << std::endl;
	else //if 0
		std::cout << "Image init failed: ERROR" << IMG_GetError() << std::endl;
}

void TextureManager::Load(const char* path, const std::string key)
{
	SDL_Texture* temp = IMG_LoadTexture(Engine::Instance().getRenderer(), path);
	if (temp == nullptr)
		std::cout << "Could not load Texture: ERROR" << IMG_GetError() << std::endl;
	else
	{
		s_textures.emplace(key, temp);//Add to the map
		std::cout << "Successfully load '" << key << "' !" << std::endl;
	}
}

void TextureManager::Unload(const std::string key) 
{
	if (s_textures.find(key) != s_textures.end())
	{
		SDL_DestroyTexture(s_textures[key]); //Deallocates texture 
		s_textures.erase(key); //Remove Texture pointer from map
	}
	else
		std::cout << "Could not unload '" << key << "' Key not found!" << std::endl;
}

SDL_Texture* TextureManager::GetTexture(const std::string key)
{
	return s_textures[key];
}

void TextureManager::Quit()
{
	for (auto const& i : s_textures)
		Unload(i.first);
		//	SDL_DestroyTexture(s_textures[i.first]);
	//s_textures.clear();
}

TextureManager::TextureManager() {}

std::map< std::string, SDL_Texture*> TextureManager::s_textures;
