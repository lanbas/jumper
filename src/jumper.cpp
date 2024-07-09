#include "jumper.h"

////////////////////////////////
// Jumper : public Object2D, public IDrawable
////////////////////////////////
Jumper::Jumper()
    :Object2D({kJumperHomeX, kJumperHomeY}, {0.0f, 0.0f}, {0.0f, kGravity}, kJumperHeight, kJumperWidth)
{
    m_state = JumperState::RUNNING;
    m_rect.w = kJumperWidth;
    m_rect.h = kJumperHeight;
    m_rect.x = kJumperHomeX;
    m_rect.y = kJumperHomeY;
}

void Jumper::jump()
{
    // TODO: Think about whose responsibility it is to "finish" jump and notice that jumper doesn't go through floor
        // Could make argument for acceleration always being constant (like gravity) and only when velocity is nonzero is it affected

    m_velocity.y = kJumpVelocityY;
    m_state = JumperState::JUMPING;
} 

void Jumper::duck()
{
    // Potential state check to avoid ducking while jumping?? Or do we want to allow this
    m_height = kDuckHeight;
    m_position.y += kJumperHeight - kDuckHeight; // Y origin is from top left, so have to lower to ground after reducing height
    m_state = JumperState::DUCKING;
}

void Jumper::reset()
{
    // Resets back to steady running state
    m_height = kJumperHeight;
    m_velocity.y = 0;
    m_position.y = kJumperHomeY;
    m_state = JumperState::RUNNING;
}

void Jumper::draw(SDL_Renderer* renderer)
{
    m_rect.y = m_position.y;
    m_rect.h = m_height; 

    SDL_RenderFillRect(renderer, &m_rect);
}

JumperState Jumper::state()
{
    return m_state;
}

void Jumper::updateMotion(float dt)
{
    // Jumper-specific update function that adds checks for whether we are on the ground and whether we should 
    // let Object2D::updateMotion actually affect position/velocity
    
    // If jumping, let motion update affect position/velocity
    if (m_state == JumperState::JUMPING)
    {
        Object2D::updateMotion(dt);
    }

    // Check for jump complete
    if (m_state == JumperState::JUMPING && m_position.y >= kJumperHomeY)
    {
        // Sets back to floor height, velocity 0, and state back to running
        reset();
    }
}

Obstacle::Obstacle(const float positionY, const Vector2D& velocity, const uint32_t height, const uint32_t width)
    :Object2D({kWindowWidth, positionY}, velocity, {0.0f, 0.0f}, height, width)
{
    m_rect.y = positionY;
    m_rect.h = height;
    m_rect.w = width;
}
    
void Obstacle::updateMotion(float dt)
{
    m_position = m_position + m_velocity * dt + (m_acceleration / 2.0f) * pow(dt, 2); // Update position based on constant velocity;
}

void Obstacle::draw(SDL_Renderer* renderer)
{
    m_rect.x = m_position.x;
    SDL_RenderFillRect(renderer, &m_rect);
}
