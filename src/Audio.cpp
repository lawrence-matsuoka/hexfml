#include "../include/Audio.hpp"
#include <iostream>

// Define static variables
sf::Music Sounds::backgroundMusic;
std::map<std::string, sf::SoundBuffer> Sounds::soundBuffers;
std::map<std::string, sf::Sound> Sounds::sounds;

static float soundVolume = 50.f; // Default sound volume
static float musicVolume = 50.f; // Default music volume

void Sounds::setSoundVolume(float volume) {
  soundVolume = volume;
  for (auto &sound : sounds) {
    sound.second.setVolume(soundVolume); // Set volume for each sound
  }
}
void Sounds::setMusicVolume(float volume) {
  musicVolume = volume;
  backgroundMusic.setVolume(musicVolume); // Set volume for music
}

void Sounds::playMusic() {
  if (!backgroundMusic.openFromFile(
          "assets/music/Nick-Roberts_ph1l0s0phy.ogg")) {
    std::cerr << "Error loading background music" << std::endl;
    return;
  }

  backgroundMusic.setLoop(true); // Set the music to loop indefinitely
  backgroundMusic.setVolume(
      musicVolume);       // Optionally set the volume (0 to 100 scale)
  backgroundMusic.play(); // Start playing the music
}
void Sounds::stopMusic() {
  backgroundMusic.stop(); // Stops the music when called
}

// Load all sound effects
void Sounds::loadSounds() {
  // List of sounds to load
  std::map<std::string, std::string> soundFiles = {
      {"click", "assets/sounds/click.wav"},
      {"piece", "assets/sounds/edited-wood03.ogg"}};

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
