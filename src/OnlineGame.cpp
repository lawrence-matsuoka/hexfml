#include "../include/OnlineGame.hpp"
#include <SFML/Window.hpp>

void runOnlineGame(Game &game, Peer &peer) {
  game.resetGame();

  while (game.getWindow().isOpen()) {
    if (peer.isMyTurn()) {
      Game::Move move = game.getMove(); // Use Game::Move instead of Move
      game.applyMove(move);             // Apply it locally
      peer.sendMove(move);              // Send to opponent
    } else {
      Game::Move move = peer.receiveMove(); // Use Game::Move instead of Move
      game.applyMove(move);                 // Apply it locally
    }

    game.draw(game.getWindow());

    if (game.isGameOver()) {
      break;
    }
  }
}
