#include "Player.h"
#include "level.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Создаем окно
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Biker of Death");

    // Загружаем карту уровня
    Level level;
    std::string mapFile = "bikerMAP.tmx";
    std::cout << "Загрузка уровня...\n";
    if (!level.loadFromFile("bikerMAP.tmx")) {
        std::cout << "Ошибка загрузки карты!\n";
    }


    std::cout << "Уровень загружен\n";
    // Получаем объект игрока с карты
    Object playerStart = level.getObject("Player");
    if (playerStart.name.empty()) {
        std::cerr << "Error: Player start object not found in map!" << std::endl;
        return -1;
    }

    // Создаем объект игрока и задаем начальные координаты
    Player player("biker.png", mapFile);
    player.x = playerStart.rect.left;
    player.y = playerStart.rect.top;
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
        float time = clock.getElapsedTime().asMicroseconds() / 500.0f;
        clock.restart();

        // Обрабатываем события окна
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Управление движением игрока
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

        // Применение гравитации
        if (!isOnGround) {
            verticalSpeed += gravity * time;
            player.dy = verticalSpeed;
        }

        // Обновляем положение игрока и проверяем столкновения с картой
        player.update(time, 1280, 800);

        // Проверяем, находится ли игрок на земле
        if (player.dy == 0) {
            isOnGround = true;
            verticalSpeed = 0;  // Останавливаем вертикальное движение
        } else {
            isOnGround = false;  // Если dy не 0, игрок в воздухе
        }

        // Стрельба пулями с задержкой
        static sf::Clock shootClock;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootClock.getElapsedTime().asMilliseconds() > 200) {
            player.shoot();
            shootClock.restart();
        }

        // Обновляем пули (движение и столкновения)
        player.updateBullets(time);

        // Очищаем экран
        window.clear();

        // Рисуем карту 
        window.draw(level);

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

    return 0;
}
