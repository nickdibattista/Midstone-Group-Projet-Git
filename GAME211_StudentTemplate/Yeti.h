#ifndef YETI_H
#define YETI_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"

class Yeti : public Body
{
protected:
    class GameManager* game;
    SDL_Surface* image;
    SDL_Texture* yetiPounce;
    bool active = false;
    bool pouncing = false;
    float movSpeed = 3.0f;

public:

    Yeti() : Body{}
    {
        game = nullptr;
    }

    // Note the last parameter in this constructor!
    // Look in GameManager.cpp to see how this is called.
    // Look in .cpp file, at Render(), to see why game_ is a parameter.
    Yeti(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        GameManager* game_
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
    void Render(float scale = 1.0f);
    void HandleEvents(const SDL_Event& event);
    void Update(float deltaTime);
    void ApplyForce(Vec3 force);
    void setTexture(SDL_Texture* texture_) { texture = texture_; }
    void Activate();
    bool getActive() { return active; }
    void Pounce(Vec3 pos);
    bool getPouncing() { return pouncing; }
};

#endif // !PLAYERBODY_H 
