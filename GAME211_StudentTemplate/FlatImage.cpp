#include "FlatImage.h"
#include "Vector.h"

FlatImage::FlatImage(const string& fileName, Scene* owner_) {
	scene = owner_;

	image = IMG_Load(fileName.c_str());
}


FlatImage::~FlatImage() {

}


bool FlatImage::OnCreate() {
	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	texture = SDL_CreateTextureFromSurface(renderer, image);

	if (image == nullptr) {
		std::cerr << "Can't open image" << std::endl;
	}

	return true;
}

void FlatImage::Render(float scale, Vec3 pos) {
	int		w, h;
	Vec3 screenCoords;

	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	screenCoords = scene->getProjectionMatrix() * pos;
	w = static_cast<int>(image->w * scale);
	h = static_cast<int>(image->h * scale);

	square.x = static_cast<int>(screenCoords.x - 0.5f * w);
	square.y = static_cast<int>(screenCoords.y - 0.5f * h);
	square.w = w;
	square.h = h;

	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);


	//con from rads to degrees

	float orientationDegrees = 0.0f;

	SDL_RenderCopyEx(renderer, texture, nullptr, &square, orientationDegrees, nullptr, SDL_FLIP_NONE);
}