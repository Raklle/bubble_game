#include "CircleEnemy.h"
#include <iostream>

CircleEnemy::CircleEnemy(float radius, const sf::Vector2f& position, const sf::Color& color, const sf::Vector2f& initialVelocity, int tier)
    : sf::CircleShape(radius), tier(tier), radius_(radius)
{
    setFillColor(color);
    setPosition(position);
    velocity = initialVelocity;
}

void CircleEnemy::update(sf::RenderWindow& window)
{
    if (getPosition().x + getRadius() * 2 >= window.getSize().x || getPosition().x <= 0.0f)
    {
        velocity.x = -velocity.x;
    }
    if (getPosition().y <= 0.0f)
    {
        velocity.y = -velocity.y;
    }
    else if (getPosition().y + getRadius() * 2 >= window.getSize().y)
    {
        velocity.y = -velocity.y;
    }
    velocity.y = velocity.y + 0.00007;
    move(velocity);

}

void CircleEnemy::setVelocity(const sf::Vector2f& newVelocity)
{
    velocity = newVelocity;
}

int CircleEnemy::getTier()
{
    return tier;
}

float CircleEnemy::getRadius()
{
    return radius_;
}

void CircleEnemy::kill()
{
      setRadius(2.0f);

}

sf::Vector2f CircleEnemy::getVelocity() const
{
    return velocity;
}
