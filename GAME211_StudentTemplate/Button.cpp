#include "Button.h"
#include "Vector.h"

Button::Button(const string &fileName, Scene* owner_) {
	scene = owner_; 

	image = IMG_Load(fileName.c_str());
}


Button::~Button() {

}


bool Button::OnCreate() {
	int		w, h;
	Vec3 screenCoords;

	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	texture = SDL_CreateTextureFromSurface(renderer, image);
	
	if (image == nullptr) {
		std::cerr << "Can't open image" << std::endl;
	}

	Vec3 pos = Vec3(10.0f, 8.0f, 0.0f);
	float scale = 1.0f;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	screenCoords = scene->getProjectionMatrix() * pos;
	w = static_cast<int>(image->w * scale);
	h = static_cast<int>(image->h * scale);

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = w;
	square.h = h;
	
	return true;
}

void Button::Render() {

	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);


	

	//con from rads to degrees

	float orientationDegrees = 0.0f;

	SDL_RenderCopyEx(renderer, texture, nullptr, &square, orientationDegrees, nullptr, SDL_FLIP_NONE);
}