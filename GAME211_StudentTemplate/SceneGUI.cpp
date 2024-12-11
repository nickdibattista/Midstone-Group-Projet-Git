#include "SceneGUI.h"
#include <VMath.h>
#include "Button.h"



// See notes about this constructor in Scene1.h.
SceneGUI::SceneGUI(SDL_Window* sdlWindow_, GameManager* game_){
	window = sdlWindow_;
    game = game_;
	renderer = SDL_GetRenderer(window);
	xAxis = 25.0f;
	yAxis = 15.0f;
}

SceneGUI::~SceneGUI(){
	if (start) {
		delete start;
		start = nullptr;
	}
}

bool SceneGUI::OnCreate() {
	int w, h;
	SDL_GetWindowSize(window, &w, &h);

	Matrix4 ndc = MMath::viewportNDC(w, h);
	Matrix4 ortho = MMath::orthographic(0.0f, xAxis, 0.0f, yAxis, 0.0f, 1.0f);
	projectionMatrix = ndc * ortho;
	inverseProjection = MMath::inverse(projectionMatrix);

	/// Turn on the SDL imaging subsystem
	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
		std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
		return false;
	}
	
	//////////////////////// Initialize the audio system here
	audio = new AudioClass();
	// or if you want to use shared_ptr:
	// audio_ss = std::make_shared<AudioClass>();

	// Initialize Button
	start = new Button("Clyde.png", Vec3(10.0f, 8.0f, 0.0f), this);
	if (!start->OnCreate()) {
		std::cerr << "Failed to create Button instance." << std::endl;
		return false;
	}

	///////////////////////// controller support /////////////////////////////

	gController = nullptr;
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
		std::cerr << "Failed to initialize SDL GameController: " << SDL_GetError() << std::endl;
		return false;
	}

	if (SDL_NumJoysticks() > 0) {
		gController = SDL_GameControllerOpen(0);
		if (gController == nullptr) {
			std::cerr << "Failed to open game controller: " << SDL_GetError() << std::endl;
		}
	}
	////////////////////////////////////////

	return true;
}


void SceneGUI::OnDestroy() {
	// Report Memory Usage if needed
	// MemoryPool is managed by GameManager


	////////// audio KILL
	if (audio) {
		delete audio;
		audio = nullptr;
	}
	///////// controller support END
	if (gController) {
		SDL_GameControllerClose(gController);
		gController = nullptr;
	}


	if (start) {
		delete start;
		start = nullptr;
	}
	IMG_Quit();
}

void SceneGUI::Update(const float deltaTime) {

	// Update player
	if (game->getPlayer()) {
		game->getPlayer()->Update(deltaTime);
	}
}

void SceneGUI::Render() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Opaque black background
	SDL_RenderClear(renderer);




	// Start ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// Example of a more "beautiful" UI layout
	{
		// Create a full screen background window that is transparent
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImVec2((float)1280, (float)720)); // Use your window size
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0, 0, 0, 0.5f)); // Semi-transparent black
		ImGui::Begin("BackgroundWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus);
		ImGui::PopStyleColor();

		// Place a child window for your content
		ImGui::BeginChild("ContentRegion", ImVec2(400, 200), true);
		ImGui::Text("Welcome to the SceneGUI!");
		ImGui::Separator();
		ImGui::Text("Press 'K' to play a sound!");
		// Add a nice button
		if (ImGui::Button("Click Me")) {
			// Do something on click
		}

		ImGui::EndChild();
		ImGui::End();
	}

	// End ImGui frame and render it
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);




	// Render Button
	if (start) {
		start->Render();
	}

	// Render the player
//	game->RenderPlayer(0.10f);

	SDL_RenderPresent(renderer);
}

void SceneGUI::HandleEvents(const SDL_Event& event)
{

	if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_k) {
		if (audio) {
			audio->PlaySound("sound/SKULLTRUMPET.ogg");
		}
	}

	////////// controller initialization 
	if (SDL_NumJoysticks() > 0) {
		gController = SDL_GameControllerOpen(0);
		if (gController == nullptr) {
			cout << "Failed to open game controller: " << SDL_GetError() << endl;
		}
	}

	// Send events to player as needed
	if (game->getPlayer()) {
		game->getPlayer()->HandleEvents(event);
	}

	Vec3 mousePos = getMousePosition();

}

Vec3 SceneGUI::getMousePosition()
{

	Uint32 buttons;
	int x, y;
	buttons = SDL_GetMouseState(&x, &y);
	Vec3 mouseScreenCoords = Vec3(float(x), float(y), 0.0f);
	Vec3 mouseWorldCoords = inverseProjection * (mouseScreenCoords);
	return mouseWorldCoords;
}