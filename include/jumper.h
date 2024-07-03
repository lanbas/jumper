#pragma once

#include "base.h"
#include "utils.h"

static constexpr const uint32_t kWindowHeight = 720;
static constexpr const uint32_t kWindowWidth = 1280;
static constexpr const uint32_t kFloorHeight = 550; // Origin top left


class Jumper : public Object2D, public IDrawable
{
public:
    void jump(); 
    void duck();
};

class Obstacle : public Object2D, public IDrawable
{
    static Obstacle spawn();
};

class DuckObstacle : public Obstacle
{

};

class JumpObstacle : public Obstacle
{

};