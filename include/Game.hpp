#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "PauseMenu.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <limits>
#include <vector>

class Game {
public:
  void run();
  Game(Board &board, sf::RenderWindow &window);
  bool gamePaused = false;
  void handleClick(sf::Vector2i mousePosition);
  void draw(sf::RenderWindow &window);
  void resetGame();
  int checkWinner();
  void displayWinner(int winner);

private:
  Board &board;
  sf::RenderWindow &window;
  PauseMenu pauseMenu;
  int rows, columns;
  float radius;
  bool playerTurn; // true = Player 1, false = Player 2
  std::vector<std::vector<int>> boardState;
  bool quitToMenu = false;

  sf::CircleShape piece;
  sf::CircleShape hoverPiece;

  // Textures
  sf::Texture blackTexture;
  sf::Texture whiteTexture;

  // Fonts
  sf::Font font;
  sf::Text turnText;

  bool dfs(int x, int y, int player, std::vector<std::vector<bool>> &visited);
};

#endif
