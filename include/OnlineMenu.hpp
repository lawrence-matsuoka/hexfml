#ifndef ONLINE_MENU_HPP
#define ONLINE_MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <functional>
#include <string>
#include <vector>

class OnlineMenu {
public:
  OnlineMenu(sf::RenderWindow &window);
  void show();
  void handleButtonClicks(int x, int y);
  void updateIPText();

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::Text ipAddressText;
  sf::Text hostButtonLabel;
  sf::Text joinButtonLabel;
  sf::Text backButtonLabel;
  sf::RectangleShape hostButton;
  sf::RectangleShape joinButton;
  sf::RectangleShape backButton;
  sf::RectangleShape ipFieldBackground;
  sf::Text ipFieldText;
  std::vector<sf::RectangleShape> buttons;
  std::vector<sf::Text> buttonLabels;
  sf::RectangleShape cursor;

  bool cursorVisible;
  bool backPressed;
  unsigned int maxCharacterLimit;

  sf::TcpSocket socket;
  std::string ipAddress;
};

#endif
