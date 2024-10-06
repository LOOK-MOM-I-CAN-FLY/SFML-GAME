#include "level.h"
#include <tinyxml2.h>

TiledMap::TiledMap(const std::string& filePath, sf::RenderWindow& window)
    : filePath(filePath), window(window) {}

void TiledMap::loadMap() {
    tinyxml2::XMLDocument mapFile;
    if (mapFile.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load map file: " << filePath << std::endl;
        return;
    }

    auto* mapElement = mapFile.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "Invalid map format!" << std::endl;
        return;
    }

    loadTilesets(mapElement);
    loadLayers(mapElement);
}

void TiledMap::loadTilesets(tinyxml2::XMLElement* mapElement) {
    for (auto* tilesetElement = mapElement->FirstChildElement("tileset");
         tilesetElement; tilesetElement = tilesetElement->NextSiblingElement("tileset")) {
        
        Tileset tileset;
        tileset.firstgid = tilesetElement->IntAttribute("firstgid");
        tileset.tileWidth = tilesetElement->IntAttribute("tilewidth");
        tileset.tileHeight = tilesetElement->IntAttribute("tileheight");
        tileset.columns = tilesetElement->IntAttribute("columns");

        auto* imageElement = tilesetElement->FirstChildElement("image");
        if (imageElement) {
            std::string imagePath = imageElement->Attribute("source");
            if (!tileset.texture.loadFromFile(imagePath)) {
                std::cerr << "Failed to load texture: " << imagePath << std::endl;
                continue;
            }
        }
        tilesets.push_back(tileset);
    }
}

void TiledMap::loadLayers(tinyxml2::XMLElement* mapElement) {
    for (auto* layerElement = mapElement->FirstChildElement("layer");
         layerElement; layerElement = layerElement->NextSiblingElement("layer")) {
        
        Layer layer;
        layer.width = layerElement->IntAttribute("width");
        layer.height = layerElement->IntAttribute("height");

        auto* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) {
            std::cerr << "Layer is missing data!" << std::endl;
            continue;
        }

        const char* encoding = dataElement->Attribute("encoding");
        if (!encoding || std::string(encoding) != "csv") {
            std::cerr << "Unsupported data encoding or missing data!" << std::endl;
            continue;
        }

        const char* csvData = dataElement->GetText();
        if (!csvData) {
            std::cerr << "Failed to read CSV data from layer." << std::endl;
            continue;
        }

        // Обрабатываем CSV данные
        std::istringstream dataStream(csvData);
        std::string tileId;
        while (std::getline(dataStream, tileId, ',')) {
            try {
                int id = std::stoi(tileId);
                layer.tiles.push_back(id);  // Добавляем тайл в слой
            } catch (const std::invalid_argument& e) {
                std::cerr << "Invalid tile ID encountered: " << tileId << std::endl;
                continue;
            }
        }

        layers.push_back(layer);
    }
}

void TiledMap::draw(sf::RenderWindow& window) {
    for (const auto& layer : layers) {
        for (int y = 0; y < layer.height; ++y) {
            for (int x = 0; x < layer.width; ++x) {
                int tileId = layer.tiles[y * layer.width + x];
                if (tileId == 0) continue;  // Пропускаем пустые тайлы

                const Tileset* tileset = nullptr;
                for (const auto& ts : tilesets) {
                    if (tileId >= ts.firstgid) {
                        tileset = &ts;
                    } else {
                        break;
                    }
                }

                if (tileset) {
                    int localId = tileId - tileset->firstgid;
                    int tx = (localId % tileset->columns) * tileset->tileWidth;
                    int ty = (localId / tileset->columns) * tileset->tileHeight;

                    sf::Sprite sprite;
                    sprite.setTexture(tileset->texture);
                    sprite.setTextureRect(sf::IntRect(tx, ty, tileset->tileWidth, tileset->tileHeight));
                    sprite.setPosition(x * tileset->tileWidth, y * tileset->tileHeight);

                    window.draw(sprite);
                } else {
                    std::cerr << "No matching tileset for tile ID: " << tileId << std::endl;
                }
            }
        }
    }
}
