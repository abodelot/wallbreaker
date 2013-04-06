#include "ParticleSystem.hpp"
#include "Utils/Math.hpp"


ParticleSystem::Emitter::Emitter():
	m_type(LINEAR),
	m_particle_count(100),
	m_looping(false),
	m_color(sf::Color(255, 128, 128, 255)),
	m_to_color(sf::Color(0, 0, 0, 0)),
	m_angle(0),
	m_angle_variation(0.4),
	m_time_to_live(5),
	m_speed(100),
	m_speed_variation(50)
{
}


void ParticleSystem::Emitter::setParticleColor(const sf::Color& color)
{
	m_color = color;
}


const sf::Color& ParticleSystem::Emitter::getParticleColor() const
{
	return m_color;
}


void ParticleSystem::Emitter::launchParticles()
{
	ParticleSystem::instance().create(*this);
}


void ParticleSystem::Emitter::clearParticles()
{
	ParticleSystem::instance().removeByEmitter(this);
}


int ParticleSystem::Emitter::getParticleCount() const
{
	return m_particle_count;
}


void ParticleSystem::Emitter::setParticleCount(int count)
{
	m_particle_count = count;
}


float ParticleSystem::Emitter::getParticleAngle() const
{
	return math::rand(m_angle - m_angle_variation, m_angle + m_angle_variation);
}


float ParticleSystem::Emitter::getParticleSpeed() const
{
	return math::rand(m_speed - m_speed_variation, m_speed + m_speed_variation);
}


ParticleSystem& ParticleSystem::instance()
{
	static ParticleSystem self;
	return self;
}


ParticleSystem::ParticleSystem():
	m_vertices(sf::Quads, 4000)
{
}


void ParticleSystem::create(const Emitter& emitter)
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
				// Reset properties
				p.color = p.emitter.getParticleColor();
				p.position = p.emitter.getSpawnPosition();
				p.lifespan = p.remaining_time = math::rand(0.f, p.emitter.m_time_to_live);

				// Compute velocity
				float angle = p.emitter.getParticleAngle();
				float speed = p.emitter.getParticleSpeed();
				p.velocity.x = speed * std::cos(angle);
				p.velocity.y = speed * -std::sin(angle);
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
			p.color.r = p.emitter.m_color.r + (elapsed * (p.emitter.m_to_color.r - p.emitter.m_color.r) / p.lifespan);
			p.color.g = p.emitter.m_color.g + (elapsed * (p.emitter.m_to_color.g - p.emitter.m_color.g) / p.lifespan);
			p.color.b = p.emitter.m_color.b + (elapsed * (p.emitter.m_to_color.b - p.emitter.m_color.b) / p.lifespan);
			p.color.a = p.emitter.m_color.a + (elapsed * (p.emitter.m_to_color.a - p.emitter.m_color.a) / p.lifespan);

			// Build the vertices (1px width square)
			sf::Vertex vertices[4];
			vertices[1].position.x = 1;
			vertices[2].position.x = 1;
			vertices[2].position.y = 1;
			vertices[3].position.y = 1;

			for (int i = 0; i < 4; ++i)
			{
				vertices[i].position += p.position;
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


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	//states.blendMode = sf::BlendAdd;

	target.draw(m_vertices, states);
}


void ParticleSystem::removeByEmitter(const Emitter* emitter)
{
	for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
	{
		if (&(it->emitter) == emitter)
			it = m_particles.erase(it);
		else
			++it;
	}
}


ParticleSystem::Particle::Particle(const ParticleSystem::Emitter& e):
	emitter(e),
	position(e.getSpawnPosition()),
	color(e.getParticleColor())
{
	lifespan = remaining_time = math::rand(0.f, e.m_time_to_live);

	// Compute velocity
	float angle = e.getParticleAngle();
	float speed = e.getParticleSpeed();
	velocity.x = speed * std::cos(angle);
	velocity.y = speed * -std::sin(angle);
}

