#include "../include/Audio.hpp"
#include <iostream>

// Define static variables
std::map<std::string, sf::SoundBuffer> Sounds::soundBuffers;
std::map<std::string, sf::Sound> Sounds::sounds;

// Load all sound effects
void Sounds::loadSounds() {
  // List of sounds to load
  std::map<std::string, std::string> soundFiles = {
      {"click", "assets/sounds/click.wav"},
      {"piece", "assets/sounds/wood03.ogg"}
  };

  // Load sounds into buffers
  for (const auto &[name, file] : soundFiles) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(file)) {
      std::cerr << "Error loading sound: " << file << std::endl;
      continue;
    }
    soundBuffers[name] = buffer;

    // Create sound and attach buffer
    sf::Sound sound;
    sound.setBuffer(soundBuffers[name]);
    sounds[name] = sound;
  }
}

// Play a specific sound effect
void Sounds::playSound(const std::string &soundName) {
  if (sounds.find(soundName) != sounds.end()) {
    sounds[soundName].play();
  } else {
    std::cerr << "Sound not found: " << soundName << std::endl;
  }
}
