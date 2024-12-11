#include "GameManager.h"
#include "Scene1.h"
#include "SceneGUI.h"



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




    // Initialize ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); //(void)io;
    ImGui::StyleColorsDark(); // or ImGui::StyleColorsClassic(); etc.

    // Initialize ImGui for SDL and for SDL_Renderer
    // Assuming windowPtr->GetSDL_Window() and a valid SDL_Renderer* renderer
    SDL_Window* sdlWindow = windowPtr->GetSDL_Window();
    SDL_Renderer* sdlRenderer = SDL_GetRenderer(sdlWindow);

    // Initialize ImGui for SDL2 and SDL_Renderer
    ImGui_ImplSDL2_InitForSDLRenderer(sdlWindow, sdlRenderer);
    ImGui_ImplSDLRenderer2_Init(sdlRenderer);
    io.Fonts->AddFontFromFileTTF("C:/GameDev/OCRA.ttf", 18.0f);





    ImGui::StyleColorsDark();
    // After setting a base style, you can further tweak individual values
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;     // Make windows have rounded corners
    style.FrameRounding = 4.0f;      // Rounded frame corners
    style.GrabRounding = 4.0f;       // Rounded grab corners
    style.ScrollbarRounding = 6.0f;  // Rounded scrollbar corners

    // Customize the colors of elements
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.29f, 0.48f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.20f, 0.29f, 0.38f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.40f, 0.54f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.40f, 0.55f, 1.00f);


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

    while (SDL_PollEvent(&event)) {



        // Pass event to ImGui
        ImGui_ImplSDL2_ProcessEvent(&event);




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
            case SDL_SCANCODE_2:
                LoadScene(2);
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


    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
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
            cout << "changed scene" << endl;
            break;

        case 2:
            currentScene = new SceneGUI( windowPtr->GetSDL_Window(), this);
            cout << "changed scene 22222222222222222 SCENEGUI" << endl;
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
