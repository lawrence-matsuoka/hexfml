#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Menu {
public:
  enum MenuResult { Nothing, Local, Online, Settings, Credits, Exit };

  Menu(sf::RenderWindow &window);
  MenuResult show(); // ✅ This function displays the menu and waits for input

private:
  sf::RenderWindow &window;
  std::vector<sf::RectangleShape> buttons; // Store buttons
  std::vector<sf::Text> buttonLabels;      // Store button labels (text)
  sf::Font font;
  MenuResult handleClick(int x, int y);

  // Sounds
  sf::SoundBuffer pieceBuffer;
  sf::Sound pieceSound;
  // Background music
  sf::Music music;
};

#endif

