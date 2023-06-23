#pragma once
#include <SFML/Graphics.hpp>

class CircleEnemy : public sf::CircleShape
{
private:
    float radius_;
    sf::Vector2f velocity;
    int tier;

public:
    CircleEnemy(float radius, const sf::Vector2f& position, const sf::Color& color, const sf::Vector2f& initialVelocity, int tier);

    void update(sf::RenderWindow&);
    void setVelocity(const sf::Vector2f& newVelocity);
    int getTier();
    float getRadius();
    void kill();
    sf::Vector2f getVelocity() const;

};