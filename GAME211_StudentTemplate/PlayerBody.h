//
//  PlayerBody.h
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"

class PlayerBody : public Body
{
protected:
    class GameManager* game;
    SDL_Texture* walkAnim;
    float pixels = 16.0f;
    float grounded = false;
    float movSpeed = 4.0f;

public:

    PlayerBody() : Body{}
    {
        game = nullptr;
    }

    // Note the last parameter in this constructor!
    // Look in GameManager.cpp to see how this is called.
    // Look in .cpp file, at Render(), to see why game_ is a parameter.
    PlayerBody(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        GameManager *game_
    ) : Body{
          pos_
        , vel_
        , accel_
        , mass_
        , radius_
        , orientation_
        , rotation_
        , angular_
    }
        , game{ game_ }
    {}
    
    // use the base class versions of getters

    bool OnCreate();
    void Render( float scale = 1.0f );
    void HandleEvents( const SDL_Event& event );
    void Update( float deltaTime );
    void ApplyForce(Vec3 force);
    void setTexture( SDL_Texture* texture_ ) { texture = texture_; }
    float getPixels(float scale) { return pixels * scale;  }
    void setGrounded(bool grounded_) { grounded = grounded_; }
    bool getGrounded() { return grounded; }
};

#endif // !PLAYERBODY_H 
