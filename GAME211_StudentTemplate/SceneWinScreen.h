#ifndef SCENEWINSCREEN_H
#define SCENEWINSCREEN_H


#include <MMath.h>
#include "Scene.h"
#include "Button.h"
#include "FlatImage.h"

using namespace MATH;
class SceneWinScreen : public Scene {
private:
	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
	Matrix4 inverseProjection;	// set in OnCreate()
	FlatImage* Logo; //set in OnCreate()
	FlatImage* Background; //set in OnCreate()
	Button* menuReturn; //set in OnCreate()
	Button* exit; //set in OnCreate()

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.
	SceneWinScreen(SDL_Window* sdlWindow, GameManager* game_);
	~SceneWinScreen();
	bool OnCreate();
	void OnDestroy();
	void Update(const float time);
	void Render();
	void HandleEvents(const SDL_Event& event);
	float getxAxis() { return xAxis; }
	float getyAxis() { return yAxis; }
	SDL_Window* getWindow() { return window; }
	Matrix4 getProjectionMatrix() { return projectionMatrix; }
	Matrix4 getInverseMatrix() { return inverseProjection; }
};

#endif
