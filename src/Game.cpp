#include "../include/Game.hpp"
#include <cmath>
#include <iostream>

void Game::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::Resized) {
        board.updateView(window, event.size.width, event.size.height);
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      }
    }

    //    board.draw(window, window.getSize().x, window.getSize().y);
    board.draw(window, 1600, 900);
    draw(window);
    window.display();
  }
}

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
  turnText.setString("Black goes first");             // Initial text

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

    // **Check for a winner**
    int winner = checkWinner();
    if (winner != 0) {
      displayWinner(winner);
      return; // Stop processing after a win
    }

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

int Game::checkWinner() {
  // Initialize visited grids
  std::vector<std::vector<bool>> visited(rows,
                                         std::vector<bool>(columns, false));
  // Check for Black's (player 1) victory (top to bottom)
  for (int i = 0; i < columns; ++i) {
    if (boardState[i][0] == 1 && !visited[i][0] && dfs(i, 0, 1, visited)) {
      return 1; // Black wins
    }
  }
  // Check for White's (player 2) victory (left to right)
  visited = std::vector<std::vector<bool>>(
      rows, std::vector<bool>(columns, false)); // Reset visited
  for (int i = 0; i < rows; ++i) {
    if (boardState[0][i] == 2 && !visited[0][i] && dfs(0, i, 2, visited)) {
      return 2; // White wins
    }
  }
  return 0; // No winner, but Hex will always have a winner
}

bool Game::dfs(int x, int y, int player,
               std::vector<std::vector<bool>> &visited) {
  // If out of bounds, return false
  if (x < 0 || x >= rows || y < 0 || y >= columns) {
    return false;
  }
  // If already visited or not the player's piece, return false
  if (visited[x][y] || boardState[x][y] != player) {
    return false;
  }
  // Mark the current cell as visited
  visited[x][y] = true;
  // **Check the winning conditions**:
  // If it's Black's turn (player == 1), check if we've reached the bottom row
  // (win condition)
  if (player == 1 && y == columns - 1) { // Black: top to bottom
    return true;                         // Reached the bottom row
  }
  // If it's White's turn (player == 2), check if we've reached the rightmost
  // column (win condition)
  if (player == 2 && x == rows - 1) { // White: left to right
    return true;                      // Reached the rightmost column
  }
  // Get neighbors based on hexagonal layout
  int dx[] = {0, 0, 1, -1,
              1, -1}; // Right, Left, Down-right, Up-left, Down-left, Up-right
  int dy[] = {1,  -1, 0, 0,
              -1, 1}; // Right, Left, Down-right, Up-left, Down-left, Up-right
  // Adjust the direction for odd rows (hexagonal board)
  if (x % 2 == 1) {
    dx[4] = 1;  // Down-left (different for odd rows)
    dx[5] = -1; // Up-right (different for odd rows)
  }
  // Recursively check all neighboring cells
  for (int i = 0; i < 6; ++i) {
    int nx = x + dx[i];
    int ny = y + dy[i];
    if (dfs(nx, ny, player, visited)) {
      return true;
    }
  }
  return false; // No path found
}

void Game::displayWinner(int winner) {
  // Set the winning message
  if (winner == 1) {
    turnText.setString("Black Wins!");
    turnText.setFillColor(sf::Color::Black);
  } else {
    turnText.setString("White Wins!");
    turnText.setFillColor(sf::Color::White);
  }
  // Display the winner
  window.draw(turnText);
  window.display();
  // Show the winning message for 3 seconds
  sf::sleep(sf::seconds(3));
  // Reset the game
  resetGame();
}

void Game::resetGame() {
  // Clear the board
  for (int x = 0; x < rows; ++x) {
    for (int y = 0; y < columns; ++y) {
      boardState[x][y] = 0;
    }
  }
  // Reset turn to Black
  playerTurn = true;
  turnText.setString("Black's turn");
  turnText.setFillColor(sf::Color::Black);
}
