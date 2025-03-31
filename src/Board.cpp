#include "../include/Board.hpp"
#include <iostream>

// Constructor: Initializes board and loads texture
Board::Board(int rows, int columns, float radius, sf::RenderWindow& window)
    : rows(rows), columns(columns), radius(radius) {
    
    // Load wood texture
    if (!woodTexture.loadFromFile("assets/textures/light-wood.jpg")) {
        std::cerr << "Error loading texture\n";
    }

    // Set up hexagon properties
    hexagon.setRadius(radius);
    hexagon.setPointCount(6);
    hexagon.setOutlineColor(sf::Color::Black);
    hexagon.setOutlineThickness(1);
    hexagon.setFillColor(sf::Color::White);
    hexagon.setOrigin(radius, radius);
    hexagon.setTexture(&woodTexture);

    // Set up view
    view = window.getDefaultView();
}

// Handle resizing and repositioning the board
void Board::updateView(sf::RenderWindow& window, int newWidth, int newHeight) {
    float aspectRatio = static_cast<float>(newWidth) / static_cast<float>(newHeight);
    view.setSize(800 * aspectRatio, 800);

    // Centre the grid of hexagons in the middle of the window
    float gridCentreX = (rows - 1) * 1.5f * radius;
    float gridCentreY = (columns - 1) * radius;

    view.setCenter(gridCentreX, gridCentreY);
    view.zoom(0.55f);
    window.setView(view);
}

// Draw the board (hexagon grid)
void Board::draw(sf::RenderWindow& window) {
    window.clear(sf::Color(85, 115, 85)); // Dark sage green

    int gridOffset = 36;
    int shiftGrid = gridOffset;

    for (int y = 0; y < columns; ++y) {
        for (int x = 0; x < rows; ++x) {
            hexagon.setPosition(shiftGrid + x * gridOffset, 50 + y * (gridOffset - 4));
            window.draw(hexagon);
        }
        shiftGrid += (gridOffset / 2);
    }
}

