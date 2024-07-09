#pragma once

#include "base.h"
#include "utils.h"

// Window info
static constexpr const uint32_t kWindowHeight = 720;
static constexpr const uint32_t kWindowWidth = 1280;

// Jumper size and position
static constexpr const uint32_t kJumperHeight = 125;
static constexpr const uint32_t kDuckHeight = kJumperHeight / 2; 
static constexpr const uint32_t kJumperWidth = 65; 
static constexpr const float kJumperHomeX = 100.0f;
static constexpr const float kJumperHomeY = 550.0f;

// Other movement/position info
static constexpr const uint32_t kFloorHeight = 550 + kJumperHeight; // Origin top left
static constexpr const float kGravity = 0.005f; // Constant acceleration applied to vertical velocity component
static constexpr const float kJumpVelocityY = -1.75f; // Velocity y-component provided by jump

// Obstacle information
static constexpr const uint32_t kSteadyStateTimeElapsed_s = 120;
static constexpr const uint32_t kDuckObstacleHeight = 30;
static constexpr const float kDuckObstacleProportion = 0.3;
static constexpr const float kInitialObstacleVelocity = -0.8;
static constexpr const float kObstacleVelocityIncrement = -0.01; // 120 seconds to reach max speed of -2
static constexpr const float kInitialObstacleWidthFactorMin = 2.0;
static constexpr const float kInitialObstacleWidthFactorMax = 4.0;
static constexpr const float kWidthFactorIncrement = 0.008; // ~120 seconds to reach max width distribution of [1.5, 2.0]
static constexpr const uint32_t kObstacleUnitWidth_px = 65;
static constexpr const uint32_t kInitialObstacleSpawnInterval_ms = 1500;
static constexpr const int32_t kObstacleSpawnIntervalIncrement_ms = -4;
static constexpr const int32_t kObstacleSpawnIntervalDeviation_ms = 400;



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
    Obstacle(const float positionY, const Vector2D& velocity, const uint32_t height, const uint32_t width);
    
    virtual void updateMotion(float dt) override;
    virtual void draw(SDL_Renderer* renderer) override;

private:
    SDL_Rect m_rect{};
};
