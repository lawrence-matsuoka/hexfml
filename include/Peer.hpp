#pragma once
#include "Game.hpp"
#include <SFML/Network.hpp>
#include <functional>

class Peer {
public:
  Peer();

  bool host(unsigned short port = 54000);
  bool join(const sf::IpAddress &ip, unsigned short port = 54000);

  bool beginHosting(unsigned short port);
  void tryAccept();

  void sendMove(const Game::Move &move);
  Game::Move receiveMove();

  bool isMyTurn() const;
  bool isGameOver() const;
  void setGameOver(bool gameOver);

  void closeConnection();

  void setStatusCallBack(std::function<void(const std::string &)> callBack);

  std::function<void(const std::string &)> statusCallBack;

  bool isHost;
  bool goesFirst = randomizeTurn();

  bool myTurn;

  bool isConnected() const;

private:
  bool randomizeTurn();

  sf::TcpSocket socket;
  sf::TcpListener listener;

  bool gameOver;

  bool connected;
};
