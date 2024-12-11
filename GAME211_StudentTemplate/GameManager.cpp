#include "GameManager.h"
#include "Scene1.h"

MemoryPool* GameManager::npcMemoryPool = nullptr;

GameManager::GameManager() {
	windowPtr = nullptr;
	timer = nullptr;
	isRunning = true;
	currentScene = nullptr;
    player = nullptr;
}

bool GameManager::OnCreate() {
    // Define screen dimensions
    const int SCREEN_WIDTH = 1000;
    const int SCREEN_HEIGHT = 600;

    // Initialize Window
    windowPtr = new Window(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (windowPtr == nullptr) {
        std::cerr << "Failed to create Window instance." << std::endl;
        OnDestroy();
        return false;
    }
    if (windowPtr->OnCreate() == false) {


        std::cerr << "Window::OnCreate() failed." << std::endl;
        OnDestroy();
        return false;


    }

    // Initialize Timer
    timer = new Timer();
    if (timer == nullptr) {
        std::cerr << "Failed to create Timer instance." << std::endl;
        OnDestroy();
        return false;
    }

    // Initialize MemoryPool
    npcMemoryPool = new MemoryPool(sizeof(PlayerBody), 100); // Pool size set to 100
    if (npcMemoryPool == nullptr) {
        std::cerr << "Failed to create MemoryPool instance." << std::endl;
        OnDestroy();
        return false;
    }
    PlayerBody::setMemoryPool(npcMemoryPool);

    // Create Scene1 Instance
    currentScene = new Scene1(windowPtr->GetSDL_Window(), this);
    if (currentScene == nullptr) {
        std::cerr << "Failed to create Scene1 instance." << std::endl;
        OnDestroy();
        return false;
    }

    // Initialize Scene1
    if (!ValidateCurrentScene()) {
        std::cerr << "Scene1 validation failed." << std::endl;
        OnDestroy();
        return false;
    }

    // Create PlayerBody instance
    float mass = 1.0f;
    float radius = 0.5f;
    float orientation = 0.0f;
    float rotation = 0.0f;
    float angular = 0.0f;
    Vec3 position(0.5f * currentScene->getxAxis(), 0.5f * currentScene->getyAxis(), 0.0f);
    Vec3 velocity(0.0f, 0.0f, 0.0f);
    Vec3 acceleration(0.0f, 0.0f, 0.0f);

    try {
        player = new PlayerBody(
            position,
            velocity,
            acceleration,
            mass,
            radius,
            orientation,
            rotation,
            angular,
            this
        );
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation for PlayerBody failed: " << e.what() << std::endl;
        OnDestroy();
        return false;
    }

    if (player == nullptr) {
        std::cerr << "Failed to create PlayerBody instance." << std::endl;
        OnDestroy();
        return false;
    }

    if (player->OnCreate() == false) {
        std::cerr << "PlayerBody::OnCreate() failed." << std::endl;
        OnDestroy();
        return false;
    }

    return true;
}


/// Here's the whole game loop
void GameManager::Run() {
    
	timer->Start();
    
	while (isRunning) {
        
        handleEvents();
		timer->UpdateFrameTicks();
        currentScene->Update(timer->GetDeltaTime());
		currentScene->Render();

		/// Keep the event loop running at a proper rate
		SDL_Delay(timer->GetSleepTime(60)); ///60 frames per sec
	}
}

void GameManager::handleEvents() 
{
    SDL_Event event;

    // Let's add mouse movement and position
    // https://wiki.libsdl.org/SDL_GetMouseState

    //SDL_PumpEvents();  // make sure we have the latest mouse state.

    //https://www.youtube.com/watch?v=SYrRMr4BaD4&list=PLM7LHX-clszBIGsrh7_3B2Pi74AhMpKhj&index=3

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                isRunning = false;
                break;
            case SDL_SCANCODE_Q:
                isRunning = false;
                break;
            case SDL_SCANCODE_DELETE:
                isRunning = false;
                break;
            case SDL_SCANCODE_1:
                LoadScene(1);
                break;
            default:
                break;
            }
        }
        currentScene->HandleEvents(event);
    }
}

GameManager::~GameManager() {}

void GameManager::OnDestroy() {

    npcMemoryPool->reportMemoryUsage();
    if (player) {
        delete player;
        player = nullptr;
    }
    if (currentScene) {
        currentScene->OnDestroy();
        delete currentScene;
        currentScene = nullptr;
    }
    if (timer) {
        delete timer;
        timer = nullptr;
    }
    if (npcMemoryPool) {
        delete npcMemoryPool;
        npcMemoryPool = nullptr;
    }
    if (windowPtr) {
        delete windowPtr;
        windowPtr = nullptr;
    }
}

// This might be unfamiliar
float GameManager::getSceneHeight() { return currentScene->getyAxis(); }

// This might be unfamiliar
float GameManager::getSceneWidth() { return currentScene->getxAxis(); }

// This might be unfamiliar
Matrix4 GameManager::getProjectionMatrix()
{ return currentScene->getProjectionMatrix(); }

// This might be unfamiliar
SDL_Renderer* GameManager::getRenderer()
{
    // [TODO] might be missing some SDL error checking
    SDL_Window* window = currentScene->getWindow();
    SDL_Renderer* renderer = SDL_GetRenderer(window);
    return renderer;
}

// This might be unfamiliar
void GameManager::RenderPlayer(float scale)
{
    player->Render(scale);
}

void GameManager::LoadScene( int i )
{
    // cleanup of current scene before loading another one
    if (currentScene) {
        currentScene->OnDestroy();
        delete currentScene;
        currentScene = nullptr;
    }

    if (player) {
        delete player;
        player = nullptr;
    }

    switch ( i )
    {
        case 1:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this);
            break;
        default:
            currentScene = new Scene1( windowPtr->GetSDL_Window(), this );
            break;
    }

    // using ValidateCurrentScene() to safely run OnCreate
    if (!ValidateCurrentScene())
    {
        std::cerr << "Failed to validate the new scene." << std::endl;
        isRunning = false;
    }
}

bool GameManager::ValidateCurrentScene()
{
    if (currentScene == nullptr) {
        std::cerr << "Current scene is nullptr." << std::endl;
        return false;
    }
    if (currentScene->OnCreate() == false) {
        std::cerr << "Scene1::OnCreate() failed." << std::endl;
        return false;
    }
    return true;
}
