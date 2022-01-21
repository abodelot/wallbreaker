#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <list>

class ParticleEmitter;

/**
 * Manager and renderer for particles. See ParticleEmitter for creating particles.
 */
class ParticleSystem: public sf::Drawable, sf::NonCopyable
{
public:
    ParticleSystem();
    ~ParticleSystem();

    /**
     * Create particles from an emitter
     */
    void create(const ParticleEmitter& emitter);

    /**
     * Update all particles
     */
    void update(float frametime);

    void removeByEmitter(const ParticleEmitter& emitter);

    void setTexture(const sf::Texture* texture);

    /**
     * Remove all particles
     */
    void clear();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    struct Particle
    {
        Particle(const ParticleEmitter& e);

        const ParticleEmitter& emitter;
        sf::Vector2f   position;
        sf::Vector2f   velocity;
        sf::Color      color;
        const float    lifespan;
        float          remaining_time;
    };

    typedef std::list<Particle> ParticleList;
    ParticleList       m_particles;
    sf::VertexArray    m_vertices;
    const sf::Texture* m_texture;
};

#endif // PARTICLE_SYSTEM_HPP
