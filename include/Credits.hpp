#ifndef CREDITS_HPP
#define CREDITS_HPP

#include <SFML/Graphics.hpp>

class Credits {
public:
  Credits(sf::RenderWindow &window);
  void show();

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::Text creditsText;
  sf::Text backButton;
};

#endif
