#include <SFML/Graphics.hpp>
#include <list>
#include "CircleEnemy.h"
#include <iostream>
#include <string>
#include "Player.h"
#include <cmath>
#include <random>

void addRandomEnemy(std::list<CircleEnemy>& enemies, sf::RenderWindow& window,int tier)
{
    // Losowanie promienia z zakresu 20.0f - 40.0f
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> radiusDist(20.0f, 40.0f);
    float radius = radiusDist(gen);

    // Pozycja zawsze w środku okna
    sf::Vector2f position(window.getSize().x / 2.0f, window.getSize().y / 3.0f);

    // Losowy kolor
    std::uniform_int_distribution<int> colorDist(0, 255);
    sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));

    std::uniform_int_distribution<int> dist(0, 1);
    int randomValue = dist(gen);

    // Losowa prędkość w zakresie
    std::uniform_real_distribution<float> velocityYDist(-0.03f, -0.01f);
    if (randomValue) {
        std::uniform_real_distribution<float> velocityXDist(-0.05f, -0.03f);
        sf::Vector2f velocity(velocityXDist(gen), velocityYDist(gen));
        enemies.emplace_back(CircleEnemy(radius, position, color, velocity, tier));
    }
    else {
        std::uniform_real_distribution<float> velocityXDist(0.03f, 0.05f);
        sf::Vector2f velocity(velocityXDist(gen), velocityYDist(gen));
        enemies.emplace_back(CircleEnemy(radius, position, color, velocity, tier));
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bubble Trouble");

    std::list<CircleEnemy> enemies;

    // Dodawanie wrogów
    addRandomEnemy(enemies, window, 2);
    addRandomEnemy(enemies, window, 2);
    addRandomEnemy(enemies, window, 1);

    sf::Font font;
    if (!font.loadFromFile("roboto.ttf"))
    {
        std::cout << "Failed to load font." << std::endl;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(window.getSize().x - 100, 10);

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("pxArt.png")) {
        std::cerr << "Failed to load pxArt.png" << std::endl;
    }

    sf::Texture projectileTexture;
    if (!projectileTexture.loadFromFile("spear.png")) {
        std::cerr << "Failed to load spear.png" << std::endl;
    }

    float x = window.getSize().x / 2 - 15.0f;
    float y = window.getSize().y - 50.0f;

    float playerSpeed = 0.12f;
    sf::Vector2u size = window.getSize();
    Player player = Player(playerTexture, playerSpeed, x, y, size);

    sf::Sprite spear;
    spear.setTexture(projectileTexture);
    spear.setScale(2.0f, 1.0f);
    spear.setPosition(player.getPosition());

    bool isProjectileActive = false;

    int score = 0;
    int gameState = 3;
    //1- menu, 2-gra, 3-koniec gry

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && (gameState == 1) )
            {
                // Kliknięcie po zakończeniu gry - zresetuj wartości i rozpocznij nową rozgrywkę
                score = 0;
                gameState = 2;
                enemies.clear();
                addRandomEnemy(enemies, window, 2);
                addRandomEnemy(enemies, window, 2);
                addRandomEnemy(enemies, window, 1);
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && ( gameState == 3))
            {
                score = 0;
                gameState = 2;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
            {
                if (!isProjectileActive)
                {
                    spear.setPosition(player.getPosition());
                    isProjectileActive = true;
                }
            }

        }
        window.clear(sf::Color::White);
        if (gameState == 1)
        {
            // Wyświetl informacje o końcu gry (np. wynik) na środku ekranu
            sf::Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setCharacterSize(48);
            gameOverText.setFillColor(sf::Color::Black);
            gameOverText.setString("Game Over\nScore: " + std::to_string(score) + "\n click R to restart");
            gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - gameOverText.getGlobalBounds().height / 2);
            window.draw(gameOverText);
        }else if(gameState == 2)
        {


            // Aktualizacja wrogów
            for (auto it = enemies.begin(); it != enemies.end(); ++it)
            {
                auto& enemy = *it;
                enemy.update(window);
            }

            // Aktualizacja ruchu sprite'a pocisku
            if (isProjectileActive)
            {
                spear.move(0.0f, -0.6f);
                if (spear.getPosition().y < 0)
                {
                    isProjectileActive = false;
                }
            }

            window.clear(sf::Color::White);

            // Rysowanie wrogów
            for (auto it = enemies.begin(); it != enemies.end(); ++it)
            {
                auto& enemy = *it;
                window.draw(enemy);
            }

            // Rysowanie gracza
            player.update();
            window.draw(player);

            // Rysowanie sprite'a pocisku
            if (isProjectileActive)
            {
                window.draw(spear);
            }

            // Sprawdzenie kolizji z prostokątem
            for (auto it = enemies.begin(); it != enemies.end(); )
            {
                auto& enemy = *it;

                if (enemy.getGlobalBounds().intersects(spear.getGlobalBounds()))
                {
                    if (enemy.getTier() > 1) {
                        sf::Vector2f newVelocity1 = enemy.getVelocity();
                    
                        newVelocity1.y = std::min(-std::abs(newVelocity1.y * 0.7), -0.1);
                        newVelocity1.x = newVelocity1.x * 1.3f;
                        std::cout << "New velocity: " << newVelocity1.y << std::endl;
                        sf::Vector2f newVelocity2 = enemy.getVelocity();
                        newVelocity2.y = std::min(-std::abs(newVelocity1.y * 0.7), -0.1);
                        newVelocity2.x = -newVelocity2.x * 1.3f;

                        sf::Vector2f newPosition1 = enemy.getPosition();
                        sf::Vector2f newPosition2 = enemy.getPosition();
                        newPosition1.x -= 15.0f;
                        newPosition2.x += 15.0f;

                        isProjectileActive = false;
                        spear.setPosition(player.getPosition());

                        // Utworz dwa nowe obiekty CircleEnemy
                        enemies.emplace_back(CircleEnemy(enemy.getRadius() * 3.0f / 4.0f, newPosition2, enemy.getFillColor(), newVelocity1, enemy.getTier() - 1));
                        enemies.emplace_back(CircleEnemy(enemy.getRadius() * 3.0f / 4.0f, newPosition1, enemy.getFillColor(), newVelocity2, enemy.getTier() - 1));
                    }

                    it = enemies.erase(it);
                    if (enemies.empty()) {
                        addRandomEnemy(enemies, window, 1);
                        addRandomEnemy(enemies, window, 2);
                    }
                    score++;
                    if (score % 7 == 6) {
                        addRandomEnemy(enemies, window, 3);
                    }
                    if (score % 4 == 3) {
                        addRandomEnemy(enemies, window, 1);
                    }
                }
                else if (enemy.getGlobalBounds().intersects(player.getGlobalBounds()))
                {

                    it = enemies.erase(it);
                    gameState = 1;
                }
                else
                {
                    it++;
                }
            }

            // Aktualizuj punkty
            scoreText.setString("Score: " + std::to_string(score));

            // Narysuj tekst punktów
            window.draw(scoreText);

        }
        else if (gameState == 3)
        {
            
            sf::Text gameMenuText;
            gameMenuText.setFont(font);
            gameMenuText.setCharacterSize(48);
            gameMenuText.setFillColor(sf::Color::Black);
            gameMenuText.setString("Press Space to start\n Walking - arrows\n Shoot - Space");
            gameMenuText.setPosition(window.getSize().x / 2 - gameMenuText.getGlobalBounds().width / 2, window.getSize().y / 2 - gameMenuText.getGlobalBounds().height / 2);
            window.draw(gameMenuText);
        }
        window.display();
    }

    return 0;
}
