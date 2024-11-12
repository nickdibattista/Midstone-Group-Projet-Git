#include "Scene1.h"
#include <VMath.h>
#include "Button.h"

// See notes about this constructor in Scene1.h.
Scene1::Scene1(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	walkAnim = NULL;
}

Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	start = new Button("Clyde.png", this);
	if (!start->OnCreate()) {
		return false;
	}

	//SDL_Surface* image;
	image = IMG_Load("Sprites/MikeyMountaintopWalk.png");
	walkAnim = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	//TODO error checking

	

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {
	// update screen position
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);

	float left, right, bottom, top;

	left = game->getPlayer()->getPos().x - xAxis / 2.0f;
	right = game->getPlayer()->getPos().x + xAxis / 2.0f;
	bottom = game->getPlayer()->getPos().y - yAxis / 2.0f;
	top = game->getPlayer()->getPos().y + yAxis / 2.0f;

	/*Matrix4 ortho = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;*/
	std::cout << left << " " << bottom << std::endl;

	// Update player
	game->getPlayer()->Update(deltaTime);

	
}

void Scene1::Render() {
	// render the player
	// game->RenderPlayer(0.10f);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//render Button
	//start->Render();

	// frame rate of animation
	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100) % 4;

	SDL_Rect srcrect = { sprite * 48, 0, 32, 64 };

	Vec3 screenCoords = projectionMatrix * game->getPlayer()->getPos();
	SDL_Rect dscrect = {screenCoords.x , screenCoords.y, 64, 128};

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}
