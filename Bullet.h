#ifndef BULLET_H
#define BULLET_H

#include "SFML/Graphics.hpp"
#include <vector>
#include <iostream>

class Bullet {
public:
    Bullet(float X, float Y, float Dx, float Dy);  // Конструктор

    void update(float time, const std::vector<sf::FloatRect>& solidObjects);  // Обновление состояния пули

    bool isAlive() const { return alive; }  // Статус пули (жива или уничтожена)
    const sf::Sprite& getSprite() const { return sprite; }  // Возвращает спрайт пули

private:
    float x, y;         // Координаты пули
    float dx, dy;       // Направление пули
    float lifetime;     // Время жизни пули
    bool alive;         // Флаг активности пули
    sf::Sprite sprite;  // Спрайт пули
    sf::Texture texture; // Текстура пули

    void checkCollisionWithMap(const std::vector<sf::FloatRect>& solidObjects);  // Проверка столкновений с объектами карты
};

#endif
