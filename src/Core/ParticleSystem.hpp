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
		Emitter();

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

		void setTextureRect(const sf::IntRect& rect);
		const sf::IntRect& getTextureRect() const;

	private:
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
		sf::IntRect  m_texture_rect;
	};

	static ParticleSystem& instance();

	/**
	 * Create particles from an emitter
	 */
	void create(const Emitter& emitter);

	/**
	 * Update all particles
	 */
	void update(float frametime);

	void removeByEmitter(const Emitter* emitter);

	void setTexture(const sf::Texture* texture);

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
	ParticleList       m_particles;
	sf::VertexArray    m_vertices;
	const sf::Texture* m_texture;
};

#endif // PARTICLE_SYSTEM_HPP
