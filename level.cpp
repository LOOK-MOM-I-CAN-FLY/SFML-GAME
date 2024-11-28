#include "level.h"


// Пространство имен для удобства работы с JSON
using json = nlohmann::json;

int Object::GetPropertyInt(const std::string& name) {
    return std::stoi(properties.at(name));
}

float Object::GetPropertyFloat(const std::string& name) {
    return std::stof(properties.at(name));
}

std::string Object::GetPropertyString(const std::string& name) {
    return properties.at(name);
}

bool Level::LoadFromFile(const std::string& filename) {
    // Загружаем JSON файл
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to load level file: " << filename << std::endl;
        return false;
    }

    json levelData;
    file >> levelData;
    file.close();

    // Читаем основные параметры карты
    width = levelData["width"].get<int>();
    height = levelData["height"].get<int>();
    tileWidth = levelData["tilewidth"].get<int>();
    tileHeight = levelData["tileheight"].get<int>();

    // Загружаем тайлсет
    std::string imagepath = levelData["tilesets"][0]["image"];
    sf::Image img;
    if (!img.loadFromFile(imagepath)) {
        std::cerr << "Failed to load tile sheet: " << imagepath << std::endl;
        return false;
    }

    // Создаем маску для прозрачности (цвет может измениться)
    img.createMaskFromColor(sf::Color(109, 159, 185));
    tilesetImage.loadFromImage(img);
    tilesetImage.setSmooth(false);

    // Получаем количество столбцов и строк в тайлсете
    int columns = tilesetImage.getSize().x / tileWidth;
    int rows = tilesetImage.getSize().y / tileHeight;

    // Разделяем тайлы по прямоугольникам
    std::vector<sf::Rect<int>> subRects;
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < columns; ++x) {
            sf::Rect<int> rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            subRects.push_back(rect);
        }
    }

    // Загружаем слои
    for (const auto& layerData : levelData["layers"]) {
        if (layerData["type"] == "tilelayer") {
            Layer layer;
            layer.opacity = layerData.value("opacity", 1.0f) * 255;

            const auto& tiles = layerData["data"];
            int x = 0, y = 0;

            for (int tileGID : tiles) {
                if (tileGID != 0) {  // 0 означает пустой тайл
                    int subRectToUse = tileGID - levelData["tilesets"][0]["firstgid"].get<int>();
                    sf::Sprite sprite;
                    sprite.setTexture(tilesetImage);
                    sprite.setTextureRect(subRects[subRectToUse]);
                    sprite.setPosition(x * tileWidth, y * tileHeight);
                    sprite.setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(layer.opacity)));

                    layer.tiles.push_back(sprite);
                }
                if (++x >= width) {
                    x = 0;
                    ++y;
                }
            }
            layers.push_back(layer);
        }
    }

    // Загружаем объекты
    if (levelData.contains("layers")) {
        for (const auto& layerData : levelData["layers"]) {
            if (layerData["type"] == "objectgroup") {
                for (const auto& objectData : layerData["objects"]) {
                    Object object;
                    object.name = objectData.value("name", "");
                    object.type = objectData.value("type", "");

                    int x = objectData["x"];
                    int y = objectData["y"];
                    object.rect = sf::IntRect(x, y, objectData.value("width", 0), objectData.value("height", 0));

                    if (objectData.contains("gid")) {
                        int gid = objectData["gid"].get<int>() - levelData["tilesets"][0]["firstgid"].get<int>();
                        object.sprite.setTexture(tilesetImage);
                        object.sprite.setTextureRect(subRects[gid]);
                        object.sprite.setPosition(x, y);
                    }

                    if (objectData.contains("properties")) {
                        for (const auto& property : objectData["properties"]) {
                            object.properties[property["name"]] = property["value"];
                        }
                    }

                    objects.push_back(object);
                }
            }
        }
    }

    return true;
}

Object Level::GetObject(const std::string& name) {
    for (const auto& object : objects) {
        if (object.name == name) {
            return object;
        }
    }
    return Object();
}

std::vector<Object> Level::GetObjects(const std::string& name) {
    std::vector<Object> result;
    for (const auto& object : objects) {
        if (object.name == name) {
            result.push_back(object);
        }
    }
    return result;
}

sf::Vector2i Level::GetTileSize() {
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::Draw(sf::RenderWindow& window) {
    for (const auto& layer : layers) {
        for (const auto& tile : layer.tiles) {
            window.draw(tile);
        }
    }
}

int Level::GetWidth() const {
    return width;
}

int Level::GetHeight() const {
    return height;
}