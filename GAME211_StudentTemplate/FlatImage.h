#ifndef FLATIMAGE_H
#define FLATIMAGE_H

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include "Scene.h"

using namespace std;
using namespace MATH;



class FlatImage
{
private:
	SDL_Surface* image;
	SDL_Texture* texture;
	SDL_Rect square;
	Scene* scene;

public:
	FlatImage(const string& fileName, Scene* owner_);
	~FlatImage();
	bool OnCreate();
	void Render(float scale = 1.0f, Vec3 pos = Vec3());
};

#endif // !FLATIMAGE_H