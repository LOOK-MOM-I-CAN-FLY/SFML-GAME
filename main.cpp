#include "Player.h"
#include "Monster.h"
#include "level.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "view.h"
#include <map>
#include <vector>
#include "Bonus.h"
#include "Menu.h"


void render(sf::RenderWindow& window, Level& level, Player& player, std::vector<Monster>& allmons, std::vector<Bonus>& allbonuses) {
    window.setView(view); 
    window.clear();


    level.Draw(window);

  ///healtbars
                sf::RectangleShape hpbar_back(sf::Vector2f(120.f, 16.f)); // Size: 200x100 pixels
                sf::RectangleShape hpbar(sf::Vector2f(118.f, 12.f)); // Size: 200x100 pixels
                        hpbar_back.setFillColor(sf::Color::Black);                  // Fill color
                        hpbar.setFillColor(sf::Color::Green);                  // Fill color   
                  


 
                   

                    

    if (player.isAlive) {
        window.draw(player.sprite);


                    hpbar_back.setPosition(player.x, player.y+70);     
                    hpbar.setPosition(player.x+2, player.y+72);   
                    hpbar.setSize( sf::Vector2f(  118.0f * (player.stats["hp"]/player.stats["hp_max"]) , 16.f));
        window.draw(hpbar_back); // Draw the rectangle
        window.draw(hpbar); // Draw the rectangle



            hpbar.setFillColor(sf::Color::Yellow);  
            hpbar.setSize( sf::Vector2f(  118.0f * (player.stats["shield"]/player.stats["shield_max"]) , 16.f));
            hpbar_back.setPosition(player.x, player.y+70+16);     
            hpbar.setPosition(player.x+2, player.y+72+16);   
        window.draw(hpbar_back); // Draw the rectangle
        window.draw(hpbar); // Draw the rectangle
  }
     ;
    /*hpbar.setOutlineColor(sf::Color::Green);                // Outline color
    hpbar.setOutlineThickness(3.f);                      // Outline thickness*/

    sf::CircleShape circle(6.f); 
    circle.setFillColor(sf::Color::Yellow);           
    circle.setOutlineColor(sf::Color(255, 165, 0));  
    circle.setOutlineThickness(2.f);

    sf::CircleShape circleMonster(6.f); 
    circleMonster.setFillColor(sf::Color::Red);           
    circleMonster.setOutlineColor(sf::Color(255, 165, 0));   
    circleMonster.setOutlineThickness(2.f);                  



     if (player.isAlive) { 
    for (  auto& bullet : player.bullets) {
        if (bullet._isAlive()) {
            //window.draw(bullet.getSprite());
            circle.setPosition(bullet.x, bullet.y);     
            window.draw(circle); // Draw the rectangle
             
        }
    }
    }


    for (  auto& bonus : allbonuses) {
            if (!bonus._isAlive()) { continue ;}              
            window.draw(bonus.getSprite());            
            
        }
     
    


    for (auto& monster : allmons) {
                if (monster._isAlive()) { 
                for (  auto& bullet : monster.bullets) {
                if (bullet._isAlive()) {
                    //window.draw(bullet.getSprite());
                    circleMonster.setPosition(bullet.x, bullet.y);     
                    window.draw(circleMonster); // Draw the rectangle
                    
                }
            }
                }

            
        if (monster._isAlive()) {
            window.draw(monster.getSprite());

                hpbar_back.setPosition(monster.x, monster.y+monster.h*1.05+4);     
                hpbar.setPosition(monster.x+2, monster.y+monster.h*1.05+2 );   
                hpbar.setFillColor(sf::Color::Blue);  
                hpbar.setSize( sf::Vector2f(  std::max( 3.0f,  monster.w*0.85f*(monster.stats["hp"]/monster.stats["hp_max"] )    ) , 16.f));
                hpbar_back.setSize( sf::Vector2f(  monster.w*0.85+4 , 16.f));
                //hpbar.setSize( sf::Vector2f(  monster.w*0.85  , 14.f));


    window.draw(hpbar_back); // Draw the rectangle
    window.draw(hpbar); // Draw the rectangle
        }
    }

    window.display();
}



int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 800), "Biker of Death");


    Menu menu(window, "images/IMBA.png"); 


    menu.run();


    if (!menu.startNewGame) {
        return 0; 
    }


    Level level;
    

    if (!level.LoadFromFile("bikerMap.tmj")) {
        std::cerr << "Ошибка загрузки карты!\n";
        return -1;
    }


    

    Player player("biker.png", level);  player.x = 100; player.y = 100; //player.sprite.setPosition(player.x, player.y);
    

   

    std::vector<sf::FloatRect> monsCollisionRects;
    std::vector<Monster> allmons;
    int monallowed = 30;
    int ii=0;
    for (   const auto& obj : level.GetObjects("spawnerSkelet") )               {    if (monallowed--<=0 ){break;} ii++; std::cout<<1001;      allmons.push_back(  * new Monster("skelet.png",   level,    obj.rect.getPosition().x, (int) obj.rect.getPosition().y ,   64 * true,   64 * true ,        1, player )  );             }
    for (   const auto& obj : level.GetObjects("spawnerSkeletBarrel") )         {    if (monallowed--<=0 ){break;} ii++; std::cout<<1002;   allmons.push_back(  * new Monster("skelet.png",   level,    obj.rect.getPosition().x, (int) obj.rect.getPosition().y ,       64 * true,   64 * true  ,         2, player )  );             }
    for (   const auto& obj : level.GetObjects("spawnerHimer") )                {    if (monallowed--<=0 ){break;} ii++; std::cout<<1002;   allmons.push_back(  * new Monster("andro.png",   level,    obj.rect.getPosition().x, (int) obj.rect.getPosition().y ,       57 * true,   88 * true  ,         3, player )  );             }
    for (   const auto& obj : level.GetObjects("spawnerZombie") )               {    if (monallowed--<=0 ){break;}  ii++; std::cout<<"H,4"; allmons.push_back(  * new Monster("zozombie.png",   level,    obj.rect.getPosition().x, (int) obj.rect.getPosition().y ,        64 * true,   64 * true  ,           4, player )  );             }
    for (   const auto& obj : level.GetObjects("spawnerBoss") )               {    if (monallowed--<=0 ){break;}  ii++; std::cout<<"H,4"; allmons.push_back(  * new Monster("boss.png",   level,    obj.rect.getPosition().x, (int) obj.rect.getPosition().y ,        200 * true,   256 * true  ,           5, player )  );             }
    std::cout<<"   = "<<ii; 

     // init bonuses   vector of class BOnus from Bonus.h
      std::vector<Bonus> allbonuses;
 
     int i=-5050000;
  
      ++i;  allbonuses.push_back( * new Bonus("bonus.png", level, player.x + 300+i * 40, player.y-20, 20, 20, 600+i, "bullet_damage",    0, 2)  );
      ++i;  allbonuses.push_back( * new Bonus("bonus.png", level, player.x + 300+i * 40, player.y-20, 20, 20, 600+i, "hp_regen",          0, 4.5)  );
      ++i;  allbonuses.push_back( * new Bonus("armourbonus.png", level, player.x + 300+i * 40, player.y-20, 20, 20, 600+i, "shield_regen",      0, 4.5)  );
      ++i;  allbonuses.push_back( * new Bonus("armourbonus.png", level, player.x + 300+i * 40, player.y-20, 20, 20, 600+i, "shield_max",        15,2)  ); 
       //++i;  allbonuses.push_back( * new Bonus("bonus.png", level, player.x + 300+i * 40, player.y-20, 20, 20, 600+i, "hp_max", 20, 3)  );
         
    ii=   i;
    for (   const auto& obj : level.GetObjects("bonusDamage") )         {     ii++;   std::cout<<ii<<"Bonus";       allbonuses.push_back(  * new Bonus("bonus.png", level,          obj.rect.getPosition().y, obj.rect.getPosition().y, 20, 20, 600+i, "bullet_damage",           0, 2)   );             }
    for (   const auto& obj : level.GetObjects("bonusArmour") )         {     ii++;   std::cout<<ii<<"Bonus";       allbonuses.push_back(  * new Bonus("armourbonus.png", level,    obj.rect.getPosition().y, obj.rect.getPosition().y, 20, 20, 600+i, "shield_max",        15,2)   );             }
    for (   const auto& obj : level.GetObjects("bonus__hp_max") )       {     ii++;   std::cout<<ii<<"Bonus";       allbonuses.push_back(  * new  Bonus("bonus.png", level,         obj.rect.getPosition().y, obj.rect.getPosition().y, 20, 20, 600+i, "hp",               50, 1.30)  );             }
    for (   const auto& obj : level.GetObjects("bonusReload") )         {     ii++;   std::cout<<ii<<"Bonus";       allbonuses.push_back(  * new  Bonus("bonus.png", level,             obj.rect.getPosition().y, obj.rect.getPosition().y, 20, 20, 600+i, "recoil_max",             -1, 0.5)  );             }
    

    char c; //std::cin>>c;    
    std::cout<<'H,4'; 
   
    view.setSize(1280, 800); 
    view.setCenter(player.x, player.y); 

    sf::Clock clock;
     
    int clip = 0;
    while (window.isOpen()) {
        //std::cout<<"Frame:"<<clip++;
        //   std::cin>>c;  
    

        float time = clock.getElapsedTime().asSeconds(); clock.restart();
        sf::Event event;  while (window.pollEvent(event)) { if (event.type == sf::Event::Closed) {  window.close(); } }

        player.update(time*100,allmons, allbonuses );//player.updateBullets(time*100);

        for (auto& monster : allmons) {
            if ( monster._isAlive() )  {  
                monster.update(100*time); 
            }
        }

        for (const auto& monster : allmons) {
            if (monster.the_id == 5 && !monster._isAlive()) {
                std::cout << "Boss defeated. You won!" << std::endl;
                window.close();
                break;
            }
        }


        updateViewForPlayer(player.x, player.y);
        render(window, level, player , allmons , allbonuses);

         
        
         
    }

    return 0;
}

 
