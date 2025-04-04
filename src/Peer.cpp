#include "../include/Peer.hpp"
#include <iostream>

Peer::Peer() : myTurn(false) {}

bool Peer::host(unsigned short port) {
  sf::TcpListener listener;
  listener.setBlocking(true);
  if (listener.listen(port) != sf::Socket::Done) {
    std::cerr << "Error listening on port\n";
    return false;
  }

  std::cout << "Waiting for connection...\n";
  if (listener.accept(socket) != sf::Socket::Done) {
    std::cerr << "Error accepting connection\n";
    return false;
  }

  myTurn = true; // Host goes first
  std::cout << "Client connected\n";
  return true;
}

bool Peer::join(const sf::IpAddress &ip, unsigned short port) {
  socket.setBlocking(true);
  if (socket.connect(ip, port) != sf::Socket::Done) {
    std::cerr << "Failed to connect to host\n";
    return false;
  }

  myTurn = false;
  std::cout << "Connected to host\n";
  return true;
}

void Peer::sendMove(const Game::Move &move) {
  sf::Packet packet = move.toPacket();
  socket.send(packet);
  myTurn = false;
}

Game::Move Peer::receiveMove() {
  sf::Packet packet;
  socket.receive(packet);
  myTurn = true;
  return Game::Move::fromPacket(packet);
}

bool Peer::isMyTurn() const { return myTurn; }
