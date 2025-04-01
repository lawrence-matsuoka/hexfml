#include "../include/Board.hpp"
#include <SFML/Graphics.hpp>

int main() {
  unsigned int windowX = 1600;
  unsigned int windowY = 900;
  sf::RenderWindow window({windowX, windowY}, "hex++", sf::Style::Default,
                          sf::ContextSettings(0, 0, 8));

  // Board board(11, 11, 20, window);
  Board board(11, 11, 40, window);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::Resized) {
        board.updateView(window, event.size.width, event.size.height);
      }
    }

    board.draw(window, windowX, windowY);
    window.display();
  }
}
