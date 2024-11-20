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

	// populate this array with all the platforms
	platformArray.push_back(new FlatImage("Sprites/Platform1.png", this, scale, pos = Vec3(12.0f, 4.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, scale, pos = Vec3(20.0f, 2.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, scale, pos = Vec3(24.0f, -1.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, scale, pos = Vec3(28.0f, -5.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform3.png", this, scale, pos = Vec3(36.0f, -7.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform3.png", this, scale, pos = Vec3(42.0f, -7.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(48.0f, -10.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(52.0f, -13.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(48.0f, -16.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(52.0f, -19.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(48.0f, -22.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, scale, pos = Vec3(52.0f, -25.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform1.png", this, scale, pos = Vec3(60.0f, -29.0f, 0.0f)));

	// check everything in the array was done properly :p
	for (FlatImage* platform : platformArray)
	{
		if (!platform->OnCreate()) {
			std::cout << "no platform" << std::endl;
			return false;
		}
	}

	// this array will be generated procedurally based on the player's position
	// fill that function on update if(player->pos().x + something > skyarray.top()->pos.x) add another one (?)
	// otherwise just fill it in like the first one .-.
	/*skyArray.push_back(new FlatImage("Sprites/SnowBackground.png", this, scale, pos = Vec3(12.0f, 8.0f, 0.0f)));
	for (FlatImage* sky : skyArray)
	{
		if (!sky->OnCreate()) {
			std::cout << "no platform" << std::endl;
			return false;
		}
	}*/

	// any other arrays for missellaneous decorations or clouds (clouds might have to go on a separate one
	// so they can move)

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
	/*if (game->getPlayer()->getGrounded() == false) {
		game->getPlayer()->ApplyForce(gravity);
		std::cout << "apply grav" << std::endl;
	}*/

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


	for (FlatImage* platform : platformArray)
	{
		platform->Render();
	}

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

	// TBD simplify Xratio and Y ratio with inverse projection (not really sure how to use it)
	// also, might need some fine tunning
	

	// compare the borders on x for player and platform (player right side against platform left & the opposite)
	collisionX = player.getPos().x + player.getPixels() * Xratio * 0.5f >= platform.GetPos().x - platform.GetImageSizeX() * Xratio * 0.5f &&
		platform.GetPos().x + platform.GetImageSizeX() * Xratio * 0.5f >= player.getPos().x - player.getPixels() * Xratio * 0.5f;
	// compare the borders on y for player and platform (player top side against platform bottom & the opposite)
	collisionY = player.getPos().y + player.getPixels() * Yratio * 0.6f >= platform.GetPos().y - platform.GetImageSizeY() * Yratio * 0.6f &&
		platform.GetPos().y + platform.GetImageSizeY() * Yratio * 0.6f >= player.getPos().y - player.getPixels() * Yratio * 0.6f;

	return collisionX && collisionY;
}

void Scene1::doCollisions() {
	// check collisions with every platform
	for (FlatImage* platform : platformArray)
	{
		if (checkCollision(*game->getPlayer(), *platform)) {
			std::cout << "collision" << std::endl;
		}
	}
}