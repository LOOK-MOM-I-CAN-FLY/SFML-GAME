#ifndef PLAYER_H
#define PLAYER_H

#include "/Users/sindi_hall/Downloads/SFML_ROOT/include/SFML/Graphics.hpp"
#include "Bullet.h"
using namespace sf;

class Bullet;

class Player {
private:
    float x, y;

public:
    float w, h, dx, dy, speed;
    int dir = 0;
    int lastHorizontalDir;
    String File;
    Image image;
    Texture texture;
    Sprite sprite;
    bool isMoving;
    std::vector<Bullet> bullets;

    Player(String F, float X, float Y, float W, float H);
    void update(float time, int mapWidth, int mapHeight);
    float getplayercoordinateX();
    float getplayercoordinateY();
    void shoot();
    void updateBullets(float time);
};

#endif
