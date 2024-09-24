#include "Player.h"

Player::Player(String F, float X, float Y, float W, float H) {
    dx = 0; dy = 0; speed = 0;
    lastHorizontalDir = 0; 
    isMoving = false;  
    File = F;
    w = W; h = H;
    image.loadFromFile("images/" + File);
    texture.loadFromImage(image);
    sprite.setTexture(texture);
    x = X; y = Y;
    sprite.setTextureRect(IntRect(0, 0, w, h));
}

void Player::update(float time, int mapWidth, int mapHeight) {
    isMoving = false;
    switch (dir) {
        case 0: 
            dx = speed; dy = 0; 
            lastHorizontalDir = 0;  // Смотрит вправо
            break; 
        case 1: 
            dx = -speed; dy = 0; 
            lastHorizontalDir = 1;  // Смотрит влево
            break; 
        case 2: 
            dx = 0; dy = speed; 
            break; 
        case 3: 
            dx = 0; dy = -speed; 
            break; 
    }

    if (dx != 0 || dy != 0) {
        isMoving = true;
    }

    x += dx * time;
    y += dy * time;

    if (x < 0) x = 0;  
    if (y < 0) y = 0;  
    if (x + w > mapWidth) x = mapWidth - w;
    if (y + h > mapHeight) y = mapHeight - h; 

    speed = 0;
    sprite.setPosition(x, y);
}

float Player::getplayercoordinateX() {
    return x;
}

float Player::getplayercoordinateY() {
    return y;
}

void Player::shoot() {
    float bulletSpeed = 0.3f;  // Задаём фиксированную скорость пули
    float bulletDx = 0;

    // Используем последнее горизонтальное направление
    if (lastHorizontalDir == 0) { // Вправо
        bulletDx = bulletSpeed;
    } else if (lastHorizontalDir == 1) { // Влево
        bulletDx = -bulletSpeed;
    }

    // Создаём новую пулю в направлении последнего горизонтального движения
    Bullet newBullet(x + w / 2, y + h / 2, bulletDx, 0);
    bullets.push_back(newBullet);
}




void Player::updateBullets(float time) {
    // Обновляем каждую пулю
    for (auto& bullet : bullets) {
        bullet.update(time);
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.isAlive; }),
        bullets.end()
    );
}