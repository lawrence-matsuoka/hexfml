#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include "Move.hpp"
#include "PauseMenu.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <functional>
#include <limits>
#include <vector>

enum class GameOverChoice { PlayAgain, QuitToMenu, QuitToDesktop };

class Game {
public:
  void run();
  Game(Board &board, sf::RenderWindow &window);
  bool gamePaused = false;
  void handleClick(sf::Vector2i mousePosition);
  void draw(sf::RenderWindow &window);
  void resetGame();
  int checkWinner();
  GameOverChoice displayWinner(int winner);

  bool quitToMenu = false;

  struct Move {
    int x;
    int y;

    // Serialize this move into a packet
    sf::Packet toPacket() const {
      sf::Packet packet;
      packet << x << y;
      return packet;
    }

    // Deserialize a packet into a Move
    static Move fromPacket(sf::Packet &packet) {
      Move move;
      packet >> move.x >> move.y;
      return move;
    }
  };

  Move getMove();
  void applyMove(const Move &);
  bool isGameOver();
  sf::RenderWindow &getWindow();
  void draw();

  sf::Text turnText;

private:
  Board &board;
  sf::RenderWindow &window;
  PauseMenu pauseMenu;
  int rows, columns;
  float radius;
  bool playerTurn; // true = Player 1, false = Player 2
  std::vector<std::vector<int>> boardState;

  sf::CircleShape piece;
  sf::CircleShape hoverPiece;

  // Textures
  sf::Texture blackTexture;
  sf::Texture whiteTexture;

  // Fonts
  sf::Font font;

  bool dfs(int x, int y, int player, std::vector<std::vector<bool>> &visited);
};

#endif
