#include "../include/Credits.hpp"
#include "../include/Audio.hpp"
#include <iostream>

Credits::Credits(sf::RenderWindow &window) : window(window) {
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  creditsText.setFont(font);
  creditsText.setString(
      "Developed by: Lawrence Matsuoka\nMusic by: Nick Roberts\nAssets from:");
  creditsText.setCharacterSize(40);
  creditsText.setFillColor(sf::Color::White);
  creditsText.setOrigin(creditsText.getLocalBounds().width / 2,
                        creditsText.getLocalBounds().height / 2);
  creditsText.setPosition(800, 200);

  backButton.setFont(font);
  backButton.setString("Back");
  backButton.setCharacterSize(40);
  backButton.setFillColor(sf::Color::White);
  backButton.setPosition(50, 800);
}

void Credits::show() {
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
        Sounds::playSound("click");
        if (isHovered) {
          return;
        }
      }
    }

    window.clear(sf::Color(85, 115, 85));
    window.draw(creditsText);
    window.draw(backButton);
    window.display();
  }
}
