#include "Button.h"
#include "Vector.h"

Button::Button(const string& fileName, Scene* owner_, Vec3 pos_, float scale_) {
	scene = owner_; 

	image = IMG_Load(fileName.c_str());

	pos = pos_;

	scale = scale_;
}


Button::~Button() {

}


bool Button::OnCreate() {
	SDL_Renderer* renderer;
	SDL_Window* window = scene->getWindow();
	renderer = SDL_GetRenderer(window);

	texture = SDL_CreateTextureFromSurface(renderer, image);
	
	if (image == nullptr) {
		std::cerr << "Can't open image" << std::endl;
	}
	
	return true;
}

void Button::Render() {
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

void Button::HandleEvents(const SDL_Event& event)
{
    // player clicked on the button
    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
		if (event.button.button == SDL_BUTTON_LEFT) {
			int x, y;
			Uint32 buttons = SDL_GetMouseState(&x, &y);

			Vec3 screenCoords = scene->getProjectionMatrix() * pos;

			bool withinX, withinY;

			withinX = x < screenCoords.x + image->w * 0.5 * scale && x > screenCoords.x - image->w * 0.5 * scale;
			withinY = y < screenCoords.y + image->h * 0.5 * scale && y > screenCoords.y - image->h * 0.5 * scale;

			if (withinX && withinY) {
				clicked = true;
			}
		}
    }

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_LEFT) {
			int x, y;
			Uint32 buttons = SDL_GetMouseState(&x, &y);

			Vec3 screenCoords = scene->getProjectionMatrix() * pos;

			bool withinX, withinY;

			// check the button press was within borders;
			withinX = x < screenCoords.x + image->w * 0.5 * scale && x > screenCoords.x - image->w * 0.5 * scale;
			withinY = y < screenCoords.y + image->h * 0.5 * scale && y > screenCoords.y - image->h * 0.5 * scale;

			if (withinX && withinY) {
				if (clicked == true) {
					activated = true;
				}
			}
			else {
				clicked = false;
			}
		}
	}
}