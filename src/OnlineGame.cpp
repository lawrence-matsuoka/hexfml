#include "../include/OnlineGame.hpp"
#include <SFML/Window.hpp>

void updateTurnText(Game &game, Peer &peer) {
  game.turnText.setPosition(50, 800);

  bool myTurn = peer.isMyTurn();
  bool iAmBlack =
      (peer.isHost && peer.goesFirst) || (!peer.isHost && !peer.goesFirst);

  if (myTurn) {
    game.turnText.setString(iAmBlack ? "Your turn (Black)"
                                     : "Your turn (White)");
    game.turnText.setFillColor(iAmBlack ? sf::Color::Black : sf::Color::White);
  } else {
    game.turnText.setString(iAmBlack ? "Opponent's turn (White)"
                                     : "Opponent's turn (Black)");
    game.turnText.setFillColor(iAmBlack ? sf::Color::White : sf::Color::Black);
  }
}

void runOnlineGame(Board &board, Game &game, Peer &peer) {
  game.resetGame();

  while (game.getWindow().isOpen()) {
    if (game.quitToMenu) {
      peer.closeConnection();
      break;
    }

    if (peer.isMyTurn()) {
      Game::Move move = game.getMove();
      game.applyMove(move); // Apply it locally
      peer.sendMove(move);  // Send to opponent
      updateTurnText(game, peer);
    } else {
      Game::Move move = peer.receiveMove();
      game.applyMove(move);
      updateTurnText(game, peer);
    }

    game.getWindow().clear(sf::Color(85, 115, 85)); // Dark sage green
    board.draw(game.getWindow(), 1600, 900);
    game.draw(game.getWindow());
    game.getWindow().display();

    if (game.isGameOver()) {
      break;
    }
  }
}
