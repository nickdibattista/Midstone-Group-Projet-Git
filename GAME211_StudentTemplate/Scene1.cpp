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
	gravity = Vec3(0.0f, -4.0f, 0.0f);
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

	Vec3 pos;

	plat1 = new FlatImage("Sprites/Platform1.png", this, scale, pos = Vec3(12.5f, 4.0f, 0.0f));
	if (!plat1->OnCreate()) {
		std::cerr << "no Platform 1" << std::endl;
		return false;
	}

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {
	//  ------------- update screen position -----------------------
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);

	float left, right, bottom, top;

	left = game->getPlayer()->getPos().x - xAxis / 2.0f;
	right = game->getPlayer()->getPos().x + xAxis / 2.0f;
	bottom = game->getPlayer()->getPos().y - yAxis / 2.0f;
	top = game->getPlayer()->getPos().y + yAxis / 2.0f;

	Matrix4 ortho = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	//  ------------- update screen position -----------------------

	// Update player

	doCollisions();

	// apply gravity
	if (game->getPlayer()->getGrounded() == false) {
		game->getPlayer()->ApplyForce(gravity);
		std::cout << "apply grav" << std::endl;
	}

	//std::cout << game->getPlayer()->getVel().y << std::endl;

	game->getPlayer()->Update(deltaTime);

	// print player pos
	// std::cout << game->getPlayer()->getPos().x << " " << game->getPlayer()->getPos().y << std::endl;
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);


	// render platform
	Vec3 screenCoords;

	plat1->Render();

	// render player
	game->getPlayer()->Render(scale);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);
}

bool Scene1::checkCollision(PlayerBody &player, FlatImage &platform) 
{
	bool collisionX;
	bool collisionY;
	float Yratio = yAxis / 600.0f;
	float Xratio = xAxis / 1000.0f;
	
	collisionX = player.getPos().x + player.getPixels() * Xratio * 0.6f >= platform.GetPos().x - platform.GetImageSizeX() * Xratio * 0.6f &&
		platform.GetPos().x + platform.GetImageSizeX() * Xratio * 0.6f >= player.getPos().x - player.getPixels() * Xratio * 0.6f;
	collisionY = player.getPos().y + player.getPixels() * Yratio * 0.6f >= platform.GetPos().y - platform.GetImageSizeY() * Yratio * 0.6f &&
		platform.GetPos().y + platform.GetImageSizeY() * Yratio * 0.6f >= player.getPos().y - player.getPixels() * Yratio * 0.6f;

	return collisionX && collisionY;
}

void Scene1::doCollisions() {
	if (checkCollision(*game->getPlayer(), *plat1)) {
		
	}
}