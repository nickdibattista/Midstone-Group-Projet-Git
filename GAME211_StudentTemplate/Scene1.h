#ifndef SCENE1_H
#define SCENE1_H


#include <MMath.h>
#include "Scene.h"
#include "Button.h"
#include "TileMap.h"
#include "FlatImage.h"
#include "Yeti.h"
#include <vector>
#include <SDL_mixer.h>

using namespace MATH;
class Scene1 : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4 inverseProjection;	// set in OnCreate()
	Vec3 gravity;
	std::vector<FlatImage*> platformArray; //an array of every platform
	FlatImage* progressBar; // set in OnCreate()
	FlatImage* playerIcon; // set in OnCreate()
	FlatImage* YetiIcon; // appears after event
	float scale = 2.5f;
	Mix_Music* backgroundMusic = nullptr;
	Mix_Chunk* killEffect;
	Mix_Chunk* winEffect;
	Mix_Chunk* yetiGrowlEffect;
	Yeti* yeti; // set in OnCreate()

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	struct SceneAssets { SDL_Texture* backgroundTexture = nullptr; };
	SceneAssets sceneAssets;
	Scene1(SDL_Window* sdlWindow, GameManager* game_);
	~Scene1();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
    void HandleEvents(const SDL_Event &event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
    Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
	bool checkCollision(PlayerBody &player, FlatImage &platform);
	void doCollisions();
	void CollisionType(PlayerBody& player, FlatImage& platform);
};

#endif
