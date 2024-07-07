#pragma once

#include "base.h"
#include "utils.h"

static constexpr const uint32_t kWindowHeight = 720;
static constexpr const uint32_t kWindowWidth = 1280;
static constexpr const uint32_t kFloorHeight = 550; // Origin top left

static constexpr const uint32_t kJumperHeight = 125;
static constexpr const uint32_t kDuckHeight = kJumperHeight / 2; 
static constexpr const uint32_t kJumperWidth = 65; 
static constexpr const float kJumperHomeX = 100.0f;
static constexpr const float kJumperHomeY = 550.0f;

static constexpr const float kGravity = 0.005f; // Constant acceleration applied to vertical velocity component
static constexpr const float kJumpVelocityY = -1.75f; // Velocity y-component provided by jump


enum class JumperState
{
    RUNNING,
    JUMPING,
    DUCKING
};

class Jumper : public Object2D, public IDrawable
{
public:
    Jumper();
    void jump(); 
    void duck();
    void reset();
    JumperState state();

    virtual void updateMotion(float dt) override;

    void draw(SDL_Renderer* renderer) override;

private:
    JumperState m_state;
    SDL_Rect m_rect{};
};

class Obstacle : public Object2D, public IDrawable
{
public:
    static Obstacle spawn();
    
    virtual void updateMotion(float dt) override;
    void draw(SDL_Renderer* renderer) override;

private:
    SDL_Rect m_rect{};
};

class DuckObstacle : public Obstacle
{

};

class JumpObstacle : public Obstacle
{

};

static spawn_obstacle(Vector2D velocity)
{

}
