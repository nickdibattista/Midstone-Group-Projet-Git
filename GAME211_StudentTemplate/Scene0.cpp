#include "Scene0.h"
#include <VMath.h>
#include "Button.h"

// See notes about this constructor in Scene1.h.
Scene0::Scene0(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	walkAnim = NULL;
}

Scene0::~Scene0(){
}

bool Scene0::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window,&w,&h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// Set player image to PacMan


	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);

	start = new Button("Clyde.png", Vec3(10.0f, 8.0f, 0.0f), this);
	if (!start->OnCreate()) {
		return false;
	}

	image = IMG_Load("MikeyMountaintopWalk.png");
	walkAnim = SDL_CreateTextureFromSurface(renderer, image);
	SDL_FreeSurface(image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(walkAnim);

	

	return true;
}

void Scene0::OnDestroy() {}

void Scene0::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
}

void Scene0::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	Uint32 ticks = SDL_GetTicks();
	Uint32 sprite = (ticks / 100) % 4;

	SDL_Rect srcrect = { sprite * 48, 0, 32, 64 };

	SDL_Rect dscrect = { 10, 10, 32, 64 };
	SDL_RenderCopy(renderer, walkAnim, &srcrect, &dscrect);

	//render Button
	start->Render();

	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void Scene0::HandleEvents(const SDL_Event& event)
{
	// send events to player as needed
	game->getPlayer()->HandleEvents(event);

	Vec3 mousePos = getMousePosition();

	if (event.button.type == SDL_MOUSEBUTTONUP
		&& event.button.button == SDL_BUTTON_LEFT)
	{
		if (start->clicked(mousePos))
		{
			printf("mouse clicked start\n");
		}
	}

}

Vec3 Scene0::getMousePosition()
{

	Uint32 buttons;
	int x, y;
	buttons = SDL_GetMouseState(&x, &y);
	Vec3 mouseScreenCoords = Vec3(float(x), float(y), 0.0f);
	Vec3 mouseWorldCoords = inverseProjection * (mouseScreenCoords);
	return mouseWorldCoords;
}