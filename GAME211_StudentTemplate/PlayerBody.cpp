// PlayerBody.cpp
#include "PlayerBody.h"
#include "MemoryPool.h"
#include <iostream>

// Initialize static member
MemoryPool* PlayerBody::memory = nullptr;

// For controller dead zone
static const int JOYSTICK_DEAD_ZONE = 8000;


// Memory management implementations
void* PlayerBody::operator new(size_t size) {
    if (!memory) {
        throw std::bad_alloc();
    }
    void* ptr = memory->allocate();
    return ptr;
}

void PlayerBody::operator delete(void* ptr) {
    if (!memory || !ptr) return;
    memory->deallocate(ptr);
}

void PlayerBody::setMemoryPool(MemoryPool* pool) {
    memory = pool;
}

bool PlayerBody::OnCreate()
{
    // Load the image
    image = IMG_Load("Pacman.png");
    if (image == nullptr) {
        std::cerr << "Failed to load Pacman.png: " << IMG_GetError() << std::endl;
        return false;
    }

    // Create texture from surface
    renderer = game->getRenderer();
    if (renderer == nullptr) {
        std::cerr << "Renderer is nullptr." << std::endl;
        SDL_FreeSurface(image);
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == nullptr) {
        std::cerr << "Failed to create texture from Pacman.png: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(image);
        return false;
    }

    return true;
}

void PlayerBody::Render(float scale)
{
    if (!texture) return;

    // Convert the position from game coords to screen coords.
    Vec3 screenCoords = game->getProjectionMatrix() * pos;

    // Scale the image
    float w = image->w * scale;
    float h = image->h * scale;

    // Define the rectangle for rendering
    SDL_Rect destRect;
    destRect.x = static_cast<int>(screenCoords.x - 0.5f * w);
    destRect.y = static_cast<int>(screenCoords.y - 0.5f * h);
    destRect.w = static_cast<int>(w);
    destRect.h = static_cast<int>(h);

    // Convert orientation from radians to degrees
    float orientationDegrees = orientation * 180.0f / M_PI;

    // Render the texture with rotation
    SDL_RenderCopyEx(renderer, texture, nullptr, &destRect,
        orientationDegrees, nullptr, SDL_FLIP_NONE);
}

void PlayerBody::HandleEvents(const SDL_Event& event)
{
    float maxSpeed = 2.0f;
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
            vel.y = maxSpeed * 1.0f;
            break;
        case SDL_SCANCODE_A:
            vel.x = maxSpeed * -1.0f;
            break;
        case SDL_SCANCODE_S:
            vel.y = maxSpeed * -1.0f;
            break;
        case SDL_SCANCODE_D:
            vel.x = maxSpeed * 1.0f;
            break;
        }
    }
    // When key released, stop moving
    if (event.type == SDL_KEYUP)
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_W:
            vel.y = 0.0f;
            break;
        case SDL_SCANCODE_A:
            vel.x = 0.0f;
            break;
        case SDL_SCANCODE_S:
            vel.y = 0.0f;
            break;
        case SDL_SCANCODE_D:
            vel.x = 0.0f;
            break;
        }
    }

    // Handle controller input
    if (event.type == SDL_CONTROLLERAXISMOTION)
    {
        // If left stick moves horizontally
        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
            if (event.caxis.value > JOYSTICK_DEAD_ZONE) {
                // Stick moved right
                vel.x = maxSpeed;
            }
            else if (event.caxis.value < -JOYSTICK_DEAD_ZONE) {
                // Stick moved left
                vel.x = -maxSpeed;
            }
            else {
                // Stick is near center, stop horizontal movement
                vel.x = 0.0f;
            }
        }

        // If left stick moves vertically
        if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
            if (event.caxis.value > JOYSTICK_DEAD_ZONE) {
                // Stick moved down
                vel.y = -maxSpeed;
            }
            else if (event.caxis.value < -JOYSTICK_DEAD_ZONE) {
                // Stick moved up
                vel.y = maxSpeed;
            }
            else {
                // Stick is near center, stop vertical movement
                vel.y = 0.0f;
            }
        }
    }

    // Normalize velocity
    if (VMath::mag(vel) > VERY_SMALL)
    {
        vel = VMath::normalize(vel) * maxSpeed;
    }
}

void PlayerBody::Update(float deltaTime)
{
    // Update position by calling the base class update
    Body::Update(deltaTime);

    // Apply a force (gravity-like effect)
    Body::ApplyForce(Vec3(0.0f, -force, 0.0f));

    // Increment force (could be part of some effect)
    force += 0.1f;
}
