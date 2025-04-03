#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>

enum class PauseMenuResult { Resume, Settings, QuitToMenu, QuitToDesktop };

class PauseMenu {
public:
  // Constructor with window, labels, and actions
  PauseMenu(sf::RenderWindow &window, const std::vector<std::string> &labels,
            const std::vector<std::function<PauseMenuResult()>> &actions);
  PauseMenuResult show();

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::RectangleShape background;
  std::vector<sf::RectangleShape> buttons;
  std::vector<sf::Text> buttonLabels;
  std::vector<std::function<PauseMenuResult()>> actions;

  static constexpr float buttonWidth = 320;
  static constexpr float buttonHeight = 70;
  static constexpr float startY = 200;
  static constexpr float spacing = 100;

  float startX;

  PauseMenuResult handleClick(int x, int y);
};

#endif
