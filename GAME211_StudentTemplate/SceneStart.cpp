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
	int scale;
	Vec3 pos;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;

	/// Turn on the SDL imaging subsystem
	IMG_Init(IMG_INIT_PNG);

	// load logo
	Logo = new FlatImage("Sprites/Logo.png", this, false, scale = 2.0f, pos = Vec3(12.5f, 11.0f, 0.0f));
	if (!Logo->OnCreate()) {
		std::cerr << "no Logo" << std::endl;
		return false;
	}

	// load background
	// TODO make a better background logo
	Background = new FlatImage("Sprites/StartSceneBg.png", this, false, scale = 6.0f, pos = Vec3(12.5f, 7.5f, 0.0f));
	if (!Background->OnCreate()) {
		std::cerr << "no start background" << std::endl;
		return false;
	}

	// load start button texture
	// TODO improve the button textures, they get lost with the bg ;-;
	start = new Button("Sprites/StartButton.png", this, pos = Vec3(12.5f, 6.0f, 0.0f), scale = 2.0f);
	if (!start->OnCreate()) {
		std::cerr << "no start button" << std::endl;
		return false;
	}

	// load exit button texture
	exit = new Button("Sprites/ExitButton.png", this, pos = Vec3(12.5f, 4.0f, 0.0f), scale = 2.0f);
	if (!exit->OnCreate()) {
		std::cerr << "no exit button" << std::endl;
		return false;
	}

	return true;
}

void SceneStart::OnDestroy() {}

void SceneStart::Update(const float deltaTime) {
	// check buttons

	if (start->GetActivated()) {
		// do event
		SDL_Event event;
		SDL_memset(&event, 0, sizeof(event));
		event.type = game->getChangeScene();
		event.user.code = 1;
		event.user.data1 = nullptr;
		event.user.data2 = nullptr;
		SDL_PushEvent(&event);
	}

	if (exit->GetActivated()) {
		// do event
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}

void SceneStart::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	float scale;
	Vec3 pos;

	// display background
	Background->Render();
	
	// display logo
	Logo->Render();

	// display buttons
	start->Render();
	exit->Render();

	SDL_RenderPresent(renderer);
}

void SceneStart::HandleEvents(const SDL_Event& event)
{
	start->HandleEvents(event);
	exit->HandleEvents(event);
}
