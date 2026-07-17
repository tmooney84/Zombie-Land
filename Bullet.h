#pragma once
#include <SFML/Graphics.hpp>

class Bullet
{
    private:
        // Bullet location
        sf::Vector2f m_Position;
        // Bullet shape
        sf::RectangleShape m_BulletShape;
        // Bullet in air?
        bool m_InFlight = false;
        // Speed of bullet
        float m_BulletSpeed = 1000;
        // fraction of 1 pixel that bullet 
        // travels horizontally and vertically each frame
        float m_BulletDistanceX;
        float m_BulletDistanceY;

        // bullet boundaries to stop flight
        float m_MaxX;
        float m_MinX;
        float m_MaxY;
        float m_MinY;

    public:
        Bullet();
        // Stop the bullet
        void stop();
        // Returns the value of m_InFlight
        bool isInFlight();
        // Fire a new bullet
        void shoot(float startX, float startY,
            float xTarget, float yTarget);
        // Tell the calling code where
        // the bullet is in the world
        sf::FloatRect getPostion();
        // Return the actual shape (for drawing)
        sf::RectangleShape getShape();
        // Update the bullet each frame
        void update(float elapsedTime);
};