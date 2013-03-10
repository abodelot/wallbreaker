#ifndef PLAYER_PAD_HPP
#define PLAYER_PAD_HPP
#include "Entity.hpp"

#include "Entities/Entity.hpp"


class PlayerPad: public Entity
{
public:
	enum PadSize
	{
		SMALL, MEDIUM, LARGE
	};

	PlayerPad();

	// override
	void onUpdate(float frametime);

	void setPadSize(PadSize size);

private:
	PadSize m_pad_size;
};

#endif // PLAYER_PAD_HPP
