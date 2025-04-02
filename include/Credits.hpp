#ifndef CREDITS_HPP
#define CREDITS_HPP

#include <SFML/Graphics.hpp>

class Credits {
public:
  Credits(sf::RenderWindow &window);
  void run();

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::Text creditText, back;

  void drawCredits();
  void handleMouseClick(sf::Vector2i mousePos);
  bool isTextClicked(sf::Text &text, sf::Vector2i mousePos);
};

#endif
