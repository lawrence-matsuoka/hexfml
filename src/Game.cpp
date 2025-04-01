#include "../include/Game.hpp"
#include <cmath>
#include <iostream>

Game::Game(Board &board, sf::RenderWindow &window)
    : board(board), window(window), rows(board.getHexCenters().size()),
      columns(board.getHexCenters()[0].size()), radius(board.radius),
      playerTurn(true), boardState(rows, std::vector<int>(columns, 0)) {
  // Set up game piece (circle)
  piece.setRadius(radius / 2);
  piece.setOrigin(radius / 2, radius / 2);

  // Load piece textures
  if (!blackTexture.loadFromFile("assets/textures/reflective-black.jpg")) {
    std::cerr << "Error loading black pieces texture\n";
  }
  if (!whiteTexture.loadFromFile("assets/textures/reflective-white.jpg")) {
    std::cerr << "Error loading white pieces texture\n";
  }

  // Load piece sound
  if (!pieceBuffer.loadFromFile("assets/sounds/wood03.ogg")) {
    std::cerr << "Error loading piece place sound\n";
  }
  pieceSound.setBuffer(pieceBuffer);
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

    pieceSound.play();
  }
}

void Game::draw(sf::RenderWindow &window) {
  const auto &hexCenters = board.getHexCenters();

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      if (boardState[x][y] != 0) {
        // Set texture based on player (1 or 2)
        if (boardState[x][y] == 1) {
          piece.setTexture(&blackTexture);  // Player 1 (black piece)
        } else {
          piece.setTexture(&whiteTexture);  // Player 2 (white piece)
        }
        piece.setPosition(hexCenters[x][y]);
        window.draw(piece);
      }
    }
  }
}

