#include "Player.h"
#include "level.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Функция обработки ввода
void handleInput(Player& player, float moveSpeed, bool& isOnGround, float jumpSpeed, float& verticalSpeed) {
    player.speed = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.dir = 1;  // Движение влево
        player.speed = -moveSpeed;  // Отрицательная скорость для движения влево
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        player.dir = 0;  // Движение вправо
        player.speed = moveSpeed;
    }

    // Обработка прыжка
    if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        verticalSpeed = -jumpSpeed;  // Начальная скорость прыжка
        isOnGround = false;
    }
}

// Функция применения гравитации
void applyGravity(Player& player, bool& isOnGround, float& verticalSpeed, float gravity, float time) {
    if (!isOnGround) {
        verticalSpeed += gravity * time;
        player.dy = verticalSpeed;
    }

    if (player.dy == 0) {
        isOnGround = true;
        verticalSpeed = 0;  // Останавливаем вертикальное движение
    } else {
        isOnGround = false;
    }
}

// Функция отрисовки
void render(sf::RenderWindow& window, Level& level, Player& player) {
    window.clear();

    // Рисуем карту уровня
    level.Draw(window);

    // Рисуем игрока
    window.draw(player.sprite);

    // Рисуем пули
    for (const auto& bullet : player.bullets) {
        if (bullet.isAlive()) {
            window.draw(bullet.getSprite());
        }
    }

    window.display();
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Biker of Death");

    // Загружаем карту уровня из JSON
    Level level;
    if (!level.LoadFromFile("biker_of_death.json")) {
        std::cerr << "Ошибка загрузки карты!\n";
        return -1;
    }

    // Создаем объект игрока и задаем начальные координаты
    Player player("biker.png", level);
    player.x = 100;
    player.y = 100;
    player.sprite.setPosition(player.x, player.y);

    sf::Clock clock;
    bool isOnGround = false;
    float jumpSpeed = 0.35f;
    float gravity = 0.0005f;
    float verticalSpeed = 0.0f;
    const float moveSpeed = 0.1f;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        handleInput(player, moveSpeed, isOnGround, jumpSpeed, verticalSpeed);
        applyGravity(player, isOnGround, verticalSpeed, gravity, time);
        player.update(time);
        player.updateBullets(time);

        render(window, level, player);
    }

    return 0;
}
