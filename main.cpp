#include "Player.h"
#include "map.h"
#include "view.h"

using namespace sf;

int main() {
    RenderWindow window(VideoMode(640, 480), "Biker of death");
    view.reset(FloatRect(0, 0, 640, 480));

    Image map_image;
    map_image.loadFromFile("images/map.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    Player p("biker.png", 100, 53, 100, 64);

    float CurrentFrame = 0;
    Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 500;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        // Управление игроком
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            p.dir = 1; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(103 * int(CurrentFrame), 0, 103, 54));
            p.sprite.setScale(-1, 1);
            p.sprite.setOrigin(103, 0);
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }

        if (Keyboard::isKeyPressed(Keyboard::D)) {
            p.dir = 0; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(103 * int(CurrentFrame), 0, 103, 54));
            p.sprite.setScale(1, 1);
            p.sprite.setOrigin(0, 0);
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }

        if (Keyboard::isKeyPressed(Keyboard::W)) {
            p.dir = 3; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(103 * int(CurrentFrame), 0, 103, 54));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }

        if (Keyboard::isKeyPressed(Keyboard::S)) {
            p.dir = 2; p.speed = 0.1;
            CurrentFrame += 0.005 * time;
            if (CurrentFrame > 3) CurrentFrame -= 3;
            p.sprite.setTextureRect(IntRect(103 * int(CurrentFrame), 0, 103, 54));
            getplayercoordinateforview(p.getplayercoordinateX(), p.getplayercoordinateY());
        }

        // Стрельба
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            p.shoot();  // Игрок стреляет
        }

        // Обновление игрока и пуль
        p.update(time, 1280, 800);   // Обновление позиции игрока
        p.updateBullets(time);       // Обновление пуль

        viewmap(time);               // Обновление вида камеры
        changeview();
        window.setView(view);
        window.clear();

        // Отрисовка карты
        for (int i = 0; i < HEIGHT_MAP; i++) {
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ')  s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == 's')  s_map.setTextureRect(IntRect(32, 0, 32, 32));
                if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));

                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }
        }

        // Отрисовка игрока
        window.draw(p.sprite);

        // Отрисовка пуль
        for (const auto& bullet : p.bullets) {
            window.draw(bullet.sprite);
        }

        window.display();
    }

    return 0;
}
