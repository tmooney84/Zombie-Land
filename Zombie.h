#pragma once
#include <SFML/Graphics.hpp>

class Zombie
{
    private:
    //speed of each type of zombie
    const float BLOATER_SPEED = 40;
    const float CHASER_SPEED = 80;
    const float CRAWLER_SPEED = 30;
    // health of zombie
    const float BLOATER_HEALTH = 5;
    const float CHASER_HEALTH = 1;
    const float CRAWLER_HEALTH = 3;
    // zombie speed variance
    const int MAX_VARIANCE = 30;
    const int OFFSET = 101 - MAX_VARIANCE;
    //zombie location
    sf::Vector2f m_Position;
    // Zombie sprite
    sf::Sprite m_Sprite;
    // speed zombie run/crawl
    float m_Speed;
    // how much health
    float m_Health;
    // Zombie alive?        //!!! ADDED true for initial construction, need to test to make sure works when zombies dead
    bool m_Alive = true;

    public:
        // Handle when a bullet hits a zombie
        bool hit();
        // Find out if zombie is alive
        bool isAlive();
        // Spawn a new zombie
        void spawn(float startX, float startY, int type, int seed);
        // Return a rectangle that is the position in the world
        sf::FloatRect getPosition();
        // Get a copy of the spirte to draw
        sf::Sprite getSprite();
        // Update the zombie each frame
        void update(float elapsedTime, sf::Vector2f playerLocation);   
};