#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include "../include/Menu.hpp"
#include <SFML/Graphics.hpp>

int main() {
  unsigned int windowX = 1600;
  unsigned int windowY = 900;
  sf::RenderWindow window({windowX, windowY}, "HexFML", sf::Style::Default,
                          sf::ContextSettings(0, 0, 8));

  Board board(11, 11, 40, window);
  Game game(board, window);

  while (window.isOpen()) {
    Menu menu(window);
    MenuResult result = menu.show();
    if (result == Local) { // start the game if "Local" is clicked
      game.run(); // Runs the game loop
    }
    if (result == Exit) {
      window.close();
      return 0;
    }
  }
  return 0;
}
