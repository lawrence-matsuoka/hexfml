#include "../include/Menu.hpp"
#include "../include/Credits.hpp"
#include "../include/Settings.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

// Constructor
Menu::Menu(sf::RenderWindow &window) : window(window) {
  // Load font
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  // Define button labels
  std::vector<std::string> labels = {"Local", "Online", "Settings", "Credits",
                                     "Quit to Desktop"};

  float buttonWidth = 320;
  float buttonHeight = 70;
  float startX = 640;
  float startY = 200;
  float spacing = 100;

  for (unsigned int i = 0; i < labels.size(); i++) {
    // Create a rectangle for each button
    sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
    button.setFillColor(
        sf::Color(100, 150, 100)); // Button colours, currently sage green
    button.setPosition(startX, startY + i * spacing);

    // Create text for each button
    sf::Text text;
    text.setFont(font);
    text.setString(labels[i]);
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::White);

    // Center the text in the button
    text.setOrigin(text.getLocalBounds().width / 2,
                   text.getLocalBounds().height / 2);
    text.setPosition(startX + buttonWidth / 2,
                     startY + i * spacing + buttonHeight / 2 - 4);

    // Store buttons and labels in their respective vectors
    buttons.push_back(button);
    buttonLabels.push_back(text);
  }
}

// Show menu and return user selection
MenuResult Menu::show() {
  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
    bool mouseOverButton = false;
    for (unsigned int i = 0; i < buttons.size(); i++) {
      if (buttons[i].getGlobalBounds().contains(mouseWorldPos.x,
                                                mouseWorldPos.y)) {
        // Highlight button text on hover
        buttonLabels[i].setFillColor(sf::Color::Yellow); // Highlight color
        mouseOverButton = true;
      } else {
        buttonLabels[i].setFillColor(sf::Color::White); // Normal color
      }
    }

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
        return Exit;
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left && mouseOverButton) {
        MenuResult result = handleClick(mouseWorldPos.x, mouseWorldPos.y);
        if (result == Settings) {
          // Call the settings screen
          class Settings settings(window);
          settings.show();
          return Nothing; // Stay in menu until settings are closed
        } else if (result == Credits) {
          // Call the credits screen
          class Credits credits(window);
          credits.show();
          return Nothing; // Stay in menu until credits are closed
        }
        return result;
      }
    }

    window.clear(sf::Color(85, 115, 85)); // Sage green background
    for (auto &button : buttons)
      window.draw(button);
    for (auto &text : buttonLabels)
      window.draw(text);
    window.display();
  }
  return Exit;
}

// Handle mouse click
MenuResult Menu::handleClick(int x, int y) {
  for (unsigned int i = 0; i < buttons.size(); i++) {
    if (buttons[i].getGlobalBounds().contains(x, y)) {
      if (i == 0)
        return Local;
      if (i == 1)
        return Online;
      if (i == 2)
        return Settings;
      if (i == 3)
        return Credits;
      if (i == 4)
        return Exit;
    }
  }
  return Nothing;
}
