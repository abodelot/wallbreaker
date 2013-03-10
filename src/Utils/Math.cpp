#include <ctime>
#include "Math.hpp"

namespace math
{

// Set the random numbers sequence seed with the current system time, so that it is always different
unsigned int static set_random_seed()
{
	unsigned int seed = static_cast<unsigned int>(time(NULL));
	srand(seed);
	return seed;
}


const float PI = 3.14159265f;
unsigned int seed = set_random_seed();


int rand(int begin, int end)
{
	return std::rand() % (end - begin + 1) + begin;
}


float rand(float begin, float end)
{
	return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}


float angle(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float x = p1.x - p2.x;
	if (x == 0.f)
	{
		return 0.f;
	}
	float y = p1.y - p2.y;
	float radians = std::atan(-y / x);
	if (p2.x > p1.x)
	{
		radians += PI;
	}
	return radians;
}


sf::Color random_color(sf::Uint8 min_r, sf::Uint8 min_g, sf::Uint8 min_b, sf::Uint8 max_r, sf::Uint8 max_g, sf::Uint8 max_b)
{
	return sf::Color(
		rand(min_r, max_r),
		rand(min_g, max_g),
		rand(min_b, max_b)
	);
}

void set_seed(unsigned int s)
{
    srand(s);
    seed = s;
}


}
