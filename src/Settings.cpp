#include "../include/Settings.hpp"
#include <iostream>

Settings::Settings(sf::RenderWindow &win)
    : window(win), soundMuted(false), musicMuted(false) {
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font\n";
  }

  muteSound.setFont(font);
  muteSound.setCharacterSize(50);
  muteSound.setFillColor(sf::Color::Black);
  muteSound.setString("Mute Sound: OFF");
  muteSound.setPosition(100, 200);

  muteMusic = muteSound;
  muteMusic.setString("Mute Music: OFF");
  muteMusic.setPosition(100, 300);

  back = muteSound;
  back.setString("Back");
  back.setPosition(100, 400);
}

void Settings::run() {
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseButtonPressed &&
                 event.mouseButton.button == sf::Mouse::Left) {
        handleMouseClick(
            sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
      }
    }
    drawSettings();
  }
}

void Settings::drawSettings() {
  window.clear(sf::Color(85, 115, 85));
  window.draw(muteSound);
  window.draw(muteMusic);
  window.draw(back);
  window.display();
}

void Settings::handleMouseClick(sf::Vector2i mousePos) {
  if (isTextClicked(muteSound, mousePos)) {
    soundMuted = !soundMuted;
    muteSound.setString(soundMuted ? "Mute Sound: ON" : "Mute Sound: OFF");
  } else if (isTextClicked(muteMusic, mousePos)) {
    musicMuted = !musicMuted;
    muteMusic.setString(musicMuted ? "Mute Music: ON" : "Mute Music: OFF");
  } else if (isTextClicked(back, mousePos)) {
    return;
  }
}

bool Settings::isTextClicked(sf::Text &text, sf::Vector2i mousePos) {
  return text.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                         static_cast<float>(mousePos.y));
}
