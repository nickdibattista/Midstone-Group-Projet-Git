#include "SceneStart.h"
#include <VMath.h>

// See notes about this constructor in Scene1.h.
SceneStart::SceneStart(SDL_Window* sdlWindow_, GameManager* game_) {
	window = sdlWindow_;
	game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
	Logo = NULL;
	Background = NULL;
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

	// load logo texture
	SDL_Surface* image;
	image = IMG_Load("Sprites/Logo.png");
	if (image == nullptr) {
		std::cerr << "Can't open Logo" << std::endl;
		return false;
	}
	Logo = SDL_CreateTextureFromSurface(renderer, image);

	// load background texture
	image = IMG_Load("Sprites/StartSceneBg.png");
	if (image == nullptr) {
		std::cerr << "Can't open background" << std::endl;
		return false;
	}
	Background = SDL_CreateTextureFromSurface(renderer, image);

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

	SDL_FreeSurface(image);
	return true;
}

void SceneStart::OnDestroy() {}

void SceneStart::Update(const float deltaTime) {

}

void SceneStart::Render() {
	// render the player
	// game->RenderPlayer(0.10f);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);

	// display background
	Vec3 screenCords;
	Vec3 pos = Vec3(12.5f, 7.5f, 0.0f);
	SDL_Surface* image = IMG_Load("Sprites/StartSceneBg.png");
	SDL_Rect square;
	float w, h;
	float scale = 6.0f;

	SDL_FreeSurface(image);

	screenCords = projectionMatrix * pos;

	// Scale the image, in case the .png file is too big or small
	w = image->w * scale;
	h = image->h * scale;

	square = { static_cast<int>(screenCords.x - 0.5f * w), 
		static_cast<int>(screenCords.y - 0.5f * h), static_cast<int>(w), static_cast<int>(h) };

	// no rotation
	float orientationDegrees = 0.0f;

	SDL_RenderCopyEx(renderer, Background, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);

	// display logo
	pos = Vec3(12.5f, 11.0f, 0.0f);
	image = IMG_Load("Sprites/Logo.png");
	scale = 2.0f;

	SDL_FreeSurface(image);

	screenCords = projectionMatrix * pos;

	// Scale the image, in case the .png file is too big or small
	w = image->w * scale;
	h = image->h * scale;

	square = { static_cast<int>(screenCords.x - 0.5f * w),
		static_cast<int>(screenCords.y - 0.5f * h), static_cast<int>(w), static_cast<int>(h) };

	SDL_RenderCopyEx(renderer, Logo, nullptr, &square,
		orientationDegrees, nullptr, SDL_FLIP_NONE);

	start->Render();

	SDL_RenderPresent(renderer);
}

void SceneStart::HandleEvents(const SDL_Event& event)
{


}
