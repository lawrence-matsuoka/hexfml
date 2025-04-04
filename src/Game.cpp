#include "../include/Game.hpp"
#include "../include/Audio.hpp"
#include "../include/PauseMenu.hpp"
#include <cmath>
#include <iostream>

void Game::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::Resized) {
        board.updateView(window, event.size.width, event.size.height);
      }
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        gamePaused = !gamePaused;
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        handleClick(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      }
    }

    if (gamePaused) {
      // Show the pause menu when the game is paused
      PauseMenuResult result = pauseMenu.show();
      // Handle the result of the pause menu (resume, settings, etc.)
      if (result == PauseMenuResult::Resume) {
        gamePaused = false;
      } else if (result == PauseMenuResult::QuitToMenu) {
        resetGame();
        gamePaused = !gamePaused;
        return;
      } else if (result == PauseMenuResult::QuitToDesktop) {
        window.close();
      }
    }
    if (quitToMenu) {
      return;
    } else {
      board.draw(window, 1600, 900);
      draw(window);
      window.display();
    }
  }
}

Game::Game(Board &board, sf::RenderWindow &window)
    : board(board), window(window),
      pauseMenu(window, {"Resume", "Quit to Menu", "Quit to Desktop"},
                {[this]() { return PauseMenuResult::Resume; },
                 [this]() { return PauseMenuResult::QuitToMenu; },
                 [this]() { return PauseMenuResult::QuitToDesktop; }}),
      rows(board.getHexCenters().size()),
      columns(board.getHexCenters()[0].size()), radius(board.radius),
      playerTurn(true), boardState(rows, std::vector<int>(columns, 0)),
      quitToMenu(false) {
  // Set up game piece
  int pieceRadius = radius / 2;
  piece.setRadius(pieceRadius);
  piece.setOrigin(pieceRadius, pieceRadius);

  // Hover effect before placing a piece
  hoverPiece.setRadius(pieceRadius);
  hoverPiece.setOrigin(pieceRadius, pieceRadius);
  hoverPiece.setFillColor(sf::Color(0, 0, 0, 100)); // semi-transparent

  // Load fonts
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font\n";
  }
  // Text properties
  turnText.setFont(font);
  turnText.setCharacterSize(50);
  turnText.setFillColor(sf::Color::Black);
  turnText.setPosition(20, window.getSize().y - 100);
  turnText.setString("Black goes first");

  // Load piece textures
  if (!blackTexture.loadFromFile("assets/textures/reflective-black.jpg")) {
    std::cerr << "Error loading black pieces texture\n";
  }
  if (!whiteTexture.loadFromFile("assets/textures/reflective-white.jpg")) {
    std::cerr << "Error loading white pieces texture\n";
  }
}

void Game::handleClick(sf::Vector2i mousePosition) {
  // Convert mouse position to world coordinates if the view is scaled
  sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);

  const auto &hexCenters = board.getHexCenters();

  int closestX = -1, closestY = -1;
  float minDistance = std::numeric_limits<float>::max();

  bool isInsideBoard = false;

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
    Sounds::playSound("piece");

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
        isInsideBoard = true;
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
  // if (boardState[closestX][closestY] == 0) {
  //   window.draw(hoverPiece);
  // }
  if (closestX >= 0 && closestX < rows && closestY >= 0 && closestY < columns &&
      boardState[closestX][closestY] == 0) {
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
  // Out of bounds
  if (x < 0 || x >= rows || y < 0 || y >= columns) {
    return false;
  }
  // Already visited or other player's piece
  if (visited[x][y] || boardState[x][y] != player) {
    return false;
  }
  // Mark current cell as visited
  visited[x][y] = true;
  // Black win con
  if (player == 1 && y == columns - 1) {
    return true;
  }
  // White win con
  if (player == 2 && x == rows - 1) {
    return true;
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
  // No path found
  return false;
}

void Game::displayWinner(int winner) {
  // Semi-transparent overlay as background
  sf::RectangleShape overlay(sf::Vector2f(1600, 900));
  overlay.setFillColor(sf::Color(0, 0, 0, 150));
  // Winner menu box
  sf::RectangleShape menuBox(sf::Vector2f(400, 400));
  menuBox.setFillColor(sf::Color(85, 115, 85));
  menuBox.setOrigin(menuBox.getLocalBounds().width / 2,
                    menuBox.getLocalBounds().height / 2);
  menuBox.setPosition(800, 600);

  // Load font for text
  sf::Font font;
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font\n";
  }

  // Set winner text
  sf::Text winnerText;
  winnerText.setFont(font);
  winnerText.setCharacterSize(50);
  winnerText.setString(winner == 1 ? "Black Wins!" : "White Wins!");
  winnerText.setFillColor(winner == 1 ? sf::Color::Black : sf::Color::White);
  winnerText.setOrigin(winnerText.getLocalBounds().width / 2,
                       winnerText.getLocalBounds().height / 2);
  winnerText.setPosition(800, 450);

  // Button properties
  float buttonWidth = 300;
  float buttonHeight = 50;
  float buttonX = 650;
  float startY = menuBox.getPosition().y - 50;
  float spacing = 70;

  std::vector<sf::RectangleShape> buttons;
  std::vector<sf::Text> buttonLabels;
  std::vector<std::string> buttonNames = {"Play Again", "Quit to Menu",
                                          "Quit to Desktop"};
  std::vector<std::function<void()>> buttonActions = {
      [this]() { resetGame(); }, // Play Again
      [this]() {
        resetGame();
        quitToMenu = true;
      },                           // Quit to Menu
      [this]() { window.close(); } // Quit to Desktop
  };

  for (size_t i = 0; i < buttonNames.size(); ++i) {
    // Create button
    sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
    button.setFillColor(sf::Color(100, 150, 100));
    button.setPosition(buttonX, startY + i * spacing);
    buttons.push_back(button);

    // Create button text
    sf::Text text;
    text.setFont(font);
    text.setString(buttonNames[i]);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);
    text.setOrigin(text.getLocalBounds().width / 2,
                   text.getLocalBounds().height / 2);
    text.setPosition(buttonX + buttonWidth / 2,
                     startY + i * spacing + buttonHeight / 2 - 5);
    buttonLabels.push_back(text);
  }
  // Event loop for winner's menu
  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
    bool mouseOverButton = false;

    for (size_t i = 0; i < buttons.size(); ++i) {
      if (buttons[i].getGlobalBounds().contains(mouseWorldPos.x,
                                                mouseWorldPos.y)) {
        buttonLabels[i].setFillColor(sf::Color::Yellow);
        mouseOverButton = true;
      } else {
        buttonLabels[i].setFillColor(sf::Color::White);
      }
    }
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        Sounds::playSound("click");
        if (mouseOverButton) {
          for (size_t i = 0; i < buttons.size(); ++i) {
            if (buttons[i].getGlobalBounds().contains(mouseWorldPos.x,
                                                      mouseWorldPos.y)) {
              buttonActions[i]();
              return; // Exit menu after selecting an action
            }
          }
        }
      }
    }

    // Redraw everything
    window.clear();
    board.draw(window, 1600, 900); // Keep the game board visible
    window.draw(overlay);
    window.draw(menuBox);
    window.draw(winnerText);
    for (auto &button : buttons)
      window.draw(button);
    for (auto &label : buttonLabels)
      window.draw(label);
    window.display();
  }
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
  // Reset quitToMenu
  quitToMenu = false;
}

// Online
Game::Move Game::getMove() {
  const auto &hexCenters = board.getHexCenters();

  while (true) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return Move{-1, -1}; // Special case
      }

      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mouseWorldPos =
            window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        int closestX = -1, closestY = -1;
        float minDistance = std::numeric_limits<float>::max();

        for (int x = 0; x < rows; ++x) {
          for (int y = 0; y < columns; ++y) {
            sf::Vector2f center = hexCenters[x][y];
            float distance = std::hypot(mouseWorldPos.x - center.x,
                                        mouseWorldPos.y - center.y);
            if (distance < minDistance) {
              minDistance = distance;
              closestX = x;
              closestY = y;
            }
          }
        }

        if (closestX >= 0 && closestY >= 0 &&
            boardState[closestX][closestY] == 0) {
          return Move{closestX, closestY};
        }
      }
    }

    window.clear();
    board.draw(window, 1600, 900);
    draw(window);
    window.display();
  }
}

void Game::applyMove(const Move &move) {
  if (boardState[move.x][move.y] == 0) {
    boardState[move.x][move.y] = playerTurn ? 1 : 2;
    playerTurn = !playerTurn;

    // Optional: update turn text and color
    if (playerTurn) {
      turnText.setString("Black");
      turnText.setFillColor(sf::Color::Black);
    } else {
      turnText.setString("White");
      turnText.setFillColor(sf::Color::White);
    }

    // Check for win
    int winner = checkWinner();
    if (winner != 0) {
      displayWinner(winner);
    }
  }
}

bool Game::isGameOver() { return checkWinner() != 0; }
sf::RenderWindow &Game::getWindow() { return window; }
