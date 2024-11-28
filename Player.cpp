#include "Player.h"
#include <iostream>

Player::Player(const std::string& textureFile, Level& level) : level(level) {
    isAlive = true;
    dx = 0; 
    dy = 0;
    stats["lives"] = 3 + true*10000;
    // Initialize stats map
    // do init stats next line:
   init_stats_default();

    w = 100;
    h = 64;
    x = 100; // Initialize player's coordinates if not set
    y = 100;
    xstart = x;
    ystart = y;

    isOnGround = false; // Player starts in the air

    // Load texture
    if (!texture.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));
}


void Player::init_stats_default(){

    stats["gravity"] = 0.035f;
    stats["jumpSpeed"] = -3.35f;
    stats["hp_max"] = 100;
    stats["hp"] = 25;
    stats["hp_regen"] = 0.0004f;
    stats["shield_max"] = 15;
    stats["shield"] = 3;
    stats["shield_regen"] = 0.00004f;
    stats["recoil"] = 15;
    stats["recoil_max"] = 3396;
    stats["bullet_damage"] = 14;
    stats["speed"] = 4.0f; // Added speed to the stats map

}


void Player::update(float time, std::vector<Monster>& allmons,  std::vector<Bonus>& allbonuses) {
    isMoving = false;

    // Update stats-based mechanics
    if (stats["recoil"] > 0) { stats["recoil"] -= 1; }
    if (isAlive && stats["hp"] < stats["hp_max"]) { stats["hp"] += stats["hp_regen"]; }
    if (stats["shield"] < stats["shield_max"]) { stats["shield"] += stats["shield_regen"]; }

    if (!isOnGround) {   
        dy += stats["gravity"] * time;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        dx = stats["speed"];
        dir = 0;
        isMoving = true;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        dx = -stats["speed"];
        dir = 1;
        isMoving = true;
    } else {
        dx = 0;
    }

    if (isOnGround && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        dy = stats["jumpSpeed"];
        isOnGround = false;
    }

    // Shooting mechanics
    if (stats["recoil"] <= 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        stats["recoil"] = stats["recoil_max"];
        shoot();
    }

    x += dx * time;
    checkCollisionWithMap(dx, 0);

    y += dy * time;
    checkCollisionWithMap(0, dy);


    for (   Bonus& bonus : allbonuses) {
            sf::FloatRect playerBounds(x, y, w, h);

            if (    playerBounds.intersects(  sf::FloatRect(bonus.getRect()  ))     ) { 
                    if (!bonus.isAlive) {continue;} 

            std::cout<<"Bstats["<<bonus.var_name <<"] = "<<stats[bonus.var_name]<<std::endl;  
                stats[bonus.var_name ] += bonus.var_add; 
                stats[bonus.var_name ] *= bonus.var_multiply ;
            std::cout<<"Estats["<<bonus.var_name <<"] = "<<stats[bonus.var_name]<<std::endl;  
                bonus.isAlive = false; 
               std::cout<<"Bonus "<<&bonus<<"   "<<bonus.the_id<<"  name = ["<<bonus.var_name<<"]  +"<<bonus.var_add<<" *"<<bonus.var_multiply<<std::endl;
               
               

                
            }
    }




    // Update bullets
    updateBullets(time, allmons);

    int mapWidth = level.GetTileSize().x * level.GetWidth();
    int mapHeight = level.GetTileSize().y * level.GetHeight();

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > mapWidth) x = mapWidth - w;
    if (y + h > mapHeight) y = mapHeight - h;

    sprite.setPosition(x, y);

    // Animation logic
    float animationSpeed = 8.5f; // Время между кадрами в миллисекундах (изначально)
    if (isMoving) {
        frameTime += time;
        if (frameTime >= animationSpeed) {
            frameTime = 0;
            currentFrame = (currentFrame + 1) % 4;
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));
    } else {
        currentFrame = 0; // Reset to idle frame
        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));
    }

    // Smooth sprite flip for left/right movement
    if (dir == 1) {
        sprite.setScale(-1.f, 1.f); // Flip horizontally for left movement
        sprite.setOrigin(w, 0);    // Set origin to right edge to prevent shifting
    } else {
        sprite.setScale(1.f, 1.f); // Restore default scale for right movement
        sprite.setOrigin(0, 0);    // Set origin to default
    }
}

void Player::checkCollisionWithMap(float Dx, float Dy) {
    isOnGround = false; // Assume player is in the air by default

//Traps
sf::FloatRect playerBounds(x + Dx, y + Dy, w, h);

for (const auto& object : level.GetObjects("trap")) {
        if (playerBounds.intersects(sf::FloatRect(object.rect))) {
            dy= - 6;
            y -= 30; //object.rect.top - h-5;
            
            
            stats["hp"]*=0.2;
            stats["hp"]-=1;
           

             if (stats["lives"]>0 && stats["hp"]<0 ) {x=xstart;y=ystart; stats["lives"]-=1; init_stats_default(); }
        }
}

   
//WAlls SOlids
    for (const auto& object : level.GetObjects("solid")) {
        if (playerBounds.intersects(sf::FloatRect(object.rect))) {
            // Check vertical collisions (Dy)
            if (Dy > 0) {  // Falling down
                y = object.rect.top - h;
                dy = 0;
                isOnGround = true; // Player is on the ground
            } else if (Dy < 0) {  // Jumping up
                y = object.rect.top + object.rect.height;
                dy = 0;
            }

            // Check horizontal collisions (Dx)
            if (Dx > 0) {  // Moving right
                x = object.rect.left - w;
            } else if (Dx < 0) {  // Moving left
                x = object.rect.left + object.rect.width;
            }
        }
    }
}

void Player::shoot() {
    float bulletSpeed = 6.3f;
    float bulletDx = (dir == 0) ? bulletSpeed : -bulletSpeed;

    Bullet newBullet(x + w / 2 - 6, y + h / 2 - 6, bulletDx, 0, stats["bullet_damage"], 'p',   *this );
    bullets.push_back(newBullet);
}

/*void Player::updateBullets(float time, std::vector<Monster>& allmons) {
    std::vector<sf::FloatRect> solidCollisionRects;
    for (const auto& obj : level.GetObjects("solid")) {
        solidCollisionRects.push_back(sf::FloatRect(obj.rect));
    }

    // Update bullets and check their collisions
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [&solidCollisionRects, time](Bullet& bullet) {
            bullet.update(time, solidCollisionRects);
            return !bullet._isAlive();
        }), bullets.end());
}*/

sf::FloatRect Player::getRect() const {
    return sf::FloatRect(x, y, w, h);
}

void Player::updateBullets(float time,std::vector<Monster>& allmons) {
    std::vector<sf::FloatRect> solidCollisionRects;
    for (const auto& obj : level.GetObjects("solid")) {  solidCollisionRects.push_back(sf::FloatRect(obj.rect)); }


 //   пули
    for (auto& b : bullets) {      if (!b._isAlive()  )    {continue;}
         b.update(time, solidCollisionRects);
    for (Monster& m : allmons) {     if (!m._isAlive() )   {continue;}

            if ( sf::FloatRect(b.x, b.y,b.w, b.h).intersects( sf::FloatRect(m.x, m.y, m.w, m.h) ) ){

                    b.isAlive = false;
                     
                    m.stats["hp"] -= b.dmgpower;
                    //m.stats["hp_regen"]-= 0.00082; 
                    //  m.speed*= 0.2;   m.dx*= 0.2;   
                    m.isAlive = (bool) (m.stats["hp"]>0);


                    
            }
            
 
    }
    }


/*
        // Update bullets and check their collisions
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
        [&solidCollisionRects, time](Bullet& bullet) {
            bullet.update(time, solidCollisionRects);
            return !bullet._isAlive();
        }), bullets.end());
*/

    }