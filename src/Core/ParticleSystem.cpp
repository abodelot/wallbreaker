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


void ParticleSystem::Emitter::setColor(const sf::Color& color)
{
	m_color = color;
}


const sf::Color& ParticleSystem::Emitter::getColor() const
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

void ParticleSystem::create(const Emitter& emitter)
{
	const sf::Vector2f& position = emitter.getSpawnPosition();
	const sf::Color& color = emitter.getColor();
	for (int i = emitter.getParticleCount(); i >= 0; --i)
	{
		m_particles.push_back(Particle(emitter, position, color));
	}
}


void ParticleSystem::update(float frametime)
{
	for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
	{
		Particle& p = *it;
		p.remaining_time -= frametime;
		if (p.remaining_time <= 0)
		{
			if (p.emitter.m_looping)
			{
				m_particles.push_back(Particle(p.emitter, p.emitter.getSpawnPosition(), p.emitter.getColor()));

			}
			// delete the particle
			it = m_particles.erase(it);
		}
		else
		{
			// update position
			float dx = p.speed.x * frametime;
			float dy = p.speed.y * frametime;
			for (int i = 0; i < 4; ++i)
			{
				p.vertex[i].position.x += dx;
				p.vertex[i].position.y += dy;
			}

			// update color
			float elapsed = p.lifespan - p.remaining_time;
			for (int i = 0; i < 4; ++i)
			{
				p.vertex[i].color.r = p.emitter.m_color.r + (elapsed * (p.emitter.m_to_color.r - p.emitter.m_color.r) / p.lifespan);
				p.vertex[i].color.g = p.emitter.m_color.g + (elapsed * (p.emitter.m_to_color.g - p.emitter.m_color.g) / p.lifespan);
				p.vertex[i].color.b = p.emitter.m_color.b + (elapsed * (p.emitter.m_to_color.b - p.emitter.m_color.b) / p.lifespan);
				p.vertex[i].color.a = p.emitter.m_color.a + (elapsed * (p.emitter.m_to_color.a - p.emitter.m_color.a) / p.lifespan);
			}

			++it;
		}
	}
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.blendMode = sf::BlendAdd;

	for (ParticleList::const_iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		target.draw(it->vertex, 4, sf::Quads, states);

	}
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

ParticleSystem::Particle::Particle(const ParticleSystem::Emitter& e, const sf::Vector2f& pos, const sf::Color& color):
	emitter(e)
{
		lifespan = remaining_time = math::rand(0.f, e.m_time_to_live);
		float angle = e.getParticleAngle();
		int velocity = e.getParticleSpeed();
		speed.x = velocity * std::cos(angle);
		speed.y = velocity * -std::sin(angle);

		for (int i = 0; i < 4; ++i)
		{
			vertex[i].position = pos;
			vertex[i].color = color;
		}
		vertex[1].position.x += 1;
		vertex[2].position.x += 1;
		vertex[2].position.y += 1;
		vertex[3].position.y += 1;
	}

