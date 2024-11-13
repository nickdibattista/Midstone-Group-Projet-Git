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

	// Set player image to PacMan

	SDL_Surface* image;
	SDL_Texture* texture;

	image = IMG_Load("pacman.png");
	texture = SDL_CreateTextureFromSurface(renderer, image);
	game->getPlayer()->setImage(image);
	game->getPlayer()->setTexture(texture);

	start = new Button("Clyde.png", Vec3(10.0f, 8.0f, 0.0f), this);
	if (!start->OnCreate()) {
		return false;
	}

	return true;
}

void Scene1::OnDestroy() {}

void Scene1::Update(const float deltaTime) {

	// Update player
	game->getPlayer()->Update(deltaTime);
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	//render Button
	start->Render();

	// render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
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

Vec3 Scene1::getMousePosition()
{

	Uint32 buttons;
	int x, y;
	buttons = SDL_GetMouseState(&x, &y);
	Vec3 mouseScreenCoords = Vec3(float(x), float(y), 0.0f);
	Vec3 mouseWorldCoords = inverseProjection * (mouseScreenCoords);
	return mouseWorldCoords;
}