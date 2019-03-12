#ifndef LASER_BEAM_HPP
#define LASER_BEAM_HPP

#include "Entity.hpp"

class LaserBeam: public Entity
{
public:
    LaserBeam();

    void onUpdate(float frametime) override;

    void onBrickHit(Brick& brick, const sf::Vector2f& previous_pos) override;

    void onCeilHit() override;
};

#endif // LASER_BEAM_HPP
