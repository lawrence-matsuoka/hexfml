#include "../include/Settings.hpp"
#include "../include/Audio.hpp"
#include <fstream>
#include <functional>
#include <iostream>

Settings::Settings(sf::RenderWindow &window)
    : window(window), soundVolume(50.f), musicVolume(50.f) {
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  settingsTitle.setFont(font);
  settingsTitle.setString("Settings");
  settingsTitle.setCharacterSize(60);
  settingsTitle.setFillColor(sf::Color::White);
  settingsTitle.setOrigin(settingsTitle.getLocalBounds().width / 2,
                          settingsTitle.getLocalBounds().height / 2);
  settingsTitle.setPosition(800, 200);

  volumeLabel.setFont(font);
  volumeLabel.setString("Sound:");
  volumeLabel.setCharacterSize(30);
  volumeLabel.setFillColor(sf::Color::White);
  volumeLabel.setOrigin(volumeLabel.getLocalBounds().width / 2,
                        volumeLabel.getLocalBounds().height / 2);
  volumeLabel.setPosition(800, 300);

  musicLabel.setFont(font);
  musicLabel.setString("Music:");
  musicLabel.setCharacterSize(30);
  musicLabel.setFillColor(sf::Color::White);
  musicLabel.setOrigin(musicLabel.getLocalBounds().width / 2,
                       musicLabel.getLocalBounds().height / 2);
  musicLabel.setPosition(800, 450);

  // Initialize sliders
  soundSliderBackground.setSize(sf::Vector2f(400, 10));
  soundSliderBackground.setFillColor(sf::Color(100, 100, 100));
  soundSliderBackground.setOrigin(
      soundSliderBackground.getLocalBounds().width / 2,
      soundSliderBackground.getLocalBounds().height / 2);
  soundSliderBackground.setPosition(800, 350);

  musicSliderBackground.setSize(sf::Vector2f(400, 10));
  musicSliderBackground.setFillColor(sf::Color(100, 100, 100));
  musicSliderBackground.setOrigin(
      musicSliderBackground.getLocalBounds().width / 2,
      musicSliderBackground.getLocalBounds().height / 2);
  musicSliderBackground.setPosition(800, 500);

  soundSlider.setSize(sf::Vector2f(30, 30));
  soundSlider.setFillColor(sf::Color::Green);
  soundSlider.setOrigin(soundSlider.getLocalBounds().width / 2,
                        soundSlider.getLocalBounds().height / 2);
  soundSlider.setPosition(800, 350);

  musicSlider.setSize(sf::Vector2f(30, 30));
  musicSlider.setFillColor(sf::Color::Green);
  musicSlider.setOrigin(soundSlider.getLocalBounds().width / 2,
                        soundSlider.getLocalBounds().height / 2);
  musicSlider.setPosition(800, 500);

  backButton.setFont(font);
  backButton.setString("Back");
  backButton.setCharacterSize(40);
  backButton.setFillColor(sf::Color::White);
  backButton.setPosition(50, 800);

  loadSettings();

  updateSlider(soundSlider, soundVolume);
  updateSlider(musicSlider, musicVolume);
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
        Sounds::playSound("click");

        // Check if the back button was clicked
        if (isHovered) {
          saveSettings();
          return; // Exit settings screen
        }

        // Check if the mouse click is inside the slider backgrounds to start
        // sliding
        if (soundSliderBackground.getGlobalBounds().contains(mouseWorldPos)) {
          isSliding = true;
          updateSliderFromMouse(mouseWorldPos.x, soundSliderBackground,
                                soundSlider, soundVolume,
                                Sounds::setSoundVolume);
        }
        if (musicSliderBackground.getGlobalBounds().contains(mouseWorldPos)) {
          isSliding = true;
          updateSliderFromMouse(mouseWorldPos.x, musicSliderBackground,
                                musicSlider, musicVolume,
                                Sounds::setMusicVolume);
        }
      }

      if (event.type == sf::Event::MouseButtonReleased &&
          event.mouseButton.button == sf::Mouse::Left) {
        isSliding = false; // Stop sliding when mouse is released
      }
    }

    // If sliding, update the position based on mouse position
    if (isSliding) {
      if (soundSliderBackground.getGlobalBounds().contains(mouseWorldPos)) {
        updateSliderFromMouse(mouseWorldPos.x, soundSliderBackground,
                              soundSlider, soundVolume, Sounds::setSoundVolume);
      }

      if (musicSliderBackground.getGlobalBounds().contains(mouseWorldPos)) {
        updateSliderFromMouse(mouseWorldPos.x, musicSliderBackground,
                              musicSlider, musicVolume, Sounds::setMusicVolume);
      }
    }

    window.clear(sf::Color(85, 115, 85));
    window.draw(settingsTitle);
    window.draw(volumeLabel);
    window.draw(musicLabel);
    window.draw(soundSliderBackground);
    window.draw(musicSliderBackground);
    window.draw(soundSlider);
    window.draw(musicSlider);
    window.draw(backButton);
    window.display();
  }
}

// This function is now declared before usage to fix the undeclared identifier
// error.
void Settings::updateSliderFromMouse(float mouseX,
                                     sf::RectangleShape &background,
                                     sf::RectangleShape &slider, float &volume,
                                     std::function<void(float)> volumeSetter) {
  float leftBound = 600;
  float rightBound = 1000;

  // Clamp mouse X to valid range
  float clampedX = std::clamp(mouseX, leftBound, rightBound);

  // Convert position to volume percentage
  volume = ((clampedX - leftBound) / background.getSize().x) * 100.f;

  // Update slider position
  slider.setPosition(clampedX, slider.getPosition().y);

  // Apply new volume
  volumeSetter(volume);
}

void Settings::updateSlider(sf::RectangleShape &slider, float value) {
  // Set the slider's position based on the volume (from 600 to 1000 range)
  slider.setPosition(600 + (value / 100.f) * 400.f, slider.getPosition().y);
}

void Settings::loadSettings() {
  std::ifstream file("config/settings.txt");
  if (file.is_open()) {
    file >> soundVolume >> musicVolume;
    file.close();
    // Apply loaded settings
    Sounds::setSoundVolume(soundVolume);
    Sounds::setMusicVolume(musicVolume);
    // Update sliders
    updateSlider(soundSlider, soundVolume);
    updateSlider(musicSlider, musicVolume);
  } else {
    std::cerr << "Warning: Could not open settings.txt, using defaults."
              << std::endl;
  }
}

void Settings::saveSettings() {
  std::ofstream file("config/settings.txt");
  if (file.is_open()) {
    file << soundVolume << "\n" << musicVolume;
    file.close();
  } else {
    std::cerr << "Error: Could not open settings.txt for writing!" << std::endl;
  }
}
