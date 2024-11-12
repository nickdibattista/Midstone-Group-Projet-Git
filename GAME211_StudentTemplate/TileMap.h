#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL.h>
#include "Scene.h"

using namespace MATH;

class TileMap 
{
private:
	SDL_Surface* tileMapSurface;
	SDL_Texture* tileTexture;
	Scene* scene;

public:
	TileMap(Scene* owner);
	~TileMap();
	bool OnCreate();
	void Render();
};
#endif // !TILEMAP_H