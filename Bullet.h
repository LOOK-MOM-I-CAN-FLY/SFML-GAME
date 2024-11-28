#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>
#include "Player.h"

class Player;

class Bullet {
public:
    Bullet(float X, float Y, float Dx, float Dy, int Dmgpower, char Owner, Player& player);  // Конструктор
    void update(float time, const std::vector<sf::FloatRect>& solidObjects);  // Обновление состояния пули
    void checkCollisionWithMonster(const std::vector<sf::FloatRect>& monsterObjects);
    bool _isAlive() const { return isAlive; }  // Статус   (жива или уничтожена)
    const sf::Sprite& getSprite() const { return sprite; }  // Возвращает спрайт пули

  Player&  the_player;
    char owner;
    float lifetime;     // Время жизни пули
    bool isAlive;         // Флаг активности пули
    int dmgpower;
    float dx, dy;       // Направление пули
    int w, h;  
    float x, y;         // Координаты пули
private: 
   
   
   
    sf::Sprite sprite;  // Спрайт пули
    sf::Texture texture; // Текстура пули

    void checkCollisionWithMap(const std::vector<sf::FloatRect>& solidObjects);  // Проверка столкновений с объектами карты
};

#endif
