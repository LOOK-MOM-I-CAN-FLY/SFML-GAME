#include "Monster.h"
 
Monster::Monster(const std::string& textureFile, Level& level, float xx, float yy, float weight, float height, int id , Player& The_Player) : level(level),the_player(The_Player) {
    srand(time(NULL));
    isAlive = true;      
    isClose = false;
    stats["gravity"] = 0.035f;
    stats["jumpSpeed"] = -3.35f;
    stats["hp_max"] = 100;
    stats["hp"] = 100;
    stats["hp_regen"] = 0.000036f;
    stats["shield_max"] = 15;
    stats["shield"] = 3;
    stats["shield_regen"] = 0.00008f;
    stats["recoil_max"] = 3986;
    stats["recoil"] = (float) (rand() % ((int) stats["recoil_max"]));
    stats["bullet_damage"] = 14;
    stats["speed"] = 6.0f; // Added speed to the stats map  
    the_id = id;
    speed =   (rand() % 6  + 0.3f) * 2;
    dx =  (rand() % 6-rand() % 6)%1 * speed + 0.3f; 
    dir = (int)(dx<0);
         if(textureFile == "boss.png"){ dir= 1 ; speed = 0; dx = 0;}
    dy = -1;      
    w = weight;
    h = height;
    x = xx; // Инициализируем update  , если они не заданы
    y = yy;
    xstart = xx;
    ystart = yy;
    sprite.setPosition(x, y); // Обновляем позицию спрайта
    isOnGround = false;  // Изначально игрок в воздухе
    gravity = !false * 0.035f;
    jumpSpeed =  !false *-3.35f; 
    std::cout<<spritefile_name;
    spritefile_name =  textureFile;



    // Загружаем текстуру 
    sf::Image im;
    if (!im.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }
    // if(textureFile == "boss.png"){
    //     im.flipHorizontally();
    // }
    texture.loadFromImage(im);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));

    frameTime = 0;
    animationSpeed = 10; // Увеличиваем задержку для более мленной смены кадров
}

void Monster::update(float time) {
    isMoving = false;

   if (stats["recoil"] > 0) { stats["recoil"] -= 1; }
    if (isAlive && stats["hp"] < stats["hp_max"]) { stats["hp"] += stats["hp_regen"]; }
   // if (stats["shield"] < stats["shield_max"]) { stats["shield"] += stats["shield_regen"]; }

 
  // Shooting mechanics
  bool canshoot = true && the_player.isAlive;
    if (stats["recoil"] <= 0 && canshoot) {
         stats["recoil"] = stats["recoil_max"];
        
        
        if (  (std::abs(  (the_player.x+the_player.w/2) - (x+w/2)) + std::abs( (the_player.y+the_player.h/2)-(y+h/2))*3)  < (200+900*("boss.png" == spritefile_name) ) ) {fight_back(); isClose=true;}
        else { shoot(); isClose=false;}
    }

     // Update bullets
    updateBullets(time);

 
    // Применяем гравитацию, если не на земле
      if (!isOnGround) {   
        dy += gravity * time;
     }
    // Управление движением
     
      
 


    // Обновляем позиции
    x += (dx  * 2) * time * (1-0.9*isClose);     
    checkCollisionWithMap(dx, 0); // Проверяем коллизии по оси X

    y += dy * time;     
    checkCollisionWithMap(0, dy); // Проверяем коллизии по оси Y

    //TODO ; placer for standsOn flag reset



    // Ограничение игрока рамками карты
    int mapWidth = level.GetTileSize().x * level.GetWidth();
    int mapHeight = level.GetTileSize().y * level.GetHeight();

    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x + w > mapWidth) {x = mapWidth - w;std::cout<<"ACHTUNG room  Xfall"<<the_id<<"  "<<xstart<<":"<<ystart;  }
    if (y + h > mapHeight){ y = mapHeight - h; std::cout<<"ACHTUNG  room Yfall"<<the_id<<"  "<<xstart<<":"<<ystart; }

    sprite.setPosition(x, y); // Обновляем позицию спрайта

    /*std::cout << "Monster " <<   the_id <<"position: (" << x << ", " << y << ")"
              << " Speed: (" << dx << ", " << dy << ")" << " isOnGround: " << isOnGround <<  
             " width height : (" << w << ", " << h << ")" << " isOnGround: " << isOnGround << "MOsnter sprite   : (" << sprite.getPosition().x << ", " << sprite.getPosition().y << ")" << std::endl;*/
     // Логика анимации
    if (true || std::abs(dx) > 0) { // Если монстр движется
        isMoving = true;
        frameTime += time;
        if (frameTime >= animationSpeed) {
            frameTime = 0;
            //andro 57 x 88
            currentFrame = (currentFrame + 1) % 6; // Переход между 4 кадрами
        }
        sprite.setTextureRect(sf::IntRect(currentFrame * w, 0, w, h));

        // Плавный поворот спрайта в зависимости от направления
        if (dx > 0) {
            sprite.setScale(-1.f, 1.f);  // Отражаем по горизонтали (для движения вправо)
            sprite.setOrigin(w, 0);     // Центр смещается на правую сторону
        } else {
            sprite.setScale(1.f, 1.f);  // Исходное состояние (для движения влево)
            sprite.setOrigin(0.f, 0);   // Центр по умолчанию
        }
    }
}

void Monster::checkCollisionWithMap(float Dx, float Dy) {
   


    for (const auto& object : level.GetObjects("point")) {
        sf::FloatRect monsterBounds(x, y, w, h);
           
        if (  monsterBounds.intersects(sf::FloatRect(object.rect))  ) {
              x-= 3*dx;
              dx *= -1; 
              dir = (int) (dx<0);
              break;
              return;
                
        } 
    } 


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



}  ///f ends



void Monster::shoot() {
     
    float bulletSpeed = 6.3f;
    float bulletDx = (dir == 0) ? bulletSpeed : -bulletSpeed;
    Bullet newBullet(x + w / 2 - 6, y + h / 2 - 6, bulletDx, 0, stats["bullet_damage"] , 'm' , the_player);
    bullets.push_back(newBullet);
     
}


void Monster::fight_back() {
     
             

                
                 float xdif,ydif;
                 xdif = (the_player.x+the_player.w/2) - (x+w/2);
                 ydif = (the_player.y+the_player.h/2) - (y+h/2);

                 
                
                float angleTOTarget = std::atan2(ydif, xdif);
                
                float bulletDx =  10 * std::cos(angleTOTarget);
                float bulletDy = 10 * std::sin(angleTOTarget);
                 


    float bulletSpeed = 2.2f;    
    Bullet newBullet(x + w / 2 - 6, y + h / 2 - 6, bulletDx, bulletDy, stats["bullet_damage"] , 'm' , the_player);
    bullets.push_back(newBullet);
     
}

void Monster::updateBullets(float time) {
     
    //   пули
   
    std::vector<sf::FloatRect> empty ;

    if (isAlive && the_player._isAlive() ) { 
        for (auto& b : bullets) {                if (!b._isAlive()  )    {continue;}
        
 /*
                float sum = b.dx + b.dy;

                float xdif = the_player.x - b.x;
                float ydif = the_player.y - b.y; 
                 
                
                float angleTOTarget = std::atan2(ydif, xdif);
                float newBulletDx,newBulletDy;
                newBulletDx = 0.1 * std::cos(angleTOTarget)  ;
                newBulletDy =  0.1 * std::sin(angleTOTarget);
                b.dx = newBulletDx;
                b.dy = newBulletDy;*/
                            
                b.update(time , empty);                         

            if ( sf::FloatRect(b.x, b.y,b.w, b.h).intersects( sf::FloatRect(the_player.x, the_player.y, the_player.w, the_player.h) ) ){

                    b.isAlive = false;
 
                      the_player.stats["shield"] -= b.dmgpower;
                      if (the_player.stats["shield"]<=0) {

                        the_player.stats["hp"] += the_player.stats["shield"];
                        the_player.stats["shield"] = 0;
                        if (the_player.stats["hp"]<0   && the_player.stats["lives"]>0 ) {the_player.x=the_player.xstart;the_player.y=the_player.ystart; the_player.stats["lives"]-=1; the_player.init_stats_default(); }
                                 
                      
                      }
                    //the_player.stats["hp_regen"]-= 0.00082; 
                    //  the_player.speed*= 0.2;   the_player.dx*= 0.2;   

                    if (the_player.isAlive && the_player.stats["hp"]<=0) {

                       the_player.x = the_player.xstart; 
                       the_player.y = the_player.ystart;
                       the_player.stats["hp"] = 11;
                       the_player.stats["hp_max"] = 100;
                       the_player.stats["shield_max"] = 15;
                       the_player.stats["shield"] = 1;


                    }
                    the_player.isAlive = (bool) (the_player.stats["hp"]>0);    

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


sf::FloatRect Monster::getRect() const {
    return sf::FloatRect(x, y, w, h);
}
