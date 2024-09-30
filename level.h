#pragma once

#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

class Object {
public:
    Object(float x = 0, float y = 0, float width = 0, float height = 0) 
        : rect(x, y, width, height) {}

    int         GetPropertyInt(const std::string& name) const;
    float       GetPropertyFloat(const std::string& name) const;
    std::string GetPropertyString(const std::string& name) const;

    std::string name;
    std::string type;
    sf::FloatRect rect;
    std::map<std::string, std::string> properties;
    sf::Sprite sprite;
};

class Level : public sf::Drawable {
public:
    bool loadFromFile(const std::string& filename); 

    Object               getObject(const std::string& name) const;
    std::vector<Object>  getObjectsByName(const std::string& name) const;
    std::vector<Object>  getObjectsByType(const std::string& type) const;
    const std::vector<Object>& getAllObjects() const;
    sf::Vector2i getTileSize() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    int width = 0, height = 0, tileWidth = 0, tileHeight = 0;
    int firstTileID = 0;
    sf::Texture tilesetTexture;
    std::vector<sf::VertexArray> tileLayers;
    std::vector<Object> objects;
};
