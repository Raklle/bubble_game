#include "Player.h"

Player::Player(const sf::Texture& texture, float playerSpeed, float x, float y, const sf::Vector2u& windowSize)
    : sf::Sprite(texture), playerSpeed(playerSpeed), windowSize(windowSize)
{
    setPosition(x, y);
}

void Player::update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (getPosition().x > 0.0f)
            move(-playerSpeed, 0.0f);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (getPosition().x + getGlobalBounds().width < windowSize.x)
            move(playerSpeed, 0.0f);
    }
}

