#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Bullet.h"
#include "Bonus.h"
#include "level.h"
#include "Monster.h"
#include <map>

class Monster;
class Bullet;
class Bonus;

class Player {
public:
    // Конструктор
    Player(const std::string& textureFile, Level& level);

    // Основные переменные игрока
    std::map<std::string, float> stats; 
    
    float x, y,xstart,ystart;               //  игрока
    float dx, dy;             // Скорость по осям
    float gravity;
    int w, h;                 // Размеры игрока
    int dir;                  // Направление движения (0 - вправо, 1 - влево, 2 - вниз, 3 - вверх)
    bool isMoving;            // Флаг движения
    bool isOnGround;          // Флаг нахождения на земле
    bool isAlive;         // Флаг активности  

    int currentFrame;  // Инициализация текущего кадра
    float animationSpeed; // Скорость анимации (время в секундах между кадрами)
    float frameTime;  // Время с последнего обновления кадра
    // Текстура и спрайт игрока
    sf::Texture texture;
    sf::Sprite sprite;

    // Список пуль
    std::vector<Bullet> bullets;

    // Функции игрока
    void update(float time,  std::vector<Monster>& allmons, std::vector<Bonus>& allbonuses);                // Обновление состояния игрока
    bool _isAlive() const { return isAlive; }  // Статус   (жива или уничтожена)
    const sf::Sprite& getSprite() const { return sprite; }  // Возвращает спрайт пули

    void updateBullets(float time,std::vector<Monster>& allmons);          // Обновление состояния пуль
    void init_stats_default();
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
