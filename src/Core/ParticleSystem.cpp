#include "ParticleSystem.hpp"
#include "ParticleEmitter.hpp"
#include "Utils/Math.hpp"


ParticleSystem::ParticleSystem():
    m_vertices(sf::Quads, 4000),
    m_texture(nullptr)
{
}


ParticleSystem::~ParticleSystem()
{
    clear();
}


void ParticleSystem::create(const ParticleEmitter& emitter)
{
    for (int i = emitter.getParticleCount(); i >= 0; --i)
    {
        m_particles.push_back(Particle(emitter));
    }
}


void ParticleSystem::update(float frametime)
{
    m_vertices.clear();

    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
    {
        Particle& p = *it;
        p.remaining_time -= frametime;
        if (p.remaining_time <= 0)
        {
            if (p.emitter.m_looping)
            {
                // Reset the particle
                new (&p) Particle(p.emitter);
            }
            else
            {
                // Delete the particle
                it = m_particles.erase(it);
            }
        }
        else
        {
            // Update position
            p.position.x += p.velocity.x * frametime;
            p.position.y += p.velocity.y * frametime;

            // Update color
            float elapsed = p.lifespan - p.remaining_time;
            p.color.r = p.emitter.m_startColor.r + (elapsed * (p.emitter.m_endColor.r - p.emitter.m_startColor.r) / p.lifespan);
            p.color.g = p.emitter.m_startColor.g + (elapsed * (p.emitter.m_endColor.g - p.emitter.m_startColor.g) / p.lifespan);
            p.color.b = p.emitter.m_startColor.b + (elapsed * (p.emitter.m_endColor.b - p.emitter.m_startColor.b) / p.lifespan);
            p.color.a = p.emitter.m_startColor.a + (elapsed * (p.emitter.m_endColor.a - p.emitter.m_startColor.a) / p.lifespan);

            // Build the vertices
            sf::Vertex vertices[4];
            const sf::IntRect& r = p.emitter.getTextureRect();

            vertices[0].texCoords = sf::Vector2f(r.left, r.top);
            vertices[1].texCoords = sf::Vector2f(r.left, r.top + r.height);
            vertices[2].texCoords = sf::Vector2f(r.left + r.width, r.top + r.height);
            vertices[3].texCoords = sf::Vector2f(r.left + r.width, r.top);

            vertices[0].position = sf::Vector2f(p.position.x, p.position.y);
            vertices[1].position = sf::Vector2f(p.position.x, p.position.y + r.height);
            vertices[2].position = sf::Vector2f(p.position.x + r.width, p.position.y + r.height);
            vertices[3].position = sf::Vector2f(p.position.x + r.width, p.position.y);

            for (int i = 0; i < 4; ++i)
            {
                vertices[i].color = p.color;
                m_vertices.append(vertices[i]);
            }

            ++it;
        }
    }
}


void ParticleSystem::clear()
{
    m_particles.clear();
}


void ParticleSystem::setTexture(const sf::Texture* texture)
{
    m_texture = texture;
}


void ParticleSystem::removeByEmitter(const ParticleEmitter& emitter)
{
    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
    {
        if (&(it->emitter) == &emitter)
            it = m_particles.erase(it);
        else
            ++it;
    }
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    target.draw(m_vertices, states);
}


ParticleSystem::Particle::Particle(const ParticleEmitter& e):
    emitter(e),
    position(e.getSpawnPosition()),
    color(e.m_startColor),
    lifespan(math::rand(0.f, e.m_timeToLive))
{
    remaining_time = lifespan;

    // Compute velocity
    float angle = e.getParticleAngle();
    float speed = e.getParticleSpeed();
    velocity.x = speed * std::cos(angle);
    velocity.y = speed * -std::sin(angle);

    position.x -= e.getTextureRect().width / 2;
    position.y -= e.getTextureRect().height / 2;
}
