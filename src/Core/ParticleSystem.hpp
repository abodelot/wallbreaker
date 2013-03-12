#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include "Game.hpp"
#include "Utils/Math.hpp"

class ParticleSystem: public sf::Drawable
{
public:
	class Emitter
	{
	public:
		enum Type
		{
			LINEAR,
			TRAIL
		};

		Emitter();

		void setColor(const sf::Color& color);
		const sf::Color& getColor() const;

		// Create particles linked to the emitter in the particle system
		void launchParticles();
		// Delete all the particle linked to the emitter
		void clearParticles();

		void setParticleCount(int count);
		int getParticleCount() const;

		virtual sf::Vector2f getSpawnPosition() const = 0;

		float getParticleAngle() const;
		float getParticleSpeed() const;

	private:



	public:
		Type m_type;
		int m_particle_count;
		bool m_looping;
		sf::Color m_color;
		sf::Color m_to_color;
		float m_angle;
		float m_angle_variation;
		float m_time_to_live;
		float m_speed;
		float m_speed_variation;
	};


	static ParticleSystem& instance();

	// creates particles
	void create(const Emitter& emitter);


	// update all the particles
	void update(float frametime);

	void removeByEmitter(const Emitter* emitter);

private:
	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	struct Particle
	{
		Particle(const ParticleSystem::Emitter& e, const sf::Vector2f& pos, const sf::Color& color):
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

		const Emitter&      emitter;
		sf::Vertex   vertex[4];
		float lifespan;
		float remaining_time;
		sf::Vector2f speed;

	};


	typedef std::list<Particle> ParticleList;

	ParticleList m_particles;
};

#endif // PARTICLE_SYSTEM_HPP
