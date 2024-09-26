#pragma once

#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml2.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

class Object {
public:
    Object(float x, float y, float width, float height)
        : rect(x, y, width, height) {}

    int GetPropertyInt(const std::string& name);
    float GetPropertyFloat(const std::string& name);
    std::string GetPropertyString(const std::string& name);

    std::string name;
    std::string type;
    std::map<std::string, std::string> properties;
    sf::FloatRect rect;
};

class TileMap : public sf::Drawable {
public:
    ~TileMap();

    bool load(const std::string& tmx_file_path);
    Object getObject(const std::string& name);
    std::vector<Object> getObjectsByName(const std::string& name);
    std::vector<Object> getObjectsByType(const std::string& type);
    std::vector<Object>& getAllObjects();

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Texture* texture = nullptr;
    std::vector<sf::VertexArray> tile_layers;
    std::vector<Object> objects;
};
