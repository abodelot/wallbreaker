#ifndef EFFECT_HPP
#define EFFECT_HPP

#include <list>
#include <SFML/Graphics.hpp>

/**
 * Static class for managing animations based on sprite transformations.
 */
class Effect
{
public:
    /**
     * Create a Zoom animation
     * @param target: sprite to zoom
     * @param factor: final scale factor
     */
    static void zoom         (sf::Sprite& target, float factor, float duration = 1.f);
    static void zoomAndRevert(sf::Sprite& target, float factor, float duration = 1.f);

    /**
     * Create a Move animation
     * @param target: sprite to move
     * @param distance: relative movement, final position is current position + distance
     */
    static void move         (sf::Sprite& target, const sf::Vector2f& distance, float duration = 1.f);
    static void moveAndRevert(sf::Sprite& target, const sf::Vector2f& distance, float duration = 1.f);

    /**
     * Create a Rotate animation
     * @param target: sprite to rotate
     * @param angle: angle of the rotation (degrees), final rotation is current rotation + angle
     */
    static void rotate         (sf::Sprite& target, float angle, float duration = 1.f);
    static void rotateAndRevert(sf::Sprite& target, float angle, float duration = 1.f);

    /**
     * Create a Fade-in animation
     */
    static void fadeIn(sf::Sprite& target, float duration = 1.f);

    /**
     * Create a Fade-out animation
     */
    static void fadeOut(sf::Sprite& target, float duration = 1.f);

    /**
     * Update all animations for the current frame
     */
    static void update(float frametime);

    /**
     * Stop and delete all running animations
     * All sprite targets will be immediately set to their final animation state
     */
    static void stopAll();

private:
    enum Type
    {
        ZOOM,
        ZOOM_REVERT,
        MOVE,
        MOVE_REVERT,
        ROTATE,
        ROTATE_REVERT,
        FADE_IN,
        FADE_OUT,
    };

    struct Object
    {
        struct Zoom
        {
            float final_scale;
            float scale_per_sec;
        };

        struct Move
        {
            float final_x;
            float final_y;
            float x_per_sec;
            float y_per_sec;
        };

        struct Rotate
        {
            float final_angle;
            float angle_per_sec;
        };

        union Data
        {
            Zoom zoom;
            Move move;
            Rotate rotate;
        };

        Object(Type t, sf::Sprite& s, float dur):
            type(t), target(s), duration(dur)
        {
            createdAt.restart();
        }

        Object(Data& d, Type t, sf::Sprite& s, float dur):
            data(d), type(t), target(s), duration(dur)
        {
            createdAt.restart();
        }

        Data data;
        Type type;
        sf::Sprite& target;
        const float duration;
        sf::Clock createdAt;
    };

    static void pushObject(Object::Data& data, Type type, sf::Sprite& target, float duration);

    typedef std::list<Object> ObjectList;

    static ObjectList m_objects;
};

#endif // EFFECT_HPP
