#include "../include/Board.hpp"
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>

// Constructor: Initializes board and loads texture
Board::Board(int rows, int columns, float radius, sf::RenderWindow &window)
    : rows(rows), columns(columns), radius(radius),
      hexCenters(rows,
                 std::vector<sf::Vector2f>(columns)) { // Initialize hexCenters

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
  view.setSize(900 * aspectRatio, 900);
  //  view.zoom(0.8f); // 0.55*/
  window.setView(view);
}

// Draw the board (hexagon grid)
void Board::draw(sf::RenderWindow &window, unsigned int windowX,
                 unsigned int windowY) {
  window.clear(sf::Color(85, 115, 85)); // Dark sage green

  int boardX = 3 * radius * rows - radius;
  int boardY = 2 * radius * columns;
  int initialX = (windowX - boardX) / 2;
  // board length = (2 * radius * columns)
  int initialY = (windowY - boardY) + 3 * radius;

  int gridOffset = 1.775 * radius;
  int shiftGrid = gridOffset;

  int borderX = radius * 17.67;

  // Top black border
  drawBackgroundBorder(
      window,
      sf::Color::Black, // Border color
      sf::Vector2f(borderX,
                   radius), // size of border found by trial and error
      sf::Vector2f(initialX + 1.825 * radius,
                   initialY - radius - 1), // Position of the top-left corner
      0                                    // Rotation angle in degrees
  );
  // Bottom black border
  drawBackgroundBorder(window, sf::Color::Black,
                       sf::Vector2f(borderX,
                                    radius), // size of border
                       sf::Vector2f(initialX + 10 * radius + 23, 751),
                       0); // position

  // Left white border
  drawBackgroundBorder(window, sf::Color::White,
                       sf::Vector2f(borderX, radius), // size of border
                       sf::Vector2f(initialX + 70, initialY - 1),
                       60.15); // position
  // Right white border
  drawBackgroundBorder(window, sf::Color::White,
                       sf::Vector2f(borderX, radius), // size of border
                       sf::Vector2f(975, initialY - 22), 60.15); // position

  //
  for (int y = 0; y < columns; ++y) {
    for (int x = 0; x < rows; ++x) {
      int hexX = initialX + x * gridOffset + shiftGrid;
      int hexY = initialY + y * (gridOffset - (0.25 * radius));

      hexCenters[x][y] = sf::Vector2f(hexX, hexY);
      hexagon.setPosition(hexX, hexY);
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
  border.setPosition(position);
  border.setRotation(angle);

  window.draw(border);
}

const std::vector<std::vector<sf::Vector2f>> &Board::getHexCenters() const {
  return hexCenters;
}
