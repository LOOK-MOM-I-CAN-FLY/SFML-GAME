#include "Player.h"
#include "tinyxml2.h"
#include <iostream>

using namespace tinyxml2;

Player::Player(const std::string& textureFile, const std::string& tmxFile) {
    dx = 0; dy = 0; speed = 0;
    this->textureFile = textureFile;
    w = 100;  // Ширина игрока
    h = 64;   // Высота игрока

    if (!texture.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));

    // Загружаем карту и инициализируем объекты
    loadMap(tmxFile);
}

void Player::loadMap(const std::string& tmxFile) {
    XMLDocument doc;
    if (doc.LoadFile(tmxFile.c_str()) != XML_SUCCESS) {
        std::cerr << "Error: Failed to load map file " << tmxFile << std::endl;
        return;
    }

    XMLElement* map = doc.FirstChildElement("map");
    if (!map) {
        std::cerr << "Error: No 'map' element found in the .tmx file." << std::endl;
        return;
    }

    // Проходим по слоям карты
    XMLElement* objectGroup = map->FirstChildElement("objectgroup");
    while (objectGroup) {
        const char* groupName = objectGroup->Attribute("name");

        // Если это группа коллизий
        if (groupName && std::string(groupName) == "collision") {
            XMLElement* object = objectGroup->FirstChildElement("object");
            while (object) {
                const char* objectName = object->Attribute("name");
                if (objectName && std::string(objectName) == "solid") {
                    float x = object->FloatAttribute("x");
                    float y = object->FloatAttribute("y");
                    float width = object->FloatAttribute("width");
                    float height = object->FloatAttribute("height");
                    solidObjects.push_back(sf::FloatRect(x, y, width, height));
                }
                object = object->NextSiblingElement("object");
            }
        }

        objectGroup = objectGroup->NextSiblingElement("objectgroup");
    }
}

void Player::update(float time, int mapWidth, int mapHeight) {
    isMoving = false;

    // Обработка направления
    switch (dir) {
        case 0: dx = speed; dy = 0; lastHorizontalDir = 0; break;  // Вправо
        case 1: dx = -speed; dy = 0; lastHorizontalDir = 1; break; // Влево
        case 2: dx = 0; dy = speed; break;  // Вниз
        case 3: dx = 0; dy = -speed; break; // Вверх
    }

    // Проверка движения
    if (dx != 0 || dy != 0) {
        isMoving = true;
    }

    // Обновление позиции и проверка столкновений
    x += dx * time;
    checkCollisionWithMap(dx, 0);  // Горизонтальные коллизии

    y += dy * time;
    checkCollisionWithMap(0, dy);  // Вертикальные коллизии

    // Ограничение движения в пределах карты
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

    // Создаём новую пулю
    Bullet newBullet(x + w / 2, y + h / 2, bulletDx, 0);
    bullets.push_back(newBullet);
}

void Player::updateBullets(float time) {
    for (auto& bullet : bullets) {
        bullet.update(time, solidObjects);  // Проверяем столкновения с объектами
    }

    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& b) { return !b.isAlive(); }),
        bullets.end()
    );
}

sf::FloatRect Player::getRect() const {
    return sf::FloatRect(x, y, w, h);
}


// Метод для проверки коллизий с картой
void Player::checkCollisionWithMap(float dx, float dy) {
    // Проходим по каждому слою
    for (const auto& layer : tiledMap->layers) {  // Обращаемся к полю layers напрямую
        for (int y = 0; y < layer.height; ++y) {
            for (int x = 0; x < layer.width; ++x) {
                int tileId = layer.tiles[y * layer.width + x];
                if (tileId == 0) continue;  // Пропускаем пустые тайлы

                // Определяем размеры тайла
                const int tileWidth = 32;  // Используй фактический размер тайла с карты
                const int tileHeight = 32;

                // Проверяем коллизию игрока с текущим тайлом
                sf::FloatRect playerBounds(x + dx, y + dy, w, h);
                sf::FloatRect tileBounds(x * tileWidth, y * tileHeight, tileWidth, tileHeight);

                if (playerBounds.intersects(tileBounds)) {
                    // Обработка столкновений
                    if (dy > 0) {
                        y = tileBounds.top - h;  // Игрок приземляется на поверхность
                        dy = 0;
                        isOnGround = true;
                    }
                    if (dy < 0) {
                        y = tileBounds.top + tileHeight;  // Столкновение с потолком
                        dy = 0;
                    }
                    if (dx > 0) {
                        x = tileBounds.left - w;  // Столкновение справа
                    }
                    if (dx < 0) {
                        x = tileBounds.left + tileWidth;  // Столкновение слева
                    }
                }
            }
        }
    }
}
