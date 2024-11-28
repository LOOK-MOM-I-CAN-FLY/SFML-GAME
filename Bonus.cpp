 
#include "Bonus.h"


Bonus::Bonus(const std::string& textureFile, Level& level, float xx, float yy, float weight, float height, int id, std::string varname, float mod_add, float mod_mltply ) : level(level) {
    srand(time(NULL));
    hp= 60;
    hp_max = 100;
    hp_regen = 0.00352;
    isAlive = true;   
    the_id = id;
    var_name=varname;
    var_add=mod_add ;
    var_multiply=mod_mltply;   
    std::cout<<"bonus"<< the_id<<"Construct"<<var_name<<" - "<<var_add<<" "<<var_multiply<<std::endl;
   
    speed =   (rand() % 6  + 0.3f) * 2;
    dx =  (rand() % 6-rand() % 6)%1 * speed + 0.3f; 

    dy = -1;      
    w = weight;
    h = height;
    x = xx; 
    y = yy;
    xstart = xx;
    ystart = yy;
    sprite.setPosition(x, y); // Обновляем позицию спрайта
    isOnGround = false;  // Изначально игрок в воздухе
    gravity = !false * 0.035f;
    jumpSpeed =  !false *-3.35f; 
    



    // Загружаем текстуру
    if (!texture.loadFromFile("images/" + textureFile)) {
        std::cerr << "Error: Failed to load texture from image!" << std::endl;
        return;
    }

    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, w, h));

    frameTime = 0;
    animationSpeed = 10; // Увеличиваем задержку для более медленной смены кадров
}

void Bonus::update(float time) {
    isMoving = false;


if (hp<hp_max) {hp+= hp_regen;}
    // Применяем гравитацию, если не на земле
      if (!isOnGround) {   
        dy += gravity * time;
     }
    // Управление движением
     
     if ( false && true) {
        dx = -speed; // Движение влево
        dir = 1;
        isMoving = true;
     }

    // Прыжок только если на земле
     
    


    // Обновляем позиции
    x += dx * time;     
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
    if (std::abs(dx) > 0) { // Если монстр движется
        isMoving = true;
        frameTime += time;
        if (frameTime >= animationSpeed) {
            frameTime = 0;
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

void Bonus::checkCollisionWithMap(float Dx, float Dy) {
   


    for (const auto& object : level.GetObjects("point")) {
        sf::FloatRect monsterBounds(x, y, w, h);
           
        if (  monsterBounds.intersects(sf::FloatRect(object.rect))  ) {
              x-= 3*dx;
              dx *= -1; 
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


 
 

sf::FloatRect Bonus::getRect() const {
    return sf::FloatRect(x, y, w, h);
}
