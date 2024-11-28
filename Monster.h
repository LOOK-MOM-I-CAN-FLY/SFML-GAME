#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Bullet.h"
#include "level.h"
#include "Player.h"


#include <iostream>
#include <random>
#include <time.h>

class Player;
class Bullet;

class Monster {
public:
    // Конструктор
    Monster(const std::string& textureFile, Level& level, float xx, float yy, float weight, float height,int id, Player& ThePlayer);

    // Основные переменные игрока
    float x, y;               //  игрока
    float dx, dy;             // Скорость по осям
    float gravity,jumpSpeed;             //  GRAVITY =)))
    float speed;              // Скорость игрока
    float w, h;                 // Размеры игрока
    int dir;                  // Направление движения (0 - вправо, 1 - влево, 2 - вниз, 3 - вверх)
    int the_id;
    bool isMoving;            // Флаг движения
    bool isOnGround;          // Флаг нахождения на земле
    bool isAlive;               // Флаг необзодимости обработки будто оно существует. иначе -  считать что уничтожен и ожидает очистки из памяти в след же такте

    std::string spritefile_name ;
    int currentFrame;  // Инициализация текущего кадра
    float animationSpeed; // Скорость анимации (время в секундах между кадрами)
    float frameTime;  // Время с последнего обновления кадра
    int xstart,ystart;
    // Текстура и спрайт игрока
    sf::Texture texture;
    sf::Sprite sprite;

    // Список пуль
    std::vector<Bullet> bullets;
    std::map<std::string, float> stats; 
    Player& the_player;

    // Функции игрока
    void update(float time);                // Обновление состояния игрока
    bool _isAlive() const { return isAlive; }  // Статус   (жива или уничтожена)
    const sf::Sprite& getSprite() const { return sprite; }  // Возвращает спрайт пули
    void updateBullets(float time);          // Обновление состояния пуль
    void shoot();                           // Стрельба
    void fight_back();                           // Стрельба close distance

    sf::FloatRect getRect() const;          // Получение границ игрока
    int GetWidth() const;
    int GetHeight() const;
    bool isClose;
private:
    // Ссылка на уровень для проверки коллизий и взаимодействия
    Level& level;

    int lastHorizontalDir;                  // Последнее горизонтальное направление (0 - вправо, 1 - влево)

    // Проверка столкновений с картой
    void checkCollisionWithMap(float dx, float dy);
};
