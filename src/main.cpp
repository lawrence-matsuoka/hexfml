#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include "../include/Menu.hpp"
#include "../include/PauseMenu.hpp"
#include "../include/Sounds.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

int main() {
  unsigned int windowX = 1600;
  unsigned int windowY = 900;
  sf::RenderWindow window({windowX, windowY}, "HexFML", sf::Style::Default,
                          sf::ContextSettings(0, 0, 8));

  Board board(11, 11, 40, window);
  Game game(board, window);
  bool gamePaused = false;

  // Define the actions for the pause menu buttons
  std::vector<std::string> menuLabels = {"Resume", "Settings", "Quit to Menu",
                                         "Quit to Desktop"};
  std::vector<std::function<PauseMenuResult()>> menuActions = {
      [&]() {
        gamePaused = false;
        return PauseMenuResult::Resume;
      },                                             // Resume
      [&]() { return PauseMenuResult::Settings; },   // Settings
      [&]() { return PauseMenuResult::QuitToMenu; }, // Quit to Menu
      [&]() {
        window.close();
        return PauseMenuResult::QuitToDesktop;
      } // Quit to Desktop
  };

  PauseMenu pauseMenu(window, menuLabels, menuActions);
  Menu mainMenu(window);

  Sounds::loadSounds();

  while (window.isOpen()) {
    MenuResult result = mainMenu.show();
    if (result == Local) {
      gamePaused = false;
      game.run(); // Start the game
    }
    if (result == Exit) {
      window.close();
      return 0;
    }
  }
  
  return 0;
}
