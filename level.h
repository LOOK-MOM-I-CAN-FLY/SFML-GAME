#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

class Object {
public:
    std::string name;
    std::string type;
    sf::IntRect rect;
    std::map<std::string, std::string> properties;
    sf::Sprite sprite;

    int GetPropertyInt(const std::string& name);
    float GetPropertyFloat(const std::string& name);
    std::string GetPropertyString(const std::string& name);
};

class Layer {
public:
    float opacity;
    std::vector<sf::Sprite> tiles;
};

class Level {
public:
    // Методы загрузки карты
    bool LoadFromFile(const std::string& filename);

    // Получение объектов по имени
    Object GetObject(const std::string& name);
    std::vector<Object> GetObjects(const std::string& name);

    // Получение размера тайлов
    sf::Vector2i GetTileSize();

    // Отрисовка карты
    void Draw(sf::RenderWindow& window);
    int GetWidth() const;
    int GetHeight() const;


private:
    int width, height;          // Ширина и высота карты в тайлах
    int tileWidth, tileHeight;  // Размеры тайлов

    sf::Texture tilesetImage;   // Текстура для тайлов

    std::vector<Object> objects;   // Все объекты на карте
    std::vector<Layer> layers;     // Слои карты
};

#endif // LEVEL_H
