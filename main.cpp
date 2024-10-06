#include "Player.h"
#include "level.h"
#include <SFML/Graphics.hpp>
#include <iostream>

// Функция обработки ввода
void handleInput(Player& player, float moveSpeed, bool& isOnGround, float jumpSpeed, float& verticalSpeed) {
    player.speed = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        player.dir = 1;  // Движение влево
        player.speed = moveSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
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
    // Применение гравитации
    if (!isOnGround) {
        verticalSpeed += gravity * time;
        player.dy = verticalSpeed;
    }

    // Проверяем, находится ли игрок на земле
    if (player.dy == 0) {
        isOnGround = true;
        verticalSpeed = 0;  // Останавливаем вертикальное движение
    } else {
        isOnGround = false;  // Если dy не 0, игрок в воздухе
    }
}

// Функция отрисовки
void render(sf::RenderWindow& window, TiledMap& tiledMap, Player& player) {
    // Очищаем экран
    window.clear();

    // Рисуем карту
    tiledMap.draw(window);

    // Рисуем игрока
    window.draw(player.sprite);

    // Рисуем все пули
    for (const auto& bullet : player.bullets) {
        if (bullet.isAlive()) {  // Проверяем, активна ли пуля перед её рисованием
            window.draw(bullet.getSprite());
        }
    }

    // Отображаем кадр
    window.display();
}

int main() {
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Biker of Death");

    // Создаем и загружаем карту уровня
    TiledMap tiledMap("biker_of_death.tmx", window);
    std::cout << "Загрузка уровня...\n";
    tiledMap.loadMap();

    // Создаем объект игрока и задаем начальные координаты
    Player player("biker.png", "biker_of_death.tmx");
    player.x = 100;  // Начальные координаты игрока
    player.y = 100;
    player.sprite.setPosition(player.x, player.y);  // Задаем позицию спрайта игрока

    // Основные игровые параметры
    sf::Clock clock;
    bool isOnGround = false;
    float jumpSpeed = 0.35f;
    float gravity = 0.0005f;
    float verticalSpeed = 0.0f;
    const float moveSpeed = 0.1f;  // Скорость движения игрока

    // Основной игровой цикл
    while (window.isOpen()) {
        // Рассчитываем время, прошедшее с прошлого кадра
        float time = clock.getElapsedTime().asSeconds();  // Время в секундах для стабильности
        clock.restart();

        // Обрабатываем события окна
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Обработка ввода
        handleInput(player, moveSpeed, isOnGround, jumpSpeed, verticalSpeed);

        // Применение гравитации
        applyGravity(player, isOnGround, verticalSpeed, gravity, time);

        // Обновляем положение игрока
        player.update(time, 1280, 800);

        // Обновляем пули (движение и столкновения)
        player.updateBullets(time);

        // Отрисовка
        render(window, tiledMap, player);
    }

    return 0;
}
