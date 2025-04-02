#include "../include/Settings.hpp"
#include <iostream>

Settings::Settings(sf::RenderWindow &window) : window(window), volume(50.f) {
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  //    text.setOrigin(text.getLocalBounds().width / 2,
  //                   text.getLocalBounds().height / 2);
  //    text.setPosition(startX + buttonWidth / 2,
  //                     startY + i * spacing + buttonHeight / 2 - 4);

  settingsTitle.setFont(font);
  settingsTitle.setString("Settings");
  settingsTitle.setCharacterSize(60);
  settingsTitle.setFillColor(sf::Color::White);
  settingsTitle.setOrigin(settingsTitle.getLocalBounds().width / 2,
                          settingsTitle.getLocalBounds().height / 2);
  settingsTitle.setPosition(800, 200);

  volumeLabel.setFont(font);
  volumeLabel.setString("Volume: " + std::to_string((int)volume));
  volumeLabel.setCharacterSize(40);
  volumeLabel.setFillColor(sf::Color::White);
  volumeLabel.setOrigin(volumeLabel.getLocalBounds().width / 2,
                        volumeLabel.getLocalBounds().height / 2);
  volumeLabel.setPosition(800, 300);

  backButton.setFont(font);
  backButton.setString("Back");
  backButton.setCharacterSize(40);
  backButton.setFillColor(sf::Color::White);
  backButton.setPosition(50, 800);
}

void Settings::show() {
  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
    bool isHovered = backButton.getGlobalBounds().contains(mouseWorldPos);
    backButton.setFillColor(isHovered ? sf::Color::Yellow : sf::Color::White);
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        return;
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        if (isHovered) {
          return; // Exit settings screen
        }
      }
    }

    window.clear(sf::Color(85, 115, 85));
    window.draw(settingsTitle);
    window.draw(volumeLabel);
    window.draw(backButton);
    window.display();
  }
}
