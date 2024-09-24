#ifndef BULLET_H
#define BULLET_H

#include "/Users/sindi_hall/Downloads/SFML_ROOT/include/SFML/Graphics.hpp"
using namespace sf;

class Bullet {
public:
    float x, y;         // Координаты пули
    float dx, dy;       // Направление пули
    float speed;        // Скорость пули
    float lifetime;     // Время жизни пули
    bool isAlive;       // Статус пули (активна или уничтожена)
    Sprite sprite;      // Спрайт пули
    Texture texture;    // Текстура пули (добавлено)

    Bullet(float X, float Y, float Dx, float Dy);  // Конструктор

    void update(float time);  // Обновление состояния пули
};

#endif
