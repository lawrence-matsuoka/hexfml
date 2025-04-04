#pragma once
#include "Game.hpp"
#include <SFML/Network.hpp>

class Peer {
public:
  Peer(); 
  bool host(unsigned short port = 54000);
  bool join(const sf::IpAddress &ip, unsigned short port = 54000);

  void sendMove(const Game::Move &move);
  Game::Move receiveMove();

  bool isMyTurn() const;

private:
  sf::TcpSocket socket;
  bool myTurn;
};

