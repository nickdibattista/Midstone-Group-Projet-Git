//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"


bool PlayerBody::OnCreate()
{
    image = IMG_Load("Sprites/MikeyMountaintopWalk.png");
    SDL_Renderer *renderer = game->getRenderer();
    walkAnim = SDL_CreateTextureFromSurface( renderer, image );
    if (image == nullptr) {
        std::cerr << "Can't open the player sprite" << std::endl;
        return false;
    }
    

    jumpSoundEffect = Mix_LoadWAV("Sounds/JumpEffectWav.wav");
    Mix_VolumeChunk(jumpSoundEffect, MIX_MAX_VOLUME);
    if (!jumpSoundEffect) {
        std::cerr << "Failed to load jump sound effect: " << Mix_GetError() << std::endl;
    }
    else {
        std::cout << "Jump sound effect loaded successfully!" << std::endl;
    }
    return true;
}

void PlayerBody::AudioCleanup()
{
    if (jumpSoundEffect) {
       Mix_FreeChunk(jumpSoundEffect);
       jumpSoundEffect = nullptr;        
    }
}



void PlayerBody::Render( float scale )
{
    int size = image->h;
    int side = size * scale;

    // This is why we need game in the constructor, to get the renderer, etc.
    SDL_Renderer *renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    // square represents the position and dimensions for where to draw the image
    Vec3 screenCoords;

    // convert the position from game coords to screen coords.
    screenCoords = projectionMatrix * pos;

    // frame rate of animation
    Uint32 ticks = SDL_GetTicks();
    Uint32 sprite = (ticks / 100) % 4;

    // cut animation, place it in world, resize it.
    // srcret is based on the image file. The one for the walking animation
    SDL_Rect srcrect = { sprite * size, 0, size, size };
    screenCoords = projectionMatrix * getPos();
    SDL_Rect dscrect = { screenCoords.x - 0.5f * side, screenCoords.y - 0.5f * side, side, side};

    SDL_RenderCopy(renderer, walkAnim, &srcrect, &dscrect);
}

void PlayerBody::HandleEvents( const SDL_Event& event )
{
    // player presses down key
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) 
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_A:
            vel.x = -movSpeed;
            break;
        case SDL_SCANCODE_D:
            vel.x = movSpeed;
            break;
        case SDL_SCANCODE_W:
            vel.y = movSpeed;
            break;
        case SDL_SCANCODE_S:
            vel.y = -movSpeed;
            break;
        case SDL_SCANCODE_R:
            movSpeed = 10.0f;
            break;
        case SDL_SCANCODE_SPACE:
            if (grounded) 
            {
                vel.y = movSpeed * 2.0f;
                grounded = false; 
                if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
                if (Mix_PlayChannel(-1, jumpSoundEffect, 0) == -1) {
                    std::cout << "error playing sound: " << Mix_GetError() << std::endl;
                }
                std::cout << "try to jump" << std::endl;
            }
            
            break;
        }
    }

    // if player releases key 
    if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_A:
            vel.x = 0.0f;
            break;
        case SDL_SCANCODE_D:
            vel.x = 0.0f;
            break;
        case SDL_SCANCODE_W:
            vel.y = 0.0f;
            break;
        case SDL_SCANCODE_S:
            vel.y = -0.0f;
            break;
        case SDL_SCANCODE_R:
            movSpeed = 4.0f;
            break;
        }
    }
}

void PlayerBody::Update( float deltaTime )
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    Body::Update( deltaTime );

}

void PlayerBody::ApplyForce(Vec3 force) {
    Body::ApplyForce(force);
}

