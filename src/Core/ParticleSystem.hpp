#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <list>
#include "Game.hpp"


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

		void setParticleColor(const sf::Color& color);
		const sf::Color& getParticleColor() const;

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

	// create particles
	void create(const Emitter& emitter);


	// update all the particles
	void update(float frametime);

	void removeByEmitter(const Emitter* emitter);

	/**
	 * Remove all particles
	 */
	void clear();

private:
    ParticleSystem();

	// override
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	struct Particle
	{
		Particle(const ParticleSystem::Emitter& e);

		const Emitter& emitter;
		sf::Vector2f   position;
		sf::Vector2f   velocity;
		sf::Color      color;
		float          lifespan;
		float          remaining_time;
	};

	typedef std::list<Particle> ParticleList;
	ParticleList    m_particles;
	sf::VertexArray m_vertices;
};

#endif // PARTICLE_SYSTEM_HPP
