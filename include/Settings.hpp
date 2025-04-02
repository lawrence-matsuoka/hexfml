#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <SFML/Graphics.hpp>

class Settings {
public:
    Settings(sf::RenderWindow &window);
    void show();

private:
    sf::RenderWindow &window;
    sf::Font font;
    sf::Text settingsTitle;
    sf::Text volumeLabel;
    sf::Text backButton;
    float volume;
};

#endif

