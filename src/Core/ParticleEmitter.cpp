#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"
#include "Utils/Math.hpp"


ParticleEmitter::ParticleEmitter():
    m_particleSystem(nullptr),
    m_looping(false),
    m_timeToLive(5),
    m_particleCount(100),
    m_startColor(sf::Color::White),
    m_endColor(sf::Color(0, 0, 0, 0)),
    m_angle(0),
    m_angleVariation(math::PI * 2),
    m_speed(100),
    m_speedVariation(50),
    // Default particle are just 1 pixel
    m_textureRect(0, 0, 1, 1)
{
}


ParticleEmitter::~ParticleEmitter()
{
    clearParticles();
}


void ParticleEmitter::setParticleSystem(ParticleSystem& particleSystem)
{
    m_particleSystem = &particleSystem;
}


void ParticleEmitter::setLooping(bool looping)
{
    m_looping = looping;
}


void ParticleEmitter::setTimeToLive(float duration)
{
    m_timeToLive = duration;
}


void ParticleEmitter::setParticleColor(const sf::Color& color)
{
    m_startColor = color;
}


void ParticleEmitter::setParticleColor(const sf::Color& start, const sf::Color& end)
{
    m_startColor = start;
    m_endColor = end;
}


void ParticleEmitter::setAngle(float angle, float variation)
{
    m_angle = angle;
    m_angleVariation = variation;
}


void ParticleEmitter::setSpeed(float speed, float variation)
{
    m_speed = speed;
    m_speedVariation = variation;
}


void ParticleEmitter::createParticles() const
{
    if (m_particleSystem != nullptr)
    {
        m_particleSystem->create(*this);
    }
}


void ParticleEmitter::clearParticles() const
{
    if (m_particleSystem != nullptr)
    {
        m_particleSystem->removeByEmitter(*this);
    }
}


void ParticleEmitter::setParticleCount(int count)
{
    m_particleCount = count;
}


int ParticleEmitter::getParticleCount() const
{
    return m_particleCount;
}


void ParticleEmitter::setSpawnArea(const sf::FloatRect& rect)
{
    m_spawnArea = rect;
}


void ParticleEmitter::setSpawnPosition(const sf::Vector2f& position)
{
    m_spawnArea = {position.x, position.y, 0, 0};
}


sf::Vector2f ParticleEmitter::getSpawnPosition() const
{
    return sf::Vector2f(
        math::rand(m_spawnArea.left, m_spawnArea.left + m_spawnArea.width),
        math::rand(m_spawnArea.top, m_spawnArea.top + m_spawnArea.height));
}


float ParticleEmitter::getParticleAngle() const
{
    return math::rand(m_angle - m_angleVariation, m_angle + m_angleVariation);
}


float ParticleEmitter::getParticleSpeed() const
{
    return math::rand(m_speed - m_speedVariation, m_speed + m_speedVariation);
}


void ParticleEmitter::setTextureRect(const sf::IntRect& rect)
{
    m_textureRect = rect;
}


const sf::IntRect& ParticleEmitter::getTextureRect() const
{
    return m_textureRect;
}
