#include "Player.h"
#include <iostream>



Player::Player(const std::string& textureFile, Level& level) : level(level)  // Инициализируем ссылку на объект уровня
{
    dx = 0; dy = 0; speed = 0;
    w = 100;
    h = 64;

    if (!texture.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}

void Player::update(float time) {
    isMoving = false;

    switch (dir) {
        case 0: dx = speed; dy = 0; lastHorizontalDir = 0; break;
        case 1: dx = -speed; dy = 0; lastHorizontalDir = 1; break;
        case 2: dx = 0; dy = speed; break;
        case 3: dx = 0; dy = -speed; break;
    }

    if (dx != 0 || dy != 0) {
        isMoving = true;
    }

    x += dx * time;
    checkCollisionWithMap(dx, 0);

    y += dy * time;
    checkCollisionWithMap(0, dy);

    int mapWidth = level.GetTileSize().x * level.GetWidth();
    int mapHeight = level.GetTileSize().y * level.GetHeight();

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > mapWidth) x = mapWidth - w;
    if (y + h > mapHeight) y = mapHeight - h;

    speed = 0;
    sprite.setPosition(x, y);
}

void Player::shoot() {
    float bulletSpeed = 0.3f;
    float bulletDx = (lastHorizontalDir == 0) ? bulletSpeed : -bulletSpeed;

    Bullet newBullet(x + w / 2, y + h / 2, bulletDx, 0);
    bullets.push_back(newBullet);
}

void Player::updateBullets(float time) {
    std::vector<sf::FloatRect> collisionRects;
    for (const auto& obj : level.GetObjects("collision")) {
        collisionRects.push_back(sf::FloatRect(obj.rect)); // Преобразование
    }

    for (auto& bullet : bullets) {
        bullet.update(time, collisionRects);
    }

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
            if (dy > 0) {
                y = object.rect.top - h;
                dy = 0;
                isOnGround = true;
            }
            if (dy < 0) {
                y = object.rect.top + object.rect.height;
                dy = 0;
            }
            if (dx > 0) {
                x = object.rect.left - w;
            }
            if (dx < 0) {
                x = object.rect.left + object.rect.width;
            }
        }
    }
}

