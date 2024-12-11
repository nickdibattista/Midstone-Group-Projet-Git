#ifndef SCENEGUI_H
#define SCENEGUI_H


#include <MMath.h>
#include "SceneGUI.h"
#include "Button.h"
#include "PlayerBody.h"
#include "AudioClass.h"


using namespace MATH;
class SceneGUI : public Scene {
private:

	SDL_GameController* gController = NULL;

	float xAxis;	// scene width, in game coords, set in constructor
	float yAxis;	// scene height, in game coords, set in constructor
	SDL_Window* window;		// an SDL window with a SDL renderer
	SDL_Renderer* renderer;	// the renderer associated with SDL window
	Matrix4 projectionMatrix;	// set in OnCreate()
    Matrix4     inverseProjection;	// set in OnCreate()
	Button *start;
	
	AudioClass* audio;
	std::shared_ptr<AudioClass> audio_ss;

public:
	// This constructor may be different from what you've seen before
	// Notice the second parameter, and look in GameManager.cpp
	// to see how this constructor is called.



	SceneGUI(SDL_Window* sdlWindow, GameManager* game_);
	~SceneGUI();
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
	Vec3 getMousePosition();
};

#endif