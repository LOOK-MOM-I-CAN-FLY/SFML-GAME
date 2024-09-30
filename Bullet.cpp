#include "Bullet.h"

Bullet::Bullet(float X, float Y, float Dx, float Dy) 
    : x(X), y(Y), dx(Dx), dy(Dy), lifetime(5000), alive(true) {  // Инициализация через список
    if (!texture.loadFromFile("images/bullet.png")) {
        std::cerr << "Error: Failed to load bullet texture!" << std::endl;
        alive = false;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 21, 21));  // Размер текстуры пули
    sprite.setScale(0.5f, 0.5f);  // Масштабирование
    sprite.setPosition(x, y);
}

void Bullet::update(float time, const std::vector<sf::FloatRect>& solidObjects) {
    if (!alive) return;  // Если пуля мертва, не обновляем

    // Обновление координат пули
    x += dx * time;
    y += dy * time;

    // Проверка столкновений
    checkCollisionWithMap(solidObjects);

    // Обновление позиции спрайта
    sprite.setPosition(x, y);

    // Уменьшение времени жизни пули
    lifetime -= time;
    if (lifetime <= 0) {
        alive = false;
    }
}

void Bullet::checkCollisionWithMap(const std::vector<sf::FloatRect>& solidObjects) {
    sf::FloatRect bulletRect(x, y, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

    for (const auto& solid : solidObjects) {
        if (bulletRect.intersects(solid)) {
            // Если пуля пересекает solid объект, она уничтожается
            alive = false;
            return;
        }
    }
}
