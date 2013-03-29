#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <cstdlib>

namespace math
{

// Trigonometry ----------------------------------------------------------------

extern const float PI;

/**
 * Convert degrees to radians
 */
inline float to_radians(float degrees)
{
	return degrees * PI / 180;
}

/**
 * Convert radians to degrees
 */
inline float to_degrees(float radians)
{
	// Normalize between 0 and 360
	float degrees = int(radians / PI * 180) % 360;
	return degrees < 0 ? degrees + 360 : degrees;
}

/**
 * Apply translation on a point
 */
inline void translate(sf::Vector2f& point, float angle, float distance)
{
	// Y-axis is flipped
	point.x = point.x + distance * std::cos(angle);
	point.y = point.y - distance * std::sin(angle);
}

/**
 * Distance between 2 points
 */
inline float distance(const sf::Vector2f& a, const sf::Vector2f& b = sf::Vector2f(0.f, 0.f))
{
	// sqrt((x1 - x2)² + (y1 - y2)²)
	return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/**
 * Angle between two points
 */
inline float angle(const sf::Vector2f& a, const sf::Vector2f& b = sf::Vector2f(0.f, 0.f))
{
	// Y-axis is flipped
	return std::atan2(b.y - a.y, b.x - a.x);
}

// Random ----------------------------------------------------------------------

extern unsigned int seed;

/**
 * Get a random int value between begin and end
 */
int rand(int begin, int end);

/**
 * Get a random float value between begin and end
 */
float rand(float begin, float end);

/**
 * Initialize random number generator seed
 */
void set_seed(unsigned int seed);

// Misc ------------------------------------------------------------------------

/**
 * Ensure value is between min and max
 */
template <class T>
inline T clamp(T value, T min, T max)
{
	return value < min ? min : (value > max ? max : value);
}

}

#endif // MATH_HPP
