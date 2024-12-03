#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H
#include <SDL.h>
#include <iostream>
#include "Window.h"
#include "Timer.h"
#include "Scene.h"
#include "PlayerBody.h"
#include "MemoryPool.h"


class GameManager {
private:
	/// These are called "forward declarations" The idea is that a pointer is 
	/// really just an unsigned int, so to create a pointer to it you really only
	/// need to tell the compiler that there is a class called Window and I need
	/// a pointer to it, thus the "class Window*" means trust me, there is a class 
	/// called Window, I just need a pointer to it.

	/// If that was too much for your brain, just #include "Window.h" and declare
	/// Window *ptr and don't use the word "class"  This is a second semester C++
	/// topic anyway
	class Window *windowPtr;
	class Timer *timer;
	bool isRunning;
	class Scene *currentScene;
	static MemoryPool* npcMemoryPool;
	// This might be unfamiliar
    class PlayerBody *player;
	Uint32 changeSceneEventType;
	Uint32 winSceneEventType;
	Uint32 MenuSceneEventType;

	void handleEvents();
	void LoadScene(int i);
	bool ValidateCurrentScene();
	SDL_Texture* walkAnim;

public:
	GameManager();
	~GameManager();
	bool OnCreate();
	void OnDestroy();


	// These might be unfamiliar
	float getSceneHeight();
	float getSceneWidth();
	Matrix4 getProjectionMatrix();
    PlayerBody* getPlayer(){ return player; }
	void RenderPlayer(float scale = 1.0f);
	SDL_Renderer* getRenderer();

	Uint32 getChangeScene() { return changeSceneEventType; };
	Uint32 getWinScene() { return winSceneEventType; }
	Uint32 getMenuScene() { return MenuSceneEventType; }

	void Run();
    
};
#endif


