#include "Bullet.h"

Bullet::Bullet(float X, float Y, float Dx, float Dy, int Dmgpower, char Owner,Player& The_player)
    : x(X), y(Y), dx(Dx), dy(Dy), lifetime(5000), isAlive(true), dmgpower(Dmgpower),the_player(The_player)  {  // Инициализация через список
    if (!texture.loadFromFile("images/zombie.png")) {
        std::cerr << "Error: Failed to load bullet texture!" << std::endl;
        int cc; std::cin>>cc;
        isAlive = false;
        owner = Owner;
        return;

    }
  
    sprite.setTexture(texture);
    //sprite.setTextureRect(sf::IntRect(0, 0, 21, 21));  // Размер текстуры пули
    //sprite.setTextureRect(sf::IntRect(32-6, 32-6, 32+6, 32+6));  // Размер текстуры пули

     //sprite.setScale(0.5f, 0.5f);  // Масштабирование
    w = 10;
    h = 10;  
    sprite.setPosition(x, y);
}

void Bullet::update(float time, const std::vector<sf::FloatRect>& solidObjects) {
    if (!isAlive) return;  // Если пуля мертва, не обновляем

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
        isAlive = false;
    }
}

void Bullet::checkCollisionWithMap(const std::vector<sf::FloatRect>& solidObjects) {
    sf::FloatRect bulletRect(x, y, w,  h);
    //std::cout<<sprite.getGlobalBounds().width<<" : "<<sprite.getGlobalBounds().height;

    for (const auto& solid : solidObjects) {
        if (bulletRect.intersects(solid)) {
            // Если пуля пересекает solid объект, она уничтожается
            isAlive = false;
            std::cout<<"bullet destroyed  against "<<"bullet XY"<<x<<":"<<y<<" solidXY: "<<x<<":"<<y<<""<<std::endl;
            return;
        }

            if (bulletRect.intersects(solid)) {
            // Если пуля пересекает solid объект, она уничтожается
            isAlive = false;
            
            return;
        }


    } 
}


/*
void Bullet::checkCollisionWithMonster(const std::vector<sf::FloatRect>& monsterObjects) {
    sf::FloatRect bulletRect(x, y, sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);

    for (  auto& mon : monsterObjects) {
         
        sf::FloatRect monRect(x, y, mon.sprite.getGlobalBounds().width, mon.sprite.getGlobalBounds().height);
            if (bulletRect.intersects(mon)) {
            // Если пуля пересекает solid объект, она уничтожается
            isAlive = false;
            mon.isAlive = (mon.hp>0);
            return;
        }


    }
}
*/