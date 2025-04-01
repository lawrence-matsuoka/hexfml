#include "../include/Game.hpp"
#include <cmath>
#include <iostream>

Game::Game(Board &board, sf::RenderWindow &window)
    : board(board), window(window), rows(board.getHexCenters().size()),
      columns(board.getHexCenters()[0].size()), radius(board.radius),
      playerTurn(true), boardState(rows, std::vector<int>(columns, 0)) {
  // Set up game piece (circle)
  int pieceRadius = radius / 2;
  piece.setRadius(pieceRadius);
  piece.setOrigin(pieceRadius, pieceRadius);

  // Hover effect before placing a piece
  hoverPiece.setRadius(pieceRadius);
  hoverPiece.setOrigin(pieceRadius, pieceRadius);
  hoverPiece.setFillColor(
      sf::Color(0, 0, 0, 100)); // Initially black, semi-transparent

  // Load fonts
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font\n";
  }
  // Set up text properties
  turnText.setFont(font);
  turnText.setCharacterSize(50);
  turnText.setFillColor(sf::Color::Black);
  turnText.setPosition(20, window.getSize().y - 100); // Bottom-left corner
  turnText.setString("Black goes first");                // Initial text

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

  // Load and play background music
  /*  if (!music.openFromFile(
            "assets/music/Nick_Roberts_March_to_the_Zenith.ogg")) {
      std::cerr << "Error loading background music\n";
    } else {
       Temp set volume (add to settings later)
      music.setVolume(50);
      music.setLoop(true);
      music.play();
    }*/
}

void Game::handleClick(sf::Vector2i mousePosition) {
  // Convert mouse position to world coordinates if the view is scaled
  sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

  const auto &hexCenters = board.getHexCenters();

  int closestX = -1, closestY = -1;
  float minDistance = std::numeric_limits<float>::max();

  bool isInsideBoard = false; // Track if mouse is inside board bounds

  // Find the closest hex center to the mouse click
  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      sf::Vector2f center = hexCenters[x][y];
      // Check if mouse is inside board bounds
      if (mouseWorldPos.x >= hexCenters[x][y].x - radius &&
          mouseWorldPos.x <= hexCenters[x][y].x + radius &&
          mouseWorldPos.y >= hexCenters[x][y].y - radius &&
          mouseWorldPos.y <= hexCenters[x][y].y + radius) {
        isInsideBoard = true; // Mouse is inside board
      }
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
  if (isInsideBoard && closestX >= 0 && closestX < rows && closestY >= 0 &&
      closestY < columns && boardState[closestX][closestY] == 0) {
    boardState[closestX][closestY] = playerTurn ? 1 : 2; // Set player move
    playerTurn = !playerTurn;                            // Switch turns

    // Play wood sound
    pieceSound.play();

    // **Update turn text & color**
    if (playerTurn) {
      turnText.setString("Black");
      turnText.setFillColor(sf::Color::Black); // Black text
    } else {
      turnText.setString("White");
      turnText.setFillColor(sf::Color::White); // White text
    }
  }
}

void Game::draw(sf::RenderWindow &window) {
  const auto &hexCenters = board.getHexCenters();

  // Handle hover effect based on mouse position
  sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
  sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
  int closestX = -1, closestY = -1;
  float minDistance = std::numeric_limits<float>::max();

  bool isInsideBoard = false;

  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      sf::Vector2f center = hexCenters[x][y];
      // Check if mouse is inside the board
      if (mouseWorldPos.x >= hexCenters[x][y].x - radius &&
          mouseWorldPos.x <= hexCenters[x][y].x + radius &&
          mouseWorldPos.y >= hexCenters[x][y].y - radius &&
          mouseWorldPos.y <= hexCenters[x][y].y + radius) {
        isInsideBoard = true; // Mouse is inside board
      }
      float distance =
          std::hypot(mouseWorldPos.x - center.x, mouseWorldPos.y - center.y);
      if (distance < minDistance) {
        minDistance = distance;
        closestX = x;
        closestY = y;
      }
    }
  }
  // Change colour on player turn
  hoverPiece.setFillColor(playerTurn ? sf::Color(0, 0, 0, 100)
                                     : sf::Color(255, 255, 255, 100));
  // Set the hover piece position to follow the mouse
  if (isInsideBoard && closestX >= 0 && closestX < rows && closestY >= 0 &&
      closestY < columns) {
    hoverPiece.setPosition(hexCenters[closestX][closestY]);
  } else {
    hoverPiece.setPosition(-100, -100); // Set off screen
  }
  if (boardState[closestX][closestY] == 0) {
    window.draw(hoverPiece);
  }

  // Draw the piece
  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      if (boardState[x][y] != 0) {
        // Set texture based on player (1 or 2)
        if (boardState[x][y] == 1) {
          piece.setTexture(&blackTexture); // Player 1 (black piece)
        } else {
          piece.setTexture(&whiteTexture); // Player 2 (white piece)
        }
        piece.setPosition(hexCenters[x][y]);
        window.draw(piece);
      }
    }
  }
  // **Draw turn text**
  window.draw(turnText);
}
