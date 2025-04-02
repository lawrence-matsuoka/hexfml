#ifndef SOUNDS_HPP
#define SOUNDS_HPP

#include <SFML/Audio.hpp>
#include <map>
#include <string>

class Sounds {
public:
  // Load all sound effects
  static void loadSounds();

  // Play a specific sound effect
  static void playSound(const std::string &soundName);

private:
  static std::map<std::string, sf::SoundBuffer> soundBuffers;
  static std::map<std::string, sf::Sound> sounds;
};

#endif // SOUNDS_HPP
