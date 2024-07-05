#include "utils.h"

///////////////////////////////
// Object2D : public Drawable
///////////////////////////////
Object2D::Object2D(const Vector2D& initialPosition, const Vector2D& initialVelocity, const Vector2D& initialAcceleration, const uint32_t height, const uint32_t width)
    :m_position(initialPosition), m_velocity(initialVelocity), m_acceleration(initialAcceleration), m_height(height), m_width(width)
{}

Vector2D Object2D::position()
{
    return m_position;
}

Vector2D Object2D::velocity()
{
    return m_velocity;
}

Vector2D Object2D::acceleration()
{
    return m_acceleration;
}

uint32_t Object2D::height()
{
    return m_height;
}

uint32_t Object2D::width()
{
    return m_width;
}

uint32_t Object2D::left()
{
    return m_position.x - (m_width / 2);
}

uint32_t Object2D::right()
{
    return m_position.x + (m_width / 2);
}

uint32_t Object2D::top()
{
    return m_position.y - (m_height / 2);
}

uint32_t Object2D::bottom()
{
    return m_position.y + (m_height / 2);
}

void Object2D::updateMotion(float dt)
{
    // Kinematic updates to position and velocity
    m_velocity += m_acceleration * dt;
    m_position = m_position + m_velocity * dt + (m_acceleration / 2.0f) * pow(dt, 2);
}

bool Object2D::isColliding(std::vector<Object2D> objectList)
{
    for (Object2D& obj : objectList)
    {
        if (areColliding(*this, obj))
            return true;
    }

    return false;
}

///////////////////////////////
// Score : public Drawable
///////////////////////////////
Score::Score(int64_t initialScore)
    :m_score(initialScore)
{}

void Score::setScore(int64_t value)
{
    m_score = value;
}

int64_t Score::getScore()
{
    return m_score;
}

void Score::addScore(int64_t value)
{
    m_score += value;
}

void Score::subtractScore(int64_t value)
{
    m_score -= value;
}

void Score::draw(SDL_Renderer* renderer)
{
    return;
}