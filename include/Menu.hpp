#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

enum MenuResult { Nothing, Local, Online, Settings, Credits, Exit };

class Menu {
public:
  Menu(sf::RenderWindow &window);
  MenuResult show();

private:
  sf::RenderWindow &window;
  std::vector<sf::RectangleShape> buttons;
  std::vector<sf::Text> buttonLabels;
  sf::Font font;
  MenuResult handleClick(int x, int y);
};

#endif
