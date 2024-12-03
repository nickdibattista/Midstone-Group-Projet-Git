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
	gravity = Vec3(0.0f, -20.0f, 0.0f);
}

Scene1::~Scene1(){
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);

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

	progressBar = new FlatImage("Sprites/ProgressBar.png", this, scale, pos = Vec3());
	if (!progressBar->OnCreate()) {
		std::cout << "no progress bar" << std::endl;
		return false;
	}

	playerIcon = new FlatImage("Sprites/PlayerIcon.png", this, scale, pos = Vec3());
	if (!playerIcon->OnCreate()) {
		std::cout << "no player icon" << std::endl;
		return false;
	}

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	doCollisions();

	// apply gravity
	if (!game->getPlayer()->getGrounded()) {
		game->getPlayer()->ApplyForce(gravity);
	}

	// update player
	game->getPlayer()->Update(deltaTime);

	//  ------------- update screen position -----------------------
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);

	float left, right, bottom, top;

	left = game->getPlayer()->getPos().x - xAxis * 0.5f;
	right = game->getPlayer()->getPos().x + xAxis * 0.5f;
	bottom = game->getPlayer()->getPos().y - yAxis * 0.5f;
	top = game->getPlayer()->getPos().y + yAxis * 0.5f;

	Matrix4 ortho = MMath::orthographic(left, right, bottom, top, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	//  ------------- update screen position -----------------------

	// update UI
	progressBar->SetPos(Vec3(left + xAxis / 2.0f, top - 1.0f, 0.0f));
	
	// player icon is more delicate
	// the start is 10.5f
	// the end is 14.5f
	// player icon must move 4 units
	// formula is (player.pos.x - player.intialPos.x) / (lastPlat.x - firstPlat.x) * 4
	float PlayerIconMovement;
	PlayerIconMovement = game->getPlayer()->getPos().x - 12.5f;
	PlayerIconMovement /= (platformArray.back()->GetPos().x - platformArray[0]->GetPos().x);
	PlayerIconMovement *= 4.0f;
	playerIcon->SetPos(Vec3(left + 10.5f + PlayerIconMovement, top - 1.4f, 0.0f));
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

	progressBar->Render();
	playerIcon->Render();

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

	// TODO find why the inverse projection isn't working

	Vec3 playerSize = Vec3(player.getPixels(scale) * 25.0f / 1000.0f, player.getPixels(scale) * 15.0f / 600.0f, 0.0f);
	Vec3 platformSize = Vec3(platform.GetImageSizeX() * 25.0f / 1000.0f, platform.GetImageSizeY() * 15.0f / 600.0f, 0.0f);


	// compare the borders on x for player and platform (player right side against platform left & the opposite)
	collisionX = player.getPos().x + playerSize.x * 0.5f >= platform.GetPos().x - platformSize.x * 0.5f &&
		platform.GetPos().x + platformSize.x * 0.5f >= player.getPos().x - playerSize.x * 0.5f;
	// compare the borders on y for player and platform (player top side against platform bottom & the opposite)
	collisionY = player.getPos().y + playerSize.y * 0.5f >= platform.GetPos().y - platformSize.y * 0.5f &&
		platform.GetPos().y + platformSize.y * 0.5f >= player.getPos().y - playerSize.y * 0.5f;

	return collisionX && collisionY;
}

void Scene1::doCollisions() {
	// check collisions with every platform
	bool collision = false;
	for (FlatImage* platform : platformArray)
	{
		if (checkCollision(*game->getPlayer(), *platform)) {
			CollisionType(*game->getPlayer(), *platform);
			collision = true;
		}
	}
	if (!collision) {
		game->getPlayer()->setGrounded(false);
	}
}

void Scene1::CollisionType(PlayerBody& player, FlatImage& platform) {
	Vec3 playerSize = Vec3(player.getPixels(scale) * 25.0f / 1000.0f, player.getPixels(scale) * 15.0f / 600.0f, 0.0f);
	Vec3 platformSize = Vec3(platform.GetImageSizeX() * 25.0f / 1000.0f, platform.GetImageSizeY() * 15.0f / 600.0f, 0.0f);
	
	// player is either right or left of the platform
	if (player.getPos().x + playerSize.x * 0.4f <= platform.GetPos().x - platformSize.x * 0.5f || 
		player.getPos().x - playerSize.x * 0.4f >= platform.GetPos().x + platformSize.x * 0.5f) {
		// check player is not on top or under the platform
		if (player.getPos().y + playerSize.y * 0.4f >= platform.GetPos().y - platformSize.y * 0.5f &&
			player.getPos().y - playerSize.y * 0.4f <= platform.GetPos().y + platformSize.y * 0.5f) {
			// case 1: player is at the right
			if (player.getPos().x < platform.GetPos().x) {
				player.setPos(Vec3(platform.GetPos().x - platformSize.x * 0.5f - playerSize.x * 0.5f, player.getPos().y, 0.0f));
				if (player.getVel().x > 0) {
					player.setVel(Vec3(0.0f, player.getVel().y, 0.0f));
				}
			}
			// case 2: player is at the left
			else {
				player.setPos(Vec3(platform.GetPos().x + platformSize.x * 0.5f + playerSize.x * 0.5f, player.getPos().y, 0.0f));
				if (player.getVel().x < 0) {
					player.setVel(Vec3(0.0f, player.getVel().y, 0.0f));
				}
			}
		}
	}
	// within the platform's left and right limits
	else {
		// case 1: player is above platform
		if (player.getPos().y > platform.GetPos().y) {
			player.setPos(Vec3(player.getPos().x, platform.GetPos().y + platformSize.y * 0.5f + playerSize.y * 0.5f, 0.0f));
			if (player.getVel().y < 0) {
				player.setVel(Vec3(player.getVel().x, 0.0f, 0.0f));
				player.setAcc(Vec3());
				player.setGrounded(true);
			}
		}
		// case2: player is underneath platform
		else {
			player.setPos(Vec3(player.getPos().x, platform.GetPos().y - platformSize.y * 0.5f - playerSize.y * 0.5f, 0.0f));
			if (player.getVel().y > 0) {
				player.setVel(Vec3(player.getVel().x, 0.0f, 0.0f));
			}
		}
	}
}