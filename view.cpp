#include "view.h"

sf::View view;  // Определение глобальной переменной камеры

void updateViewForPlayer(float x, float y) {
    view.setCenter(x, y);  // Центрируем камеру на позиции игрока
}
