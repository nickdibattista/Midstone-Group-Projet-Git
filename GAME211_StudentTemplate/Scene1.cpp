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
	if (start) {
		delete start;
		start = nullptr;
	}
}

bool Scene1::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);

	/// Turn on the SDL imaging subsystem
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}

	// Initialize Button
	start = new Button("Clyde.png", Vec3(10.0f, 8.0f, 0.0f), this);
	if (!start->OnCreate()) {
		std::cerr << "Failed to create Button instance." << std::endl;
		return false;
	}

	return true;
}


void Scene1::OnDestroy() {
	// Report Memory Usage if needed
	// MemoryPool is managed by GameManager

	// Clean up Button
	if (start) {
		delete start;
		start = nullptr;
	}

	// Quit SDL_image
	IMG_Quit();
}

void Scene1::Update(const float deltaTime) {

	// Update player
	if (game->getPlayer()) {
		game->getPlayer()->Update(deltaTime);
	}
}

void Scene1::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Opaque black background
	SDL_RenderClear(renderer);

	// Render Button
	if (start) {
		start->Render();
	}

	// Render the player
	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void Scene1::HandleEvents(const SDL_Event& event)
{
	// Send events to player as needed
	if (game->getPlayer()) {
		game->getPlayer()->HandleEvents(event);
	}

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