#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include <SFML/Graphics.hpp>

int main() {
  unsigned int windowX = 1600;
  unsigned int windowY = 900;
  sf::RenderWindow window({windowX, windowY}, "hex++", sf::Style::Default,
                          sf::ContextSettings(0, 0, 8));

  Board board(11, 11, 40, window);
  Game game(board, window);

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
        game.handleClick(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      }
    }

    board.draw(window, windowX, windowY);
    game.draw(window);
    window.display();
  }
}
