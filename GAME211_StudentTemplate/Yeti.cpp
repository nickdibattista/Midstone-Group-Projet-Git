#include "Yeti.h"

bool Yeti::OnCreate()
{
    SDL_Renderer* renderer = game->getRenderer();

	// load walking texture
	image = IMG_Load("Sprites/YetiPounce.png");
	yetiPounce = SDL_CreateTextureFromSurface(renderer, image);
	if (image == nullptr) {
		std::cerr << "Can't open the yeti pounce sprite" << std::endl;
		return false;
	}

	vel.x = movSpeed;

    return true;
}

void Yeti::Render(float scale)
{
    if (active) {
		int w, h;
		Vec3 screenCoords;

		SDL_Renderer* renderer = game->getRenderer();
		Matrix4 projectionMatrix = game->getProjectionMatrix();

		screenCoords = projectionMatrix * pos;

		w = static_cast<int>(image->w * scale);
		h = static_cast<int>(image->h * scale);

		SDL_Rect square;

		square.x = static_cast<int>(screenCoords.x - 0.5f * w);
		square.y = static_cast<int>(screenCoords.y - 0.5f * h);
		square.w = w;
		square.h = h;

		SDL_QueryTexture(yetiPounce, nullptr, nullptr, &w, &h);


		//con from rads to degrees

		float orientationDegrees = 0.0f;

		SDL_RenderCopyEx(renderer, yetiPounce, nullptr, &square, orientationDegrees, nullptr, SDL_FLIP_NONE);
    }
}

void Yeti::HandleEvents(const SDL_Event& event)
{

}

void Yeti::Update(float deltaTime)
{
	if (active) {
		Body::Update(deltaTime);
	}
}

void Yeti::ApplyForce(Vec3 force) {
    Body::ApplyForce(force);
}

void Yeti::Activate() {
    active = true;
	vel.x = movSpeed;
    //play yeti roar
}

void Yeti::Pounce(Vec3 pos_) {
	pouncing = true;
	pos = pos_ + Vec3(0.0f, 5.0f, 0.0f);
	vel = (pos - pos_) / 2.0f;
}