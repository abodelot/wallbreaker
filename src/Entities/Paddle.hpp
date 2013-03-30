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

	void setSticky(bool sticky);

private:
	void setSize(Size size);

	Size m_size;
	bool m_stikcy;
};

#endif // PADDLE_HPP
