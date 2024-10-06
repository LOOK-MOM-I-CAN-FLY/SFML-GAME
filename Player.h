#ifndef PLAYER_H
#define PLAYER_H

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include <vector>
#include <iostream>
#include "Bullet.h"
#include "tinyxml2.h"
#include "level.h"


class Player {
public:
    Player(const std::string& textureFile, const std::string& tmxFile);  // Конструктор с загрузкой текстуры и карты
    void update(float time, int mapWidth, int mapHeight);
    float getPlayerCoordinateX() const;
    float getPlayerCoordinateY() const;
    void shoot();
    void updateBullets(float time);
    sf::FloatRect getRect() const;
    void checkCollisionWithMap(float dx, float dy);


    std::vector<Bullet> bullets;  // Массив пуль

    float x, y, w, h;  // Координаты и размеры игрока
    float dx, dy;  // Изменения координат
    float speed;  // Скорость игрока
    int lastHorizontalDir;  // Последнее горизонтальное направление движения
    bool isMoving;  // Флаг для проверки движения игрока
    int dir;  // Текущее направление движения
    bool isOnGround = false;
    
    sf::Texture texture;  // Текстура игрока
    sf::Sprite sprite;  // Спрайт игрока
    std::string textureFile;  // Имя файла текстуры

    std::vector<sf::FloatRect> solidObjects;  // Вектор объектов для коллизий
    TiledMap* tiledMap;
    void loadMap(const std::string& tmxFile);  // Метод загрузки карты
    void initializePlayerPosition(const tinyxml2::XMLElement* playerObject);  // Инициализация позиции игрока
};

#endif
