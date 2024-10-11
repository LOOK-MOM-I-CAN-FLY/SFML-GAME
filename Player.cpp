#include "Player.h"
#include <iostream>

Player::Player(const std::string& textureFile, Level& level) : level(level) {
    dx = 0; dy = 0;
    speed = 0;
    w = 100;
    h = 64;
    x = 100; // Инициализируем координаты игрока, если они не заданы
    y = 100;
    isOnGround = false;  // Изначально игрок в воздухе

    // Загружаем текстуру
    if (!texture.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}

void Player::update(float time) {
    isMoving = false;

    // Применяем гравитацию, если не на земле
    const float gravity = 0.0005f;
    if (!isOnGround) {
        dy += gravity * time;
    }

    // Управление движением
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx = speed; // Движение вправо
        dir = 0;
        isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dx = -speed; // Движение влево
        dir = 1;
        isMoving = true;
    } else {
        dx = 0; // Остановка, если нет ввода
    }

    // Прыжок только если на земле
    if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        dy = -0.35f; // Прыжок
        isOnGround = false;
    }

    // Обновляем позиции
    x += dx * time;
    checkCollisionWithMap(dx, 0); // Проверяем коллизии по оси X

    y += dy * time;
    checkCollisionWithMap(0, dy); // Проверяем коллизии по оси Y

    // Ограничение игрока рамками карты
    int mapWidth = level.GetTileSize().x * level.GetWidth();
    int mapHeight = level.GetTileSize().y * level.GetHeight();

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > mapWidth) x = mapWidth - w;
    if (y + h > mapHeight) y = mapHeight - h;

    sprite.setPosition(x, y); // Обновляем позицию спрайта

    std::cout << "Player position: (" << x << ", " << y << ")"
              << " Speed: (" << dx << ", " << dy << ")" << " isOnGround: " << isOnGround << std::endl;
}

void Player::checkCollisionWithMap(float Dx, float Dy) {
    isOnGround = false; // По умолчанию считаем, что игрок не на земле

    for (const auto& object : level.GetObjects("solid")) {
        sf::FloatRect playerBounds(x + Dx, y + Dy, w, h);

        if (playerBounds.intersects(sf::FloatRect(object.rect))) {
            // Проверяем коллизии по вертикали (Dy)
            if (Dy > 0) {  // Падение вниз
                y = object.rect.top - h;
                dy = 0;
                isOnGround = true; // Игрок на земле
            } else if (Dy < 0) {  // Прыжок вверх
                y = object.rect.top + object.rect.height;
                dy = 0;
            }

            // Проверяем коллизии по горизонтали (Dx)
            if (Dx > 0) {  // Движение вправо
                x = object.rect.left - w;
            } else if (Dx < 0) {  // Движение влево
                x = object.rect.left + object.rect.width;
            }
        }
    }
}



void Player::shoot() {
    float bulletSpeed = 0.3f;
    float bulletDx = (dir == 0) ? bulletSpeed : -bulletSpeed;  // Направление пули в зависимости от направления игрока

    Bullet newBullet(x + w / 2, y + h / 2, bulletDx, 0);
    bullets.push_back(newBullet);
}

void Player::updateBullets(float time) {
    std::vector<sf::FloatRect> collisionRects;
    for (const auto& obj : level.GetObjects("solid")) {
        collisionRects.push_back(sf::FloatRect(obj.rect));
    }

    // Обновляем пули и проверяем их коллизии
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [&collisionRects, time](Bullet& bullet) {
            bullet.update(time, collisionRects);
            return !bullet.isAlive();  // Удаляем неактивные пули
        }), bullets.end());
}


sf::FloatRect Player::getRect() const {
    return sf::FloatRect(x, y, w, h);
}
