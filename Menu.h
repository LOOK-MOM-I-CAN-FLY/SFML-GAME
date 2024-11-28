#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iostream>
#include <algorithm>
#include "Player.h"

class Player;

class Menu {
public:
    Menu(sf::RenderWindow& window, const std::string& backgroundPath);
    void run();
    bool startNewGame;

private:
    void handleEvents();
    void render();
    void updateMusic();
    void openPauseMenu();
    void saveGame();
    void openSettings();
    void loadGame();    
    void handleEscape();
    sf::RenderWindow& window;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    std::vector<sf::Text> buttons;
    size_t selectedButton;

    sf::Music music;
    std::vector<std::string> playlist;
    size_t currentTrack;
    float musicVolume;

    bool gamePaused;
};
