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
	friend class ParticleSystem;
	public:
		enum Type
		{
			LINEAR,
			TRAIL
		};

		Emitter();

		void setType(Type type);
		void setTimeToLive(float duration);
		void setLooping(bool looping);

		void setParticleColor(const sf::Color& color);
		void setParticleColor(const sf::Color& start, const sf::Color& end);

		void setAngle(float angle, float variation = 0.f);
		void setSpeed(float speed, float variation = 0.f);

		// Create particles linked to the emitter in the particle system
		void launchParticles();
		// Delete all the particle linked to the emitter
		void clearParticles();

		void setParticleCount(int count);
		int getParticleCount() const;

		void setSpawnPosition(const sf::Vector2f& position);
		const sf::Vector2f& getSpawnPosition() const;

		float getParticleAngle() const;
		float getParticleSpeed() const;

	private:
		Type         m_type;
		bool         m_looping;
		float        m_time_to_live;
		sf::Vector2f m_spawn_position;
		int          m_particle_count;
		sf::Color    m_start_color;
		sf::Color    m_end_color;
		float        m_angle;
		float        m_angle_variation;
		float        m_speed;
		float        m_speed_variation;
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
		const float    lifespan;
		float          remaining_time;
	};

	typedef std::list<Particle> ParticleList;
	ParticleList    m_particles;
	sf::VertexArray m_vertices;
};

#endif // PARTICLE_SYSTEM_HPP
