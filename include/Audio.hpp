#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <SFML/Audio.hpp>
#include <map>
#include <string>

class Sounds {
public:
  // Music
  static void playMusic();
  static void stopMusic();

  // Load all sound effects
  static void loadSounds();

  // Play a specific sound effect
  static void playSound(const std::string &soundName);

private:
  static sf::Music backgroundMusic;
  static std::map<std::string, sf::SoundBuffer> soundBuffers;
  static std::map<std::string, sf::Sound> sounds;
};

#endif
