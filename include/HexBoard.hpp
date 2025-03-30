#ifndef HEXBOARD_HPP
#define HEXBOARD_HPP

#include "Hexagon.hpp"
#include <vector>

class HexBoard {
public:
    std::vector<std::vector<Hexagon>> board;
    float hexRadius;
    float hexWidth;
    float hexHeight;

    // Constructor to initialize the board
    HexBoard(float radius);

    // Resize board based on window size
    void resizeBoard(float windowWidth, float windowHeight);

    // Draw all hexagons on the board
    void draw(sf::RenderWindow& window);
};

#endif

