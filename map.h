#ifndef MAP_H
#define MAP_H

#include "/Users/sindi_hall/Downloads/SFML_ROOT/include/SFML/Graphics.hpp"
const int HEIGHT_MAP = 25;//размер карты высота
const int WIDTH_MAP = 40;//размер карты ширина 
 
 
sf::String TileMap[HEIGHT_MAP] = {
	"0000000000000000000000000000000000000000",
	"0                                      0",
	"0      s                    s          0",
	"0                                      0",
	"0                                      0",
	"0              s                       0",
	"0                                      0",
	"0                             s        0",
	"0                                      0",
	"0      s                               0",
	"0                                      0",
	"0              s              s        0",
	"0                                      0",
	"0                    s                 0",
	"0                                      0",
	"0                                      0",
	"0                       s              0",
	"0        s                             0",
	"0                      s               0",
	"0                                      0",
	"0                                      0",
	"0        s           s                 0",
	"0                                      0",
	"0                                      0",
	"0000000000000000000000000000000000000000",
};

#endif // MAP_H