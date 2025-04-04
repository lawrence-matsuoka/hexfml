#pragma once
#include <SFML/Network.hpp>

struct Move {
  int x;
  int y;

  sf::Packet toPacket() const {
    sf::Packet packet;
    packet << x << y;
    return packet;
  }

  static Move fromPacket(sf::Packet &packet) {
    Move m;
    packet >> m.x >> m.y;
    return m;
  }
};
