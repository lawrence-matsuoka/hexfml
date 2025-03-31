#include "../include/Board.hpp"
#include <cmath>
#include <iostream>

// Constructor: Initializes board and loads texture
Board::Board(int rows, int columns, float radius, sf::RenderWindow &window)
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
void Board::updateView(sf::RenderWindow &window, int newWidth, int newHeight) {
  float aspectRatio =
      static_cast<float>(newWidth) / static_cast<float>(newHeight);
  view.setSize(800 * aspectRatio, 800);
  //  view.zoom(0.8f); // 0.55*/
  window.setView(view);
}

// Draw the board (hexagon grid)
void Board::draw(sf::RenderWindow &window, unsigned int windowX,
                 unsigned int windowY) {
  window.clear(sf::Color(85, 115, 85)); // Dark sage green

  // The first hex is drawn at 36, 50 with origin (20,20)
  // Add bottom and top black border
  drawBackgroundBorder(
      window,
      sf::Color::Black,       // Border color
      sf::Vector2f(300, 500), // Width & height of the parallelogram
      sf::Vector2f(25, 30),   // Position of the top-left corner
      0                       // Rotation angle in degrees
  );
  // Add left and right white border

  // board width = (3 * radius * rows - radius)
  int initialX = (windowX - (3 * radius * rows - radius)) / 2;
  // board length = (2 * radius * columns)
  int initialY = (windowY - (2 * radius * columns)) / 2;

  int gridOffset = 1.8 * radius;
  int shiftGrid = gridOffset;

  for (int y = 0; y < columns; ++y) {
    for (int x = 0; x < rows; ++x) {
      hexagon.setPosition(initialX + x * gridOffset + shiftGrid,
                          initialY + y * (gridOffset - (0.2 * radius)));

      window.draw(hexagon);
    }
    shiftGrid += (gridOffset / 2);
  }
}

void Board::drawBackgroundBorder(sf::RenderWindow &window, sf::Color color,
                                 sf::Vector2f size, sf::Vector2f position,
                                 float angle) {
  sf::RectangleShape border(size); // Simple rectangle
  border.setFillColor(color);

  // Set position, rotation, and center for proper rotation
  // border.setOrigin(size.x / 2, size.y / 2); // Center origin for proper
  // rotation
  border.setPosition(position);
  border.setRotation(angle);

  window.draw(border);
}
