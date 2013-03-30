#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "Entity.hpp"

class Paddle: public Entity
{
public:
	enum Size
	{
		SMALL, MEDIUM, LARGE
	};

	Paddle();

	// override
	void onUpdate(float frametime);

	/**
	 * Increase paddle size
	 */
	void grow();

	/**
	 * Decrease paddle size
	 */
	void shrink();

	/**
	 * Reset paddle to its default behaviour
	 */
	void reset();

	void setSticky(bool sticky);
	bool isSticky() const;

private:
	void setSize(Size size);

	Size m_size;
	bool m_sticky;
};

#endif // PADDLE_HPP
