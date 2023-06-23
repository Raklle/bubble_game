#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Player : public sf::Sprite
{
public:
    Player(const sf::Texture& texture, float playerSpeed, float x, float y, const sf::Vector2u& windowSize);
    float playerSpeed;
    void update();
private:
    sf::Vector2u windowSize;
};

