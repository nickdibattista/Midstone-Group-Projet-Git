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
	//In-Game Background Music
	if (SDL_Init(SDL_INIT_AUDIO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		return -1;
	}

	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
		std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << "\n";
		return -1;
	}
	Mix_Music* backgroundMusic = Mix_LoadMUS("Sounds/backgroundMusic.mp3");
	if (!backgroundMusic) {
		std::cout << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << "\n";
		return -1;
	}

	//set music volume
	Mix_VolumeMusic(32);
	
	// -1 means loop music indefinitely
	Mix_PlayMusic(backgroundMusic, -1);  


	//Viewport
	int w, h;
	SDL_GetWindowSize(window,&w,&h);
	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);


	// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);


	//Load Background Image
	SDL_Surface* backgroundSurface = IMG_Load("Sprites/SnowBackground.png");
	if (!backgroundSurface) {
		std::cout << "Failed to load background image! SDL_image Error: " << IMG_GetError << "\n";
		return -1;
	}
	sceneAssets.backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
	SDL_FreeSurface(backgroundSurface);


	// populate this array with all the platforms
	Vec3 pos;
	platformArray.push_back(new FlatImage("Sprites/Platform1.png", this, false, scale, pos = Vec3(12.0f, 4.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(20.0f, 2.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(24.0f, -1.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(28.0f, -5.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform3.png", this, false, scale, pos = Vec3(36.0f, -7.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform3.png", this, false, scale, pos = Vec3(42.0f, -7.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(48.0f, -10.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(52.0f, -13.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(48.0f, -16.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(52.0f, -19.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(48.0f, -22.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(52.0f, -25.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform1.png", this, false, scale, pos = Vec3(60.0f, -29.0f, 0.0f)));
	//Nick's Starting point for adding platforms
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(70.0f, -35.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(75.0f, -34.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(79.0f, -34.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(83.0f, -34.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(88.0f, -38.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(91.0f, -37.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(94.0f, -38.0f, 0.0f)));
	//Increase Difficulty
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(100.0f, -40.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(101.0f, -45.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(102.0f, -50.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(106.0f, -40.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform2.png", this, false, scale, pos = Vec3(106.0f, -44.0f, 0.0f)));
	//Stairs Jump
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(108.0f, -50.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(112.0f, -49.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(116.0f, -48.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Platform4.png", this, false, scale, pos = Vec3(121.0f, -51.0f, 0.0f)));
	platformArray.push_back(new FlatImage("Sprites/Board.png", this, false, scale, pos = Vec3(129.0f, -55.0f, 0.0f)));
	//Ending Platform
	platformArray.push_back(new FlatImage("Sprites/Platform3.png", this, true, scale, pos = Vec3(137.0f, -55.0f, 0.0f)));
	platformArray.back()->SetIsEndPlatform(true);

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

void Scene1::OnDestroy() {
	
		if (backgroundMusic) {
			Mix_FreeMusic(backgroundMusic);
			backgroundMusic = nullptr;
		}
	
}

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
	if (!game->getPlayer()->getGrounded()) {
		game->getPlayer()->ApplyForce(gravity);
		// std::cout << "apply grav" << std::endl;
	}
	

	//std::cout << game->getPlayer()->getVel().y << std::endl;

	game->getPlayer()->Update(deltaTime);

	// print player pos
	// std::cout << game->getPlayer()->getPos().x << " " << game->getPlayer()->getPos().y << std::endl;
}

void Scene1::Render() {
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//render background image
	if (sceneAssets.backgroundTexture) {
		SDL_RenderCopy(renderer, sceneAssets.backgroundTexture, NULL, NULL);
	}
	else {
		std::cout << "background texture is null!\n";
	}

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
	bool reachedEnd = false;
	for (FlatImage* platform : platformArray)
	{
		if (checkCollision(*game->getPlayer(), *platform)) {
			CollisionType(*game->getPlayer(), *platform);
			collision = true;
			if (platform->GetIsEndPlatform()) {
				reachedEnd = true;
			}
		}
	}
	if (!collision) {
		game->getPlayer()->setGrounded(false);
	}
	if (reachedEnd) {
		SDL_Event gameWon;
		SDL_memset(&gameWon, 0, sizeof(gameWon));
		gameWon.type = game->getWinScene();
		gameWon.user.code = 1;
		gameWon.user.data1 = nullptr;
		gameWon.user.data2 = nullptr;
		SDL_PushEvent(&gameWon);
	}
}

void Scene1::CollisionType(PlayerBody& player, FlatImage& platform) {
	Vec3 playerSize = Vec3(player.getPixels(scale) * 25.0f / 1000.0f, player.getPixels(scale) * 15.0f / 600.0f, 0.0f);
	Vec3 platformSize = Vec3(platform.GetImageSizeX() * 25.0f / 1000.0f, platform.GetImageSizeY() * 15.0f / 600.0f, 0.0f);
	
	// on the sides
	if (player.getPos().x + playerSize.x * 0.4f <= platform.GetPos().x - platformSize.x * 0.5f || 
		player.getPos().x - playerSize.x * 0.4f >= platform.GetPos().x + platformSize.x * 0.5f) {
		if (player.getPos().y + playerSize.y * 0.4f >= platform.GetPos().y - platformSize.y * 0.5f &&
			player.getPos().y - playerSize.y * 0.4f <= platform.GetPos().y + platformSize.y * 0.5f) {
			// from the right
			if (player.getPos().x < platform.GetPos().x) {
				player.setPos(Vec3(platform.GetPos().x - platformSize.x * 0.5f - playerSize.x * 0.5f, player.getPos().y, 0.0f));
				if (player.getVel().x > 0) {
					player.setVel(Vec3(0.0f, player.getVel().y, 0.0f));
				}
			}
			// from the left
			else {
				player.setPos(Vec3(platform.GetPos().x + platformSize.x * 0.5f + playerSize.x * 0.5f, player.getPos().y, 0.0f));
				if (player.getVel().x < 0) {
					player.setVel(Vec3(0.0f, player.getVel().y, 0.0f));
				}
			}
		}
	}
	// within the platform
	else {
		// from above
		if (player.getPos().y > platform.GetPos().y) {
			player.setPos(Vec3(player.getPos().x, platform.GetPos().y + platformSize.y * 0.5f + playerSize.y * 0.5f, 0.0f));
			if (player.getVel().y < 0) {
				player.setVel(Vec3(player.getVel().x, 0.0f, 0.0f));
				player.setAcc(Vec3());
				player.setGrounded(true);
			}
		}
		// from underneath
		else {
			player.setPos(Vec3(player.getPos().x, platform.GetPos().y - platformSize.y * 0.5f - playerSize.y * 0.5f, 0.0f));
			if (player.getVel().y > 0) {
				player.setVel(Vec3(player.getVel().x, 0.0f, 0.0f));
			}
		}
	}
}