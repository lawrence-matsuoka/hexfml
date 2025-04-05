#include "../include/Peer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

Peer::Peer() : myTurn(false), gameOver(false) {
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

  randomizeTurn(); // Randomize who goes first
  std::cout << "Client connected\n";
  return true;
}

bool Peer::join(const sf::IpAddress &ip, unsigned short port) {
  socket.setBlocking(true);
  if (socket.connect(ip, port) != sf::Socket::Done) {
    std::cerr << "Failed to connect to host\n";
    return false;
  }

  randomizeTurn(); // Randomize who goes first
  std::cout << "Connected to host\n";
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

void Peer::randomizeTurn() {
  myTurn = std::rand() % 2 == 0; // Randomly decide who goes first
}
