#include "level.h"

int Object::GetPropertyInt(const std::string& name) const {
    return std::stoi(properties.at(name));
}

float Object::GetPropertyFloat(const std::string& name) const {
    return std::stof(properties.at(name));
}

std::string Object::GetPropertyString(const std::string& name) const {
    return properties.at(name);
}

bool Level::loadFromFile(const std::string& filename) {
    tinyxml2::XMLDocument document;
    if (document.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load file: " << filename << std::endl;
        return false;
    }

    tinyxml2::XMLElement* mapElement = document.FirstChildElement("map");
    if (!mapElement) {
        std::cerr << "No 'map' element found in file: " << filename << std::endl;
        return false;
    }

    const char* tileWidthAttr = mapElement->Attribute("tilewidth");
    const char* tileHeightAttr = mapElement->Attribute("tileheight");
    const char* widthAttr = mapElement->Attribute("width");
    const char* heightAttr = mapElement->Attribute("height");

    if (!tileWidthAttr || !tileHeightAttr || !widthAttr || !heightAttr) {
        std::cerr << "Map attributes missing in file: " << filename << std::endl;
        return false;
    }

    tileWidth = std::stoi(tileWidthAttr);
    tileHeight = std::stoi(tileHeightAttr);
    width = std::stoi(widthAttr);
    height = std::stoi(heightAttr);

    // Loading tileset
    tinyxml2::XMLElement* tilesetElement = mapElement->FirstChildElement("tileset");
    if (!tilesetElement) {
        std::cerr << "No 'tileset' element found in file: " << filename << std::endl;
        return false;
    }

    const char* firstGidAttr = tilesetElement->Attribute("firstgid");
    if (!firstGidAttr) {
        std::cerr << "No 'firstgid' attribute in 'tileset' element" << std::endl;
        return false;
    }

    firstTileID = std::stoi(firstGidAttr);

    tinyxml2::XMLElement* imageElement = tilesetElement->FirstChildElement("image");
    if (!imageElement) {
        std::cerr << "No 'image' element found in 'tileset'" << std::endl;
        return false;
    }

    const char* textureFile = imageElement->Attribute("source");
    if (!textureFile) {
        std::cerr << "No 'source' attribute in 'image' element" << std::endl;
        return false;
    }

    if (!tilesetTexture.loadFromFile(textureFile)) {
        std::cerr << "Failed to load tileset texture: " << textureFile << std::endl;
        return false;
    }

    // Calculate the number of columns and rows in the tileset
    int columns = tilesetTexture.getSize().x / tileWidth;
    int rows = tilesetTexture.getSize().y / tileHeight;
    std::vector<sf::IntRect> subRects;

    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            sf::IntRect rect(x * tileWidth, y * tileHeight, tileWidth, tileHeight);
            subRects.push_back(rect);
        }
    }

    // Parse tile layers
    tinyxml2::XMLElement* layerElement = mapElement->FirstChildElement("layer");
    while (layerElement) {
        tinyxml2::XMLElement* dataElement = layerElement->FirstChildElement("data");
        if (!dataElement) {
            std::cerr << "No 'data' element in 'layer'" << std::endl;
            layerElement = layerElement->NextSiblingElement("layer");
            continue;
        }

        const char* data = dataElement->GetText();
        if (!data) {
            std::cerr << "No text found in 'data' element" << std::endl;
            layerElement = layerElement->NextSiblingElement("layer");
            continue;
        }

        std::vector<int> tileNumbers;
        std::string buffer;

        for (char c : std::string(data)) {
            if (isdigit(c)) {
                buffer += c;
            } else if (!buffer.empty()) {
                tileNumbers.push_back(std::stoi(buffer));
                buffer.clear();
            }
        }

        sf::VertexArray layerVertices(sf::Quads);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int tileGID = tileNumbers[y * width + x];
                if (tileGID == 0) continue;

                sf::Vertex quad[4];
                quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
                quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
                quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
                quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

                int subRectIdx = tileGID - firstTileID;
                sf::IntRect texRect = subRects[subRectIdx];

                quad[0].texCoords = sf::Vector2f(texRect.left, texRect.top);
                quad[1].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top);
                quad[2].texCoords = sf::Vector2f(texRect.left + texRect.width, texRect.top + texRect.height);
                quad[3].texCoords = sf::Vector2f(texRect.left, texRect.top + texRect.height);

                for (int i = 0; i < 4; i++) {
                    layerVertices.append(quad[i]);
                }
            }
        }

        tileLayers.push_back(layerVertices);
        layerElement = layerElement->NextSiblingElement("layer");
    }

    // Parse object layers
    tinyxml2::XMLElement* objectGroupElement = mapElement->FirstChildElement("objectgroup");
    while (objectGroupElement) {
        tinyxml2::XMLElement* objectElement = objectGroupElement->FirstChildElement("object");
        while (objectElement) {
            float x = std::stof(objectElement->Attribute("x"));
            float y = std::stof(objectElement->Attribute("y"));
            float width = objectElement->Attribute("width") ? std::stof(objectElement->Attribute("width")) : 0;
            float height = objectElement->Attribute("height") ? std::stof(objectElement->Attribute("height")) : 0;

            Object obj(x, y, width, height);
            if (objectElement->Attribute("name")) obj.name = objectElement->Attribute("name");
            if (objectElement->Attribute("type")) obj.type = objectElement->Attribute("type");

            tinyxml2::XMLElement* propertiesElement = objectElement->FirstChildElement("properties");
            if (propertiesElement) {
                tinyxml2::XMLElement* propertyElement = propertiesElement->FirstChildElement("property");
                while (propertyElement) {
                    const char* propName = propertyElement->Attribute("name");
                    const char* propValue = propertyElement->Attribute("value");
                    if (propName && propValue) {
                        obj.properties[propName] = propValue;
                    }
                    propertyElement = propertyElement->NextSiblingElement("property");
                }
            }

            objects.push_back(obj);
            objectElement = objectElement->NextSiblingElement("object");
        }
        objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
    }

    return true;
}


Object Level::getObject(const std::string& name) const {
    for (const auto& obj : objects) {
        if (obj.name == name) return obj;
    }
    throw std::runtime_error("Object not found");
}

std::vector<Object> Level::getObjectsByName(const std::string& name) const {
    std::vector<Object> foundObjects;
    for (const auto& obj : objects) {
        if (obj.name == name) {
            foundObjects.push_back(obj);
        }
    }
    return foundObjects;
}

std::vector<Object> Level::getObjectsByType(const std::string& type) const {
    std::vector<Object> foundObjects;
    for (const auto& obj : objects) {
        if (obj.type == type) {
            foundObjects.push_back(obj);
        }
    }
    return foundObjects;
}

const std::vector<Object>& Level::getAllObjects() const {
    return objects;
}

sf::Vector2i Level::getTileSize() const {
    return sf::Vector2i(tileWidth, tileHeight);
}

void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = &tilesetTexture;
    for (const auto& layer : tileLayers) {
        target.draw(layer, states);
    }
}
