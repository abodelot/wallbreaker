#include "Wallbreaker.hpp"
#include "Core/Game.hpp"
#include "Core/Effect.hpp"
#include "Core/LevelManager.hpp"
#include "Core/Resources.hpp"
#include "Core/Settings.hpp"
#include "Core/SoundSystem.hpp"
#include "Entities/Context.hpp"
#include "Entities/LaserBeam.hpp"
#include "Entities/PowerUp.hpp"
#include "Gui/Theme.hpp"
#include "Gui/Label.hpp"
#include "Utils/Math.hpp"

#define X_OFFSET 32

Wallbreaker::Wallbreaker():
    m_width(LevelManager::NB_BRICK_COLS * Brick::WIDTH),
    m_height(LevelManager::NB_BRICK_LINES * Brick::HEIGHT),
    m_level(LevelManager::getInstance()),
    m_remainingBricks(0),
    m_titleText(gui::Theme::font),
    m_score(0),
    m_status(READY),
    m_playerLives(0),
    m_pauseMenu(Game::getInstance().getWindow()),
    m_gameOverMenu(Game::getInstance().getWindow())
{
    // Initialize render texture
    m_levelTexture.create(m_width, m_height);

    // Sprites used for rendering the game area
    m_levelSprite.setTexture(m_levelTexture.getTexture());
    m_levelSprite.setOrigin(m_width / 2, m_height / 2);
    m_levelSprite.setPosition(
        m_width / 2 + LevelManager::BORDER_SIZE + X_OFFSET,
        m_height / 2 + LevelManager::BORDER_SIZE
    );

    // Align on right
    m_hud.setPosition(APP_WIDTH - HUD::WIDTH, 0);

    m_backgroundSprite.setTexture(Resources::getTexture("background.png"));
    m_bordersSprite.setTexture(Resources::getTexture("borders.png"));
    m_bordersSprite.setPosition(X_OFFSET, 0);
    // Player paddle positioned at the bottom-center
    m_paddle.setPosition((m_width - m_paddle.getWidth()) / 2, m_height - m_paddle.getHeight());

    // Build 'pause' menu
    m_pauseMenu.setPosition(
        LevelManager::BORDER_SIZE + X_OFFSET + (m_width - gui::Theme::widgetWidth) / 2, 120
    );
    m_pauseMenu.addButton("Resume",  1);
    m_pauseMenu.addButton("Options", 2);
    m_pauseMenu.addButton("Quit",    3);

    // Build 'game over' menu
    m_gameOverMenu.setPosition(m_pauseMenu.getPosition());
    m_gameOverMenu.addButton("Try again", 1);
    m_gameOverMenu.addButton("Quit",      2);

    // States are stored in Game, therefore Wallbreaker instance is unique and
    // context can be safely defined once in Wallbreaker ctor
    Context::get().wallbreaker = this;
    Context::get().particles = &m_particles;
}


Wallbreaker::~Wallbreaker()
{
    clearEntities();
}


void Wallbreaker::onEvent(const sf::Event& event)
{
    switch (m_status)
    {
        case PLAYING:
            switch (event.type)
            {
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Escape:
                            setStatus(PAUSED);
                            break;
                        case sf::Keyboard::Space:
                            if (m_paddle.isSticky())
                                applyOnEachBall(&Ball::unstick);
                            else if (m_paddle.hasLaser())
                                addEntity(m_paddle.shoot());
                            break;
#ifdef WALLBREAKER_DEBUG
                        case sf::Keyboard::R: // Reload level
                            m_remainingBricks = m_level.reload();
                            m_hud.setBrickCount(m_remainingBricks);
                            break;
                        case sf::Keyboard::N: // Go to next level
                            loadNextLevel();
                            break;
                        case sf::Keyboard::G: // Simulate LARGE_PADDLE
                            m_paddle.grow();
                            break;
                        case sf::Keyboard::S: // Simulate SMALL_PADDLE
                            m_paddle.shrink();
                            break;
                        case sf::Keyboard::Y: // Simulate STICKY_PADDLE
                            m_paddle.activeSticky();
                            break;
                        case sf::Keyboard::L: // Simulate LASER_PADDLE
                            m_paddle.activeLaser();
                            break;
                        case sf::Keyboard::T: // Simulate TRIPLE_BALL
                            createBall();
                            createBall();
                            break;
                        case sf::Keyboard::P: // Simulate POWER_BALL
                            applyOnEachBall(&Ball::enablePowerBall);
                            break;
                        case sf::Keyboard::E: // Simulate SPEED_RESET
                            applyOnEachBall(&Ball::resetSpeed);
                            break;
#endif
                        default:
                            break;
                    }
                    break;

                case sf::Event::MouseMoved:
                    // Paddle follows the mouse cursor
                    m_paddle.onMouseMoved(Game::getInstance().getWindow().mapPixelToCoords({
                        event.mouseMove.x, event.mouseMove.y
                    }));
                    break;

                case sf::Event::MouseButtonPressed:
                    if (m_paddle.isSticky())
                        applyOnEachBall(&Ball::unstick);
                    else if (m_paddle.hasLaser())
                        addEntity(m_paddle.shoot());
                    break;

                case sf::Event::LostFocus:
                    setStatus(PAUSED);
                    break;

                default:
                    break;
            }
            break;

        case PAUSED:
            switch (m_pauseMenu.onEvent(event))
            {
                case 1: // Resume
                    setStatus(PLAYING);
                    break;
                case 2: // Go to options menu
                    Game::getInstance().setCurrentState("OptionsMenu");
                    break;
                case 3: // Clear game and back to main menu
                    resetGame();
                    Game::getInstance().restorePreviousState();
                    break;
                default:
                    // Resume
                    if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                        setStatus(PLAYING);
                    break;
            }
            break;

        case READY:
            if (event.type == sf::Event::MouseButtonPressed ||
                (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space))
            {
                applyOnEachBall(&Ball::createParticles);
                setStatus(PLAYING);
            }
            else if (event.type == sf::Event::MouseMoved)
            {
                // Paddle follows the mouse cursor
                m_paddle.onMouseMoved(Game::getInstance().getWindow().mapPixelToCoords({
                    event.mouseMove.x, event.mouseMove.y
                }));
            }
            break;

        case GAME_OVER:
            switch (m_gameOverMenu.onEvent(event))
            {
                case 1: // Continue (reload current level)
                    resetGame(m_level.getCurrentLevel());
                    break;
                case 2: // Clear game and back to main menu
                    resetGame();
                    Game::getInstance().restorePreviousState();
                    break;
            }
            break;
    }
}


void Wallbreaker::onFocus()
{
    if (m_status != PAUSED)
    {
        resetGame();
    }
}


void Wallbreaker::update(float frametime)
{
    if (m_status == PLAYING || m_status == READY)
    {
        Effect::update(frametime);

        // Update player paddle and make sure it remains inside bounds
        m_paddle.onUpdate(frametime);
        if (m_paddle.getX() < 0)
            m_paddle.setX(0);
        else if (m_paddle.getX() + m_paddle.getWidth() > m_width)
            m_paddle.setX(m_width - m_paddle.getWidth());

        if (m_status == PLAYING)
        {
            updateEntities(frametime);
        }
        else
        {
            Entity* ball = m_entities.front();
            // Keep the ball centered on the player pad
            ball->setPosition(
                m_paddle.getX() + (m_paddle.getWidth() - ball->getWidth()) / 2,
                m_paddle.getY() - ball->getHeight()
            );
        }
        m_particles.update(frametime);
    }
    updateLevelTexture();
}


void Wallbreaker::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(m_backgroundSprite, states);
    target.draw(m_bordersSprite, states);
    target.draw(m_levelSprite, states);
    target.draw(m_hud, states);

    if (m_status != PLAYING)
    {
        sf::RectangleShape overlay(sf::Vector2f(m_levelTexture.getSize()));
        overlay.setPosition(m_levelSprite.getPosition() - m_levelSprite.getOrigin());
        overlay.setFillColor({0, 0, 0, 128});
        target.draw(overlay);
        target.draw(m_titleText);
        if (m_status == PAUSED)
        {
            m_pauseMenu.show();
        }
        else if (m_status == GAME_OVER)
        {
            m_gameOverMenu.show();
        }
    }
}


void Wallbreaker::resetGame(size_t level)
{
    Effect::stopAll();

    // Reset score
    m_score = 0;
    m_hud.setScore(0);
    m_hud.setHighscore(Settings::highscore);

    // Reset lives
    m_playerLives = MAX_PLAYER_LIVES;
    m_hud.setLifeCount(MAX_PLAYER_LIVES);

    // Load level
    m_remainingBricks = m_level.loadAt(level);
    m_hud.setBrickCount(m_remainingBricks);
    m_hud.setLevel(m_level.getCurrentLevel());

    animateBricks();
    setStatus(READY);
}


void Wallbreaker::updateLevelTexture()
{
    m_levelTexture.clear(sf::Color::Transparent);
    // Draw bricks
    m_levelTexture.draw(m_level);

    // Draw particles
    m_levelTexture.draw(m_particles);

    // Draw entities
    m_levelTexture.draw(m_paddle);
    for (Entity*& entity: m_entities)
    {
        m_levelTexture.draw(*entity);
    }

    m_levelTexture.display();
}


void Wallbreaker::addPlayerLife()
{
    if (m_playerLives < MAX_PLAYER_LIVES)
        m_hud.setLifeCount(++m_playerLives);
}


void Wallbreaker::applyOnEachBall(Ball::ActionPointer action)
{
    for (Entity*& entity: m_entities)
    {
        Ball* ball = entity->toBall();
        if (ball != NULL)
        {
            (ball->*action)();
        }
    }
}


void Wallbreaker::addEntity(Entity* entity)
{
    m_entities.push_back(entity);
}


void Wallbreaker::clearEntities()
{
    for (Entity*& entity: m_entities)
    {
        delete entity;
    }
    m_entities.clear();
}


void Wallbreaker::updateEntities(float frametime)
{
    // For each entity
    for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
    {
        Entity& entity = **it;
        if (entity.isAlive())
        {
            sf::Vector2f old_pos = entity.getPosition();
            entity.onUpdate(frametime);

            // Ensure entity doesn't go beyond the walls
            if (entity.getY() < 0)
            {
                entity.onCeilHit();
                entity.setY(0);
            }
            else if (entity.getPosition().x < 0)
            {
                entity.onWallHit();
                entity.setX(0);
            }
            else if (entity.getX() + entity.getWidth() > m_width)
            {
                entity.onWallHit();
                entity.setX(m_width - entity.getWidth());
            }
            // Kill entities if they go beyond the bottom line
            else if (entity.getY() > m_height)
            {
                entity.kill();
            }
            // Check if entity collides with paddle
            else if ((entity.getY() + entity.getHeight() > m_paddle.getY())
                  && (entity.getY() < m_paddle.getY())
                  && (entity.getX() + entity.getWidth() > m_paddle.getX())
                  && (entity.getX() < m_paddle.getX() + m_paddle.getWidth()))
            {
                entity.setY(m_height - m_paddle.getHeight() - entity.getHeight());
                entity.onPaddleHit(m_paddle);
            }
            // Check if entity is colliding with a brick
            else
            {
                // Get corners
                int left =   entity.getX() / Brick::WIDTH;
                int top =    entity.getY() / Brick::HEIGHT;
                int right =  (entity.getX() + entity.getWidth())  / Brick::WIDTH;
                int bottom = (entity.getY() + entity.getHeight()) / Brick::HEIGHT;

                if (checkBrick(entity, top, left, old_pos)
                 || checkBrick(entity, top, right, old_pos)
                 || checkBrick(entity, bottom, left, old_pos)
                 || checkBrick(entity, bottom, right, old_pos))
                {
                    if (m_remainingBricks == 0)
                    {
                        if (m_level.getCurrentLevel() == m_level.getLevelCount())
                        {
                            setStatus(GAME_OVER);
                        }
                        else
                        {
                            loadNextLevel();
                            setStatus(READY);
                        }
                        break;
                    }
                }
            }
            ++it;
        }
        else
        {
            // Remove dead entity from entity list
            delete *it;
            it = m_entities.erase(it);
            if (Ball::getCount() == 0)
            {
                m_hud.setLifeCount(--m_playerLives);
                SoundSystem::playSound("life-lost.ogg");
                setStatus(m_playerLives == 0 ? GAME_OVER : READY);
                break;
            }
        }
    }
}


bool Wallbreaker::checkBrick(Entity& entity, int i, int j, const sf::Vector2f& old_pos)
{
    if (i >= 0 && i < LevelManager::NB_BRICK_LINES && j >= 0 && j < LevelManager::NB_BRICK_COLS)
    {
        Brick& brick = m_level.getBrick(i, j);
        if (brick.isActive())
        {
            entity.onBrickHit(brick, old_pos);

            if (!brick.isActive())
            {
                // Randomly create a new power-up
                if (math::rand(0, 9) == 0)
                {
                    PowerUp* powerup = PowerUp::createRandom();
                    powerup->setPosition(brick.getPosition());
                    addEntity(powerup);
                }
                // Update remaining bricks counter
                --m_remainingBricks;
                m_hud.setBrickCount(m_remainingBricks);
                // Upgrade score
                ++m_score;
                m_hud.setScore(m_score);
                if (m_score > Settings::highscore)
                {
                    Settings::highscore = m_score;
                    m_hud.setHighscore(Settings::highscore);
                }
            }
            return true;
        }
    }
    return false;
}


void Wallbreaker::loadNextLevel()
{
    Effect::stopAll();
    m_remainingBricks = m_level.loadNext();
    m_hud.setLevel(m_level.getCurrentLevel());
    m_hud.setBrickCount(m_remainingBricks);
    SoundSystem::playSound("level-complete.ogg");

    animateBricks();
}


void Wallbreaker::setStatus(Status status)
{
    m_status = status;
    switch (status)
    {
    case READY:
        // Remove power-ups applied to the paddle
        m_paddle.reset();

        // Clear all and spawn a new ball
        clearEntities();
        createBall();
        m_titleText.setString("Ready?");
        Game::getInstance().getWindow().setMouseCursorVisible(false);
        break;

    case GAME_OVER:
        m_titleText.setString(m_playerLives > 0 ? "Game complete!" : "Game Over");
        Game::getInstance().getWindow().setMouseCursorVisible(true);
        break;

    case PAUSED:
        m_titleText.setString("Paused");
        Game::getInstance().getWindow().setMouseCursorVisible(true);
        break;

    case PLAYING:
        Game::getInstance().getWindow().setMouseCursorVisible(false);
        break;
    }
    // Center text
    m_titleText.setScale(2, 2);
    m_titleText.setPosition(
        X_OFFSET + LevelManager::BORDER_SIZE + (m_width - m_titleText.getSize().x) / 2, 80
    );
}


void Wallbreaker::createBall()
{
    Ball* ball = new Ball();
    // Center ball on player pad
    ball->setPosition(
        m_paddle.getPosition().x + (m_paddle.getWidth() - ball->getWidth()) / 2,
        m_height - m_paddle.getHeight() - ball->getHeight()
    );
    if (m_status == PLAYING)
    {
        ball->createParticles();
    }
    addEntity(ball);
}


void Wallbreaker::animateBricks()
{
    for (size_t i = 0; i < LevelManager::NB_BRICK_LINES; ++i)
    {
        for (size_t j = 0; j < LevelManager::NB_BRICK_COLS; ++j)
        {
            Brick& brick = m_level.getBrick(i, j);
            if (brick.getType() != Brick::NONE)
            {
                brick.setScale(0, 0);
                Effect::zoom(brick, 1, 1);
            }
        }
    }
}
