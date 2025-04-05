#include "../include/OnlineGame.hpp"
#include <SFML/Window.hpp>

void runOnlineGame(Game &game, Peer &peer) {
  game.resetGame();

  while (game.getWindow().isOpen()) {
    if (game.quitToMenu) {
      break;
    }
    if (peer.isMyTurn()) {
      Game::Move move = game.getMove();
      game.applyMove(move); // Apply it locally
      peer.sendMove(move);  // Send to opponent
    } else {
      Game::Move move = peer.receiveMove();
      game.applyMove(move);
    }

    game.draw(game.getWindow());

    if (game.isGameOver()) {
      break;
    }
  }
}
