#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include <SFML/Graphics.hpp>

class ParticleSystem;

class ParticleEmitter
{
friend class ParticleSystem;
public:
    ParticleEmitter();
    ~ParticleEmitter();

    void setParticleSystem(ParticleSystem& particleSystem);

    void setTimeToLive(float duration);
    void setLooping(bool looping);

    void setParticleColor(const sf::Color& color);
    void setParticleColor(const sf::Color& start, const sf::Color& end);

    void setAngle(float angle, float variation = 0.f);
    void setSpeed(float speed, float variation = 0.f);

    // Create particles linked to the emitter in the particle system
    void createParticles() const;

    /**
     * Delete all the particle linked to the emitter.
     * This is called automatically upon emitter destruction.
     */
    void clearParticles() const;

    /**
     * Define number of particles created by createParticles.
     * Default is 100.
     */
    void setParticleCount(int count);
    int getParticleCount() const;

    void setSpawnPosition(const sf::Vector2f& position);
    void setSpawnArea(const sf::FloatRect& rect);
    sf::Vector2f getSpawnPosition() const;

    float getParticleAngle() const;
    float getParticleSpeed() const;

    void setTextureRect(const sf::IntRect& rect);
    const sf::IntRect& getTextureRect() const;

private:
    ParticleSystem* m_particleSystem;
    bool         m_looping;
    float        m_timeToLive;
    sf::FloatRect m_spawnArea;
    int          m_particleCount;
    sf::Color    m_startColor;
    sf::Color    m_endColor;
    float        m_angle;
    float        m_angleVariation;
    float        m_speed;
    float        m_speedVariation;
    sf::IntRect  m_textureRect;
};

#endif
