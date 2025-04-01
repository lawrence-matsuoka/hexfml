#include "../include/Game.hpp"
#include <cmath>

Game::Game(Board &board, sf::RenderWindow &window)
    : board(board), window(window), rows(board.getHexCenters().size()),
      columns(board.getHexCenters()[0].size()), radius(board.radius),
      playerTurn(true), boardState(rows, std::vector<int>(columns, 0)) {
  // Set up game piece (circle)
  piece.setRadius(radius - 10);
  piece.setOrigin(radius - 10, radius - 10);
}

void Game::handleClick(sf::Vector2i mousePosition) {
  const auto &hexCenters = board.getHexCenters();

  int closestX = -1, closestY = -1;
  float minDistance = std::numeric_limits<float>::max();

  // Convert mouse position to world coordinates if the view is scaled
  sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

  // Find the closest hex center to the mouse click
  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      sf::Vector2f center = hexCenters[x][y];
      float distance =
          std::hypot(mouseWorldPos.x - center.x, mouseWorldPos.y - center.y);

      if (distance < minDistance) {
        minDistance = distance;
        closestX = x;
        closestY = y;
      }
    }
  }
  // Check if valid move
  if (closestX >= 0 && closestX < rows && closestY >= 0 && closestY < columns &&
      boardState[closestX][closestY] == 0) {
    boardState[closestX][closestY] = playerTurn ? 1 : 2; // Set player move
    playerTurn = !playerTurn;                            // Switch turns
  }
}

void Game::draw(sf::RenderWindow &window) {
  const auto &hexCenters = board.getHexCenters();

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      if (boardState[x][y] != 0) {
        piece.setFillColor(boardState[x][y] == 1 ? sf::Color::Black
                                                 : sf::Color::White);
        piece.setPosition(hexCenters[x][y]); // Use stored center positions
        window.draw(piece);
      }
    }
  }
}
