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

    // Гравитация всегда действует, если не на земле
    const float gravity = 0.0005f;
    if (!isOnGround) {
        dy += gravity * time;
    } else {
        dy = 0; // Обнуляем вертикальную скорость, когда на земле
    }

    // Управление движением
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        speed = 0.1f;
        dx = speed;
        dir = 0;  // Вправо
        isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        speed = 0.1f;
        dx = -speed;
        dir = 1;  // Влево
        isMoving = true;
    } else {
        dx = 0;  // Если не двигается, обнуляем скорость
    }

    // Прыжок только если на земле
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
        dy = -0.35f;  // Скорость прыжка вверх
        isOnGround = false;  // Во время прыжка не на земле
    }

    // Диагностика
    std::cout << "Player position: (" << x << ", " << y << ")"
              << " Speed: (" << dx << ", " << dy << ")" << " isOnGround: " << isOnGround << std::endl;

    // Обновление позиции
    x += dx * time;
    checkCollisionWithMap(dx, 0);  // Проверяем горизонтальные коллизии

    y += dy * time;
    checkCollisionWithMap(0, dy);  // Проверяем вертикальные коллизии

    // Ограничение игрока рамками карты
    int mapWidth = level.GetTileSize().x * level.GetWidth();
    int mapHeight = level.GetTileSize().y * level.GetHeight();

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > mapWidth) x = mapWidth - w;
    if (y + h > mapHeight) y = mapHeight - h;

    sprite.setPosition(x, y);  // Обновляем позицию спрайта
}

void Player::shoot() {
    float bulletSpeed = 0.3f;
    float bulletDx = (dir == 0) ? bulletSpeed : -bulletSpeed;  // Направление пули в зависимости от направления игрока

    Bullet newBullet(x + w / 2, y + h / 2, bulletDx, 0);
    bullets.push_back(newBullet);
}

void Player::updateBullets(float time) {
    std::vector<sf::FloatRect> collisionRects;
    for (const auto& obj : level.GetObjects("collision")) {
        collisionRects.push_back(sf::FloatRect(obj.rect));  // Преобразуем объекты карты
    }

    for (auto& bullet : bullets) {
        bullet.update(time, collisionRects);
    }

    // Удаляем неактивные пули
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.isAlive(); }),
        bullets.end()
    );
}

sf::FloatRect Player::getRect() const {
    return sf::FloatRect(x, y, w, h);
}

void Player::checkCollisionWithMap(float dx, float dy) {
    for (const auto& object : level.GetObjects("collision")) {
        sf::FloatRect playerBounds(x + dx, y + dy, w, h);

        if (playerBounds.intersects(sf::FloatRect(object.rect))) {
            if (dy > 0) {  // Падение вниз
                y = object.rect.top - h;
                dy = 0;
                isOnGround = true;  // На земле
            }
            if (dy < 0) {  // Прыжок вверх
                y = object.rect.top + object.rect.height;
                dy = 0;
            }
            if (dx > 0) {  // Движение вправо
                x = object.rect.left - w;
            }
            if (dx < 0) {  // Движение влево
                x = object.rect.left + object.rect.width;
            }
        }
    }
}
