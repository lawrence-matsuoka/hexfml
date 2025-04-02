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

  Menu menu(window);
  Menu::MenuResult result = menu.show();

  if (result == Menu::Local) { // ✅ If "Local" is clicked, start the game
    window.close(); // Close the menu window before launching the game

    sf::RenderWindow gameWindow(sf::VideoMode(1600, 900), "hex++ Game",
                                sf::Style::Default);
    Board board(11, 11, 40, gameWindow);
    Game game(board, gameWindow);

    game.run(); // ✅ Runs the game loop inside Game
  }

  return 0;
}
