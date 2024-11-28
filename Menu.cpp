#include "Menu.h"

// Конструктор класса Menu
Menu::Menu(sf::RenderWindow& window, const std::string& backgroundPath) 
    : window(window), startNewGame(false), selectedButton(0), musicVolume(50.0f), currentTrack(0), gamePaused(false) {

    // Загрузка шрифта в формате OTF
    if (!font.loadFromFile("resources/Inkulinati.otf")) {
        std::cerr << "Ошибка загрузки шрифта Inkulinati.otf!\n";
    }

    // Загрузка текстуры фона
    if (!backgroundTexture.loadFromFile(backgroundPath)) {
        std::cerr << "Ошибка загрузки фона: " << backgroundPath << "\n";
    }
    backgroundSprite.setTexture(backgroundTexture);

    // Масштабирование фона под размер окна
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = backgroundTexture.getSize();
    backgroundSprite.setScale(
        static_cast<float>(windowSize.x) / textureSize.x,
        static_cast<float>(windowSize.y) / textureSize.y
    );

    // Создание кнопок меню
    std::vector<std::string> buttonLabels = {"New game", "Continue", "Settings", "Exit"};
    float startY = 200.0f; // Начальная позиция по Y для кнопок
    float offsetY = 80.0f; // Смещение между кнопками

    for (size_t i = 0; i < buttonLabels.size(); ++i) {
        sf::Text button;
        button.setFont(font);
        button.setString(buttonLabels[i]);
        button.setCharacterSize(50); // Размер шрифта кнопок
        button.setFillColor(sf::Color::Black);
        
        // Центрирование кнопки по горизонтали
        sf::FloatRect textRect = button.getLocalBounds();
        button.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        button.setPosition(windowSize.x / 2.0f, startY + i * offsetY);
        
        buttons.push_back(button);
    }

    music.setVolume(musicVolume);

    // Инициализация плейлиста с поддержкой MP3
    playlist = {
        "music/Ballada_o_drevnerusskom_voine.mp3",
        "music/Shtil.mp3",
        "music/Raskachaem_etot_mir.mp3",
        "music/Himera.mp3",
        "music/Goryashhaya_strela.mp3"
    };

    // Перемешивание плейлиста, начиная со второй песни
    std::random_device rd;
    std::mt19937 g(rd());
    if (playlist.size() > 1) {
        std::shuffle(playlist.begin() + 1, playlist.end(), g);
    }

    // Загрузка и воспроизведение первой песни
    if (!music.openFromFile(playlist[0])) {
        std::cerr << "Ошибка загрузки музыки: " << playlist[0] << "\n";
    } else {
        music.setLoop(false);
        music.play();
    }
}

// Основной цикл меню
void Menu::run() {
    while (window.isOpen()) {
        handleEvents();
        updateMusic();
        render();

        if (startNewGame) {
            break;
        }
    }
}

// Обработка событий
void Menu::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (event.type == sf::Event::KeyPressed) {
            // Навигация по кнопкам меню
            if (event.key.code == sf::Keyboard::Up) {
                selectedButton = (selectedButton > 0) ? selectedButton - 1 : buttons.size() - 1;
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedButton = (selectedButton + 1) % buttons.size();
            }

            // Выбор кнопки
            if (event.key.code == sf::Keyboard::Enter) {
                if (selectedButton == 0) { // "New game"
                    startNewGame = true;
                } else if (selectedButton == 1) { // "Continue"
                    loadGame();  // Загружаем игру
                } else if (selectedButton == 2) { // "Settings"
                    // Настройки игры
                } else if (selectedButton == 3) { // "Exit"
                    window.close();
                }
            }

            // Дополнительные команды
            if (event.key.code == sf::Keyboard::Escape && !gamePaused) {
                openPauseMenu();
            } else if (event.key.code == sf::Keyboard::Right) {
                currentTrack = (currentTrack + 1) % playlist.size();
                music.openFromFile(playlist[currentTrack]);
                music.play();
            } else if (event.key.code == sf::Keyboard::Up) {
                musicVolume = std::min(musicVolume + 5.0f, 100.0f);
                music.setVolume(musicVolume);
            } else if (event.key.code == sf::Keyboard::Down) {
                musicVolume = std::max(musicVolume - 5.0f, 0.0f);
                music.setVolume(musicVolume);
            }
        }
    }
}

// Рендеринг меню
void Menu::render() {
    window.clear();

    // Рисование фона
    window.draw(backgroundSprite);

    // Рисование кнопок
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (i == selectedButton) {
            buttons[i].setFillColor(sf::Color::Red); // Выделение выбранной кнопки
        } else {
            buttons[i].setFillColor(sf::Color::Black);
        }
        window.draw(buttons[i]);
    }

    window.display();
}

void Menu::updateMusic() {
    if (music.getStatus() == sf::Music::Stopped) {
        currentTrack = (currentTrack + 1) % playlist.size();
        if (!music.openFromFile(playlist[currentTrack])) {
            std::cerr << "Ошибка загрузки музыки: " << playlist[currentTrack] << "\n";
        } else {
            music.play();
        }
    }
}

void Menu::openPauseMenu() {
    gamePaused = true;

    std::vector<std::string> pauseOptions = {"Continue", "Save and Exit", "Exit"};
    std::vector<sf::Text> pauseButtons;
    size_t pauseSelectedButton = 0;

    for (size_t i = 0; i < pauseOptions.size(); ++i) {
        sf::Text button;
        button.setFont(font);
        button.setString(pauseOptions[i]);
        button.setCharacterSize(50);
        button.setFillColor(sf::Color::Black);

        sf::FloatRect textRect = button.getLocalBounds();
        button.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        button.setPosition(window.getSize().x / 2.0f, 300.0f + i * 80.0f);

        pauseButtons.push_back(button);
    }

    while (gamePaused && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    pauseSelectedButton = (pauseSelectedButton > 0) ? pauseSelectedButton - 1 : pauseButtons.size() - 1;
                } else if (event.key.code == sf::Keyboard::Down) {
                    pauseSelectedButton = (pauseSelectedButton + 1) % pauseButtons.size();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (pauseSelectedButton == 0) { // Continue
                        gamePaused = false;
                    } else if (pauseSelectedButton == 1) { // Save and Exit
                        saveGame();
                        window.close();
                    } else if (pauseSelectedButton == 2) { // Exit
                        window.close();
                    }
                }
            }
        }

        window.clear();
        for (size_t i = 0; i < pauseButtons.size(); ++i) {
            if (i == pauseSelectedButton) {
                pauseButtons[i].setFillColor(sf::Color::Red); 
            } else {
                pauseButtons[i].setFillColor(sf::Color::Black);
            }
            window.draw(pauseButtons[i]);
        }
        window.display();
    }
}

void Menu::saveGame() {
    // Сохранение игры (например, в файл)
    std::ofstream saveFile("game_save.txt");
    if (saveFile.is_open()) {
        saveFile << "Player position: " << 100 << ", " << 100 << "\n";
        saveFile.close();
    }
}

void Menu::loadGame() {
    // Загрузка сохраненной игры
    std::ifstream saveFile("game_save.txt");
    if (saveFile.is_open()) {
        std::string line;
        while (std::getline(saveFile, line)) {
            std::cout << line << std::endl;
        }
        saveFile.close();
    }
}
