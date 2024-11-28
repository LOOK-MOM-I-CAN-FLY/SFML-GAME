#pragma once
#include <SFML/Graphics.hpp>

extern sf::View view;  // Объявляем глобальную переменную камеры

void updateViewForPlayer(float x, float y);  // Объявление функции для обновления камеры
