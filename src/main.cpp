#include "../include/Audio.hpp"
#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include "../include/Menu.hpp"
#include "../include/OnlineMenu.hpp"
#include "../include/PauseMenu.hpp"
#include "../include/Settings.hpp"
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
      },
      [&]() { return PauseMenuResult::QuitToMenu; },
      [&]() {
        window.close();
        return PauseMenuResult::QuitToDesktop;
      } // Quit to Desktop
  };

  PauseMenu pauseMenu(window, menuLabels, menuActions);
  Menu mainMenu(window);

  Sounds::loadSounds();

  class Settings settings(window);
  settings.loadSettings();

  while (window.isOpen()) {
    // Play music
    Sounds::playMusic();
    MenuResult result = mainMenu.show();
    if (result == Local) {
      game.resetGame();
      Sounds::playMusic();
      gamePaused = false;
      game.run(); // Start the game
    }
    if (result == Online) {
      OnlineMenu onlineMenu(window);
      onlineMenu.show();
    }
    if (result == Exit) {
      Sounds::stopMusic();
      window.close();
      return 0;
    }
  }

  return 0;
}
