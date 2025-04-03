#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <functional>

class Settings {
public:
  Settings(sf::RenderWindow &window);
  ~Settings() = default;
  void show();
  void loadSettings();
  void saveSettings();
  void updateSliderFromMouse(float mouseX, sf::RectangleShape &background,
                             sf::RectangleShape &slider, float &volume,
                             std::function<void(float)> volumeSetter);

private:
  sf::RenderWindow &window;
  sf::Font font;
  sf::Text settingsTitle;
  sf::Text volumeLabel;
  sf::Text musicLabel;
  sf::Text backButton;
  sf::RectangleShape soundSlider;
  sf::RectangleShape musicSlider;
  sf::RectangleShape soundSliderBackground;
  sf::RectangleShape musicSliderBackground;
  float soundVolume; // Volume for sound effects
  float musicVolume; // Volume for music
  void updateSlider(sf::RectangleShape &slider, float value);
  void drawSlider(sf::RenderWindow &window, sf::RectangleShape &slider,
                  float value);
  bool isSliding = false;
};

#endif
