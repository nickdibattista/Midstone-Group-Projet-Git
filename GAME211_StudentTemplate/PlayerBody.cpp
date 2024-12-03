//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"


bool PlayerBody::OnCreate()
{
    SDL_Renderer* renderer = game->getRenderer();

    // load walking texture
    image = IMG_Load("Sprites/MikeyMountaintopWalk.png");
    walkTexture = SDL_CreateTextureFromSurface( renderer, image );
    if (image == nullptr) {
        std::cerr << "Can't open the player walking sprite" << std::endl;
        return false;
    }
<<<<<<< HEAD
    

    jumpSoundEffect = Mix_LoadWAV("Sounds/JumpEffectWav.wav");
    Mix_VolumeChunk(jumpSoundEffect, MIX_MAX_VOLUME);
    if (!jumpSoundEffect) {
        std::cerr << "Failed to load jump sound effect: " << Mix_GetError() << std::endl;
    }
    else {
        std::cout << "Jump sound effect loaded successfully!" << std::endl;
    }
=======

    // load idle texture
    image = IMG_Load("Sprites/MikeyMountaintopIdle.png");
    idleTexture = SDL_CreateTextureFromSurface(renderer, image);
    if (image == nullptr) {
        std::cerr << "Can't open the player idle sprite" << std::endl;
        return false;
    }

    // 
    image = IMG_Load("Sprites/MikeyMountaintopJump.png");
    jumpTexture = SDL_CreateTextureFromSurface(renderer, image);
    if (image == nullptr) {
        std::cerr << "Can't open the player jump sprite" << std::endl;
        return false;
    }

>>>>>>> origin/Miguel
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

    // render the appropiate animation
    if (activeAnim == walkAnim) {
        Uint32 ticks = SDL_GetTicks();
        Uint32 sprite = (ticks / 100) % 4;

        // cut animation, place it in world, resize it.
        // srcret is based on the image file. The one for the walking animation
        SDL_Rect srcrect = { sprite * size, 0, size, size };
        screenCoords = projectionMatrix * getPos();
        SDL_Rect dscrect = { screenCoords.x - 0.5f * side, screenCoords.y - 0.5f * side, side, side };

        if (facingRight) {
            SDL_RenderCopyEx(renderer, walkTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopyEx(renderer, walkTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_HORIZONTAL);
        }
    }
    if (activeAnim == idleAnim) {
        SDL_Rect srcrect = { 0, 0, size, size };
        screenCoords = projectionMatrix * getPos();
        SDL_Rect dscrect = { screenCoords.x - 0.5f * side, screenCoords.y - 0.5f * side, side, side };
        if (facingRight) {
            SDL_RenderCopyEx(renderer, idleTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopyEx(renderer, idleTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_HORIZONTAL);
        }
    }
    if (activeAnim == jumpAnim) {
        SDL_Rect srcrect = { 0, 0, size, size };
        screenCoords = projectionMatrix * getPos();
        SDL_Rect dscrect = { screenCoords.x - 0.5f * side, screenCoords.y - 0.5f * side, side, side };
        if (facingRight) {
            SDL_RenderCopyEx(renderer, jumpTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_NONE);
        }
        else {
            SDL_RenderCopyEx(renderer, jumpTexture, &srcrect, &dscrect, 0.0f, nullptr, SDL_FLIP_HORIZONTAL);
        }
    }

}

void PlayerBody::HandleEvents( const SDL_Event& event )
{
    // player presses down key
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0) 
    {
        switch (event.key.keysym.scancode)
        {
        case SDL_SCANCODE_A:
            movHor = -1;
            break;
        case SDL_SCANCODE_D:
            movHor = 1;
            break;
        case SDL_SCANCODE_SPACE:
            if (grounded) 
            {
                vel.y = movSpeed * 2.0f;
<<<<<<< HEAD
                grounded = false; 
                if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    std::cerr << "SDL_Mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                }
                if (Mix_PlayChannel(-1, jumpSoundEffect, 0) == -1) {
                    std::cout << "error playing sound: " << Mix_GetError() << std::endl;
                }
=======
                grounded = false;
>>>>>>> origin/Miguel
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
            movHor = 0;
            break;
        case SDL_SCANCODE_D:
            movHor = 0;
            break;
        }
    }
}

void PlayerBody::Update( float deltaTime )
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    Body::Update( deltaTime );

    vel.x = movSpeed * movHor;

    // animation tree !!! (hurray...)
    switch (activeAnim) 
    {
    case walkAnim:
        if (!grounded) {
            activeAnim = jumpAnim;
        }
        else if (vel.x == 0 && grounded) {
            activeAnim = idleAnim;
        }
        break;
    case idleAnim:
        if (!grounded) {
            activeAnim = jumpAnim;
        }
        else if (vel.x != 0 && grounded) {
            activeAnim = walkAnim;
        }
        break;
    case jumpAnim:
        if (vel.y == 0 && vel.x == 0) {
            activeAnim = idleAnim;
        }
        else if (vel.y == 0 && vel.x != 0) {
            activeAnim = walkAnim;
        }
        break;
    }

    if (vel.x < 0) {
        facingRight = false;
    }
    else if (vel.x > 0) {
        facingRight = true;
    }
}

void PlayerBody::ApplyForce(Vec3 force) {
    Body::ApplyForce(force);
}

