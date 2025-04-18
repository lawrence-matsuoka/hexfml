#include "../include/Peer.hpp"
#include <SFML/Network/Packet.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

Peer::Peer() : isHost(false), myTurn(false), gameOver(false), connected(false) {
  std::srand(std::time(0)); // Initialize random seed
}

bool Peer::host(unsigned short port) {
  listener.setBlocking(true);
  if (listener.listen(port) != sf::Socket::Done) {
    if (statusCallBack) {
      statusCallBack("Error listening on port\n");
    }

    return false;
  }

  if (listener.accept(socket) != sf::Socket::Done) {
    if (statusCallBack) {
      statusCallBack("Connection error\n");
    }
    return false;
  }

  isHost = true;
  goesFirst = randomizeTurn();
  myTurn = goesFirst;

  sf::Packet packet;
  packet << goesFirst;
  socket.send(packet);

  connected = true;

  return true;
}

bool Peer::join(const sf::IpAddress &ip, unsigned short port) {
  socket.setBlocking(true);
  if (socket.connect(ip, port) != sf::Socket::Done) {
    if (statusCallBack) {
      statusCallBack("Failed to connect to host\n");
    }
    return false;
  }

  isHost = false;
  if (statusCallBack) {
    statusCallBack("Connected to host\n");
  }

  sf::Packet packet;
  socket.receive(packet);
  connected = true;

  packet >> goesFirst;
  goesFirst = !goesFirst;

  myTurn = goesFirst;

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
  connected = false;
  if (statusCallBack) {
    statusCallBack("Connection closed\n");
  }
}

void Peer::setStatusCallBack(
    std::function<void(const std::string &)> callBack) {
  statusCallBack = callBack;
}

bool Peer::isConnected() const { return connected; }

bool Peer::beginHosting(unsigned short port) {
  listener.setBlocking(false);
  if (listener.listen(port) != sf::Socket::Done) {
    if (statusCallBack)
      statusCallBack("Error listening on port\n");
    return false;
  }
  if (statusCallBack)
    statusCallBack("Waiting for connection...\n");
  return true;
}

void Peer::tryAccept() {
  if (!connected && listener.accept(socket) == sf::Socket::Done) {
    isHost = true;
    connected = true;

    if (statusCallBack)
      statusCallBack("Client connected\n");

    sf::Packet packet;
    packet << goesFirst;
    socket.send(packet);
  }
}

bool Peer::isBlack() const {
  return (isHost && goesFirst) || (!isHost && !goesFirst);
}
