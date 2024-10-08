#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Bullet.h"
#include "level.h"

class Player {
public:
    // Конструктор
    Player(const std::string& textureFile, Level& level);

    // Основные переменные игрока
    float x, y;               // Позиция игрока
    float dx, dy;             // Скорость по осям
    float speed;              // Скорость игрока
    int w, h;                 // Размеры игрока
    int dir;                  // Направление движения (0 - вправо, 1 - влево, 2 - вниз, 3 - вверх)
    bool isMoving;            // Флаг движения
    bool isOnGround;          // Флаг нахождения на земле

    // Текстура и спрайт игрока
    sf::Texture texture;
    sf::Sprite sprite;

    // Список пуль
    std::vector<Bullet> bullets;

    // Функции игрока
    void update(float time);                // Обновление состояния игрока
    void updateBullets(float time);          // Обновление состояния пуль
    void shoot();                           // Стрельба
    sf::FloatRect getRect() const;          // Получение границ игрока
    int GetWidth() const;
    int GetHeight() const;

private:
    // Ссылка на уровень для проверки коллизий и взаимодействия
    Level& level;

    int lastHorizontalDir;                  // Последнее горизонтальное направление (0 - вправо, 1 - влево)

    // Проверка столкновений с картой
    void checkCollisionWithMap(float dx, float dy);
};
