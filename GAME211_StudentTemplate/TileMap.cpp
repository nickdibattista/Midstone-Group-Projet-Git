#include "TileMap.h"

TileMap::TileMap(Scene* owner_) 
{
	scene = owner_;
}

TileMap::~TileMap() 
{

}

bool TileMap::OnCreate() 
{
	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	tileMapSurface = IMG_Load("Sprites/TileSet.png");
	tileTexture = SDL_CreateTextureFromSurface(renderer, tileMapSurface);
	SDL_FreeSurface(tileMapSurface);
	return true;
}

void TileMap::Render() 
{
	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	int tilemap[20][15];
	// assign a tile to each space
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			tilemap[i][j] = rand() % 9 + 1;
		}
	}

	SDL_Rect tile[20][15];
	// populate screen with tiles
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			tile[i][j].x = i * 16;
			tile[i][j].y = j * 16;
			tile[i][j].w = 16;
			tile[i][j].h = 16;
		}
	}

	SDL_Rect tile_1 = { 16, 16, 16, 16 };
	SDL_Rect tile_2 = { 32, 16, 16, 16 };
	SDL_Rect tile_3 = { 48, 16, 16, 16 };
	SDL_Rect tile_4 = { 16, 32, 16, 16 };
	SDL_Rect tile_5 = { 32, 32, 16, 16 };
	SDL_Rect tile_6 = { 48, 32, 16, 16 };
	SDL_Rect tile_7 = { 16, 48, 16, 16 };
	SDL_Rect tile_8 = { 32, 48, 16, 16 };
	SDL_Rect tile_9 = { 48, 48, 16, 16 };

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 15; j++) {
			switch (tilemap[i][j]) 
			{
			case 1:
				SDL_RenderCopy(renderer, tileTexture, &tile_1, &tile[i][j]);
				break;
			case 2:
				SDL_RenderCopy(renderer, tileTexture, &tile_2, &tile[i][j]);
				break;
			case 3:
				SDL_RenderCopy(renderer, tileTexture, &tile_3, &tile[i][j]);
				break;
			case 4:
				SDL_RenderCopy(renderer, tileTexture, &tile_4, &tile[i][j]);
				break;
			case 5:
				SDL_RenderCopy(renderer, tileTexture, &tile_5, &tile[i][j]);
				break;
			case 6:
				SDL_RenderCopy(renderer, tileTexture, &tile_6, &tile[i][j]);
				break;
			case 7:
				SDL_RenderCopy(renderer, tileTexture, &tile_7, &tile[i][j]);
				break;
			case 8:
				SDL_RenderCopy(renderer, tileTexture, &tile_8, &tile[i][j]);
				break;
			case 9:
				SDL_RenderCopy(renderer, tileTexture, &tile_9, &tile[i][j]);
				break;
			}
		}
	}
}

