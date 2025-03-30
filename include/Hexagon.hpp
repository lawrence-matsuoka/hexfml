#ifndef HEXAGON_HPP
#define HEXAGON_HPP

#include <SFML/Graphics.hpp>

class Hexagon {
public:
    sf::ConvexShape shape;

    // Constructor to create a hexagon at a given position and size
    Hexagon(float radius, float x, float y);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow& window);
};

#endif

