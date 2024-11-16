#include "SceneStart.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneStart::SceneStart(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
}

SceneStart::~SceneStart() {
}

bool SceneStart::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// load logo
	Logo = new FlatImage("Sprites/Logo.png", this);
	if (!Logo->OnCreate()) {
		std::cerr << "no Logo" << std::endl;
		return false;
	}

	// load background
	Background = new FlatImage("Sprites/StartSceneBg.png", this);
	if (!Background->OnCreate()) {
		std::cerr << "no start background" << std::endl;
		return false;
	}

	// load start button texture
	start = new Button("Sprites/StartButton.png", this);
	if (!start->OnCreate()) {
		std::cerr << "no start button" << std::endl;
		return false;
	}

	// load exit button texture
	exit = new Button("Sprites/ExitButton.png", this);
	if (!exit->OnCreate()) {
		std::cerr << "no exit button" << std::endl;
		return false;
	}

	return true;
}

void SceneStart::OnDestroy() {}

void SceneStart::Update(const float deltaTime) {

}

void SceneStart::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	float scale;
	Vec3 pos;

	// display background
	Background->Render(scale = 6.0f, pos = Vec3(12.5f, 7.5f, 0.0f));
	
	// display logo
	Logo->Render(scale = 2.0f, pos = Vec3(12.5f, 11.0f, 0.0f));

	// display buttons
	start->Render(scale = 2.0f, pos = Vec3(12.5f, 6.0f, 0.0f));
	exit->Render(scale = 2.0f, pos = Vec3(12.5f, 4.0f, 0.0f));

	SDL_RenderPresent(renderer);
}

void SceneStart::HandleEvents(const SDL_Event& event)
{


}
