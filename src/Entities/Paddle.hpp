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

	void setSize(Size size);

private:
	Size m_size;
};

#endif // PADDLE_HPP
