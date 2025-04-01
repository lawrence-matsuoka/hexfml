#ifndef GAME_HPP
#define GAME_HPP

#include "Board.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Game {
public:
  Game(Board &board, sf::RenderWindow &window);
  void handleClick(sf::Vector2i mousePosition);
  void draw(sf::RenderWindow &window);

private:
  Board &board;
  sf::RenderWindow &window;
  int rows, columns;
  float radius;
  bool playerTurn; // true = Player 1, false = Player 2
  std::vector<std::vector<int>> boardState;

  sf::CircleShape piece;
};

#endif
