#include "Button.h"
#include "Vector.h"

Button::Button(const string &fileName, Vec3 position_, Scene* owner_) {
	scene = owner_; 
	image = IMG_Load(fileName.c_str()); 
	square = { 0,0,0,0 };
	texture = NULL;
	position = position_;
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

	//Vec3 pos = Vec3(10.0f, 8.0f, 0.0f);
	float scale = 1.0f;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	screenCoords = scene->getProjectionMatrix() * position;
	w = static_cast<int>(image->w * scale);
	h = static_cast<int>(image->h * scale);
	square.x = static_cast<int>(screenCoords.x);
	square.y = static_cast<int>(screenCoords.y);
	square.w = w;
	square.h = h;
	
	topLeft = position;
	bottomRight = scene->getInverseMatrix() * Vec3(static_cast<float>(square.x + w), static_cast<float>(square.y + h), 1.0f);

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

bool Button::clicked(Vec3 mousePos)
{

	//return true;

	return (
		topLeft.x < mousePos.x && mousePos.x < bottomRight.x &&
		bottomRight.y < mousePos.y < topLeft.y
		);
}


