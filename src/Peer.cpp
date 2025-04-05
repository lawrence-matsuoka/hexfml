#include "../include/Peer.hpp"
#include <SFML/Network/Packet.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

Peer::Peer() : myTurn(false), gameOver(false), isHost(false) {
  std::srand(std::time(0)); // Initialize random seed
}

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

  isHost = true;

  bool goesFirst = randomizeTurn();
  std::cout << "Client connected\n";

  sf::Packet packet;
  packet << goesFirst;
  socket.send(packet);

  return true;
}

bool Peer::join(const sf::IpAddress &ip, unsigned short port) {
  socket.setBlocking(true);
  if (socket.connect(ip, port) != sf::Socket::Done) {
    std::cerr << "Failed to connect to host\n";
    return false;
  }

  isHost = false;
  std::cout << "Connected to host\n";

  sf::Packet packet;
  socket.receive(packet);

  bool goesFirst;
  packet >> goesFirst;

  myTurn = !goesFirst;

  return true;
}

void Peer::sendMove(const Game::Move &move) {
  sf::Packet packet = move.toPacket();
  socket.send(packet);
  myTurn = false; // Pass turn to the other player
}

Game::Move Peer::receiveMove() {
  sf::Packet packet;
  socket.receive(packet);
  myTurn = true; // This player can make the next move
  return Game::Move::fromPacket(packet);
}

bool Peer::isMyTurn() const { return myTurn; }

bool Peer::isGameOver() const { return gameOver; }

void Peer::setGameOver(bool gameOver) { this->gameOver = gameOver; }

bool Peer::randomizeTurn() { return std::rand() % 2 == 0; }

void Peer::closeConnection() {
  socket.disconnect();
  std::cout << "Connection closed." << std::endl;
}
