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
  // Show the pause menu and return result (click)
  PauseMenuResult show();

private:
  sf::RenderWindow &window;      // Reference to the SFML window
  sf::Font font;                 // Font for buttons and labels
  sf::RectangleShape background; // Transparent background for fading effect
  std::vector<sf::RectangleShape> buttons; // Vector to hold button shapes
  std::vector<sf::Text> buttonLabels;      // Vector to hold button labels
  std::vector<std::function<PauseMenuResult()>> actions; // Callback actions

  static constexpr float buttonWidth = 320;
  static constexpr float buttonHeight = 70;
  static constexpr float startY = 200;
  static constexpr float spacing = 100;

  float startX; // Calculated X position to center buttons

  PauseMenuResult handleClick(int x, int y);
};

#endif // PAUSEMENU_HPP
