#include "../include/Credits.hpp"
#include <iostream>

Credits::Credits(sf::RenderWindow &win) : window(win) {
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font\n";
  }

  creditText.setFont(font);
  creditText.setCharacterSize(40);
  creditText.setFillColor(sf::Color::Black);
  creditText.setString("Game Created by You\nMore credits to be added...");
  creditText.setPosition(100, 200);

  back = creditText;
  back.setString("Back");
  back.setPosition(100, 400);
}

void Credits::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    drawCredits();
  }
}

void Credits::drawCredits() {
  window.clear(sf::Color(85, 115, 85));
  window.draw(creditText);
  window.draw(back);
  window.display();
}
