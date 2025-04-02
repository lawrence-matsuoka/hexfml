#include "../include/PauseMenu.hpp"
#include "../include/Sounds.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

PauseMenu::PauseMenu(
    sf::RenderWindow &window, const std::vector<std::string> &labels,
    const std::vector<std::function<PauseMenuResult()>> &actions)
    : window(window), actions(actions) {
  // Load font
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  // Calculate startX for centering
  startX = (window.getSize().x - buttonWidth) / 2;

  // Create background for fading effect
  background.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
  background.setFillColor(sf::Color(85, 115, 85)); // Semi-transparent black

  // Create buttons and labels based on input labels and actions
  for (unsigned int i = 0; i < labels.size(); i++) {
    // Create a rectangle for each button
    sf::RectangleShape button(sf::Vector2f(buttonWidth, buttonHeight));
    button.setFillColor(sf::Color(100, 150, 100)); // Button colors
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

PauseMenuResult PauseMenu::handleClick(int x, int y) {
  for (unsigned int i = 0; i < buttons.size(); i++) {
    if (buttons[i].getGlobalBounds().contains(x, y)) {
      return actions[i](); // Call the corresponding action function
    }
  }
  return PauseMenuResult::Resume; // Default action if clicked outside of the
                                  // menu
}

PauseMenuResult PauseMenu::show() {
  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
    bool mouseOverButton = false;

    for (unsigned int i = 0; i < buttons.size(); i++) {
      if (buttons[i].getGlobalBounds().contains(mouseWorldPos.x,
                                                mouseWorldPos.y)) {
        // Highlight button text on hover
        buttonLabels[i].setFillColor(sf::Color::Yellow);
        mouseOverButton = true;
      } else {
        buttonLabels[i].setFillColor(sf::Color::White);
      }
    }

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        return PauseMenuResult::QuitToDesktop; // Exit to Desktop if the window
                                               // is closed
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        // Add sound later
        return PauseMenuResult::Resume; // Pressing escape again closes the menu
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        Sounds::playSound("click");
        if (mouseOverButton) {
          return handleClick(mouseWorldPos.x, mouseWorldPos.y);
        }
      }
    }

    window.clear();
    window.draw(
        background); // Draw the semi-transparent background to fade the screen
    for (auto &button : buttons)
      window.draw(button);
    for (auto &text : buttonLabels)
      window.draw(text);
    window.display();
  }

  return PauseMenuResult::Resume; // Default action if the menu is somehow
                                  // closed
}
