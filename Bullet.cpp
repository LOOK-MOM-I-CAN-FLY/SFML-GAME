#include "Bullet.h"

Bullet::Bullet(float X, float Y, float Dx, float Dy) {
    x = X;
    y = Y;
    dx = Dx;  // Задаём горизонтальную скорость
    dy = Dy;  // Задаём вертикальную скорость
    lifetime = 5000;  // Время жизни пули 5 секунд
    isAlive = true;


    texture.loadFromFile("images/bullet.png");
    sprite.setTexture(texture);
    sprite.setTextureRect(IntRect(0, 0, 21, 21));  // Определяем область текстуры
    sprite.setScale(0.5, 0.5);  // Масштабируем спрайт

    sprite.setPosition(x, y);
}

void Bullet::update(float time) {
    // Обновляем координаты пули с учётом скорости
    x += dx * time;
    y += dy * time;

    // Обновляем позицию спрайта
    sprite.setPosition(x, y);

    // Уменьшаем время жизни пули
    lifetime -= time;
    if (lifetime <= 0) {
        isAlive = false;
    }
}
