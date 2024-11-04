#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <iostream>
#include <SDL_image.h>
#include <string>
#include "Scene.h"

using namespace std;
using namespace MATH;



class Button
{
private:
	SDL_Surface* image;
	SDL_Texture* texture;
	SDL_Rect square; 
	Scene* scene;
	Vec3 position;
	Vec3 topLeft;
	Vec3 bottomRight;

public:
	Button(const string &fileName, Vec3 postion_, Scene* owner_);
	~Button();
	bool OnCreate();
	void Render();
	bool clicked(Vec3 mousePos);
};

#endif // !BUTTON_H