#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Graphics.hpp>

class Settings {
public:
  Settings(sf::RenderWindow &window);
  void run();

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::Text muteSound, muteMusic, back;
  bool soundMuted, musicMuted;

  void drawSettings();
  void handleMouseClick(sf::Vector2i mousePos);
  bool isTextClicked(sf::Text &text, sf::Vector2i mousePos);
};

#endif
