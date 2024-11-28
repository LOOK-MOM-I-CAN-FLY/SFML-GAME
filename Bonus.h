#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Bullet.h"
#include "level.h"
#include <iostream>
#include <random>
#include <time.h>


class Bonus {
public:
    // Конструктор
    Bonus(const std::string& textureFile, Level& level, float xx, float yy, float weight, float height,int id, std::string varname, float mod_add, float mod_mltply );

    // Основные переменные игрока
    float x, y;               //  игрока
    std::string var_name;
    float var_add ;
    float var_multiply;
    
    float dx, dy;             // Скорость по осям
    float gravity,jumpSpeed;             //  GRAVITY =)))
    float speed;              // Скорость игрока
    float w, h;                 // Размеры игрока
    int dir;                  // Направление движения (0 - вправо, 1 - влево, 2 - вниз, 3 - вверх)
    int the_id;
    bool isMoving;            // Флаг движения
    bool isOnGround;          // Флаг нахождения на земле
    bool isAlive;               // Флаг необзодимости обработки будто оно существует. иначе -  считать что уничтожен и ожидает очистки из памяти в след же такте
    float hp,hp_max,hp_regen;
    int currentFrame;  // Инициализация текущего кадра
    float animationSpeed; // Скорость анимации (время в секундах между кадрами)
    float frameTime;  // Время с последнего обновления кадра
    int xstart,ystart;
    // Текстура и спрайт игрока
    sf::Texture texture;
    sf::Sprite sprite;

   
    // Функции игрока
    void update(float time);                // Обновление состояния игрока
    bool _isAlive() const { return isAlive; }  // Статус   (жива или уничтожена)
    const sf::Sprite& getSprite() const { return sprite; }  // Возвращает спрайт пули
    
    
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
