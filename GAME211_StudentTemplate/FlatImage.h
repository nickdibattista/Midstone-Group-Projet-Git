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
	Vec3 pos;
	float scale;
	bool isEndPlatform = false;

public:
	FlatImage(const string& fileName, Scene* owner_, bool isEnd, float scale_ = 1.0f, Vec3 pos_ = Vec3());
	~FlatImage();
	bool OnCreate();
	void Render();
	Vec3 GetPos() { return pos; }
	float GetImageSizeX() { return image->w * scale; }
	float GetImageSizeY() { return image->h * scale; }
	bool GetIsEndPlatform() const { return isEndPlatform; }
	void SetIsEndPlatform(bool isEnd) { isEndPlatform = isEnd; }
};
#endif // !FLATIMAGE_H