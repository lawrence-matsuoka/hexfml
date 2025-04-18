#include "../include/OnlineMenu.hpp"
#include "../include/Audio.hpp"
#include "../include/Board.hpp"
#include "../include/Game.hpp"
#include "../include/Move.hpp"
#include "../include/OnlineGame.hpp"
#include "../include/Peer.hpp"
#include <SFML/Network.hpp>
#include <iostream>

OnlineMenu::OnlineMenu(sf::RenderWindow &window)
    : window(window), cursorVisible(true), backPressed(false),
      maxCharacterLimit(15) {
  // Load font
  if (!font.loadFromFile("assets/fonts/honeycomb-happiness-font/"
                         "HoneycombHappiness-ywnRm.ttf")) {
    std::cerr << "Error loading font" << std::endl;
  }

  // Status text for sockets
  statusText.setFont(font);
  statusText.setCharacterSize(40);
  statusText.setFillColor(sf::Color::White);
  statusText.setPosition(100, 800); // bottom left

  // Set up IP Address text
  ipAddressText.setFont(font);
  updateIPText();
  ipAddressText.setCharacterSize(30);
  ipAddressText.setFillColor(sf::Color::White);
  ipAddressText.setPosition(100,
                            100); // Position the text at the top of the screen

  // Set up Port text (Below IP)
  portText.setFont(font);
  portText.setString("Default Port: 54000");
  portText.setCharacterSize(20);
  portText.setFillColor(sf::Color::White);
  portText.setPosition(100, 140);

  // Set up Port Forwarding "Link"
  portLink.setFont(font);
  portLink.setString("How to Port Forward?");
  portLink.setCharacterSize(20);
  portLink.setFillColor(sf::Color::White);
  portLink.setPosition(100, 160);

  // Host button setup
  hostButton.setSize(sf::Vector2f(320, 70));
  hostButton.setFillColor(sf::Color(100, 150, 100));
  hostButton.setPosition(640, 200);
  hostButtonLabel.setFont(font);
  hostButtonLabel.setString("Host");
  hostButtonLabel.setCharacterSize(40);
  hostButtonLabel.setFillColor(sf::Color::White);
  hostButtonLabel.setPosition(
      640 + 160 - hostButtonLabel.getLocalBounds().width / 2,
      200 + 27 - hostButtonLabel.getLocalBounds().height / 2);

  // Join button setup
  joinButton.setSize(sf::Vector2f(320, 70));
  joinButton.setFillColor(sf::Color(100, 150, 100));
  joinButton.setPosition(640, 300);
  joinButtonLabel.setFont(font);
  joinButtonLabel.setString("Join");
  joinButtonLabel.setCharacterSize(40);
  joinButtonLabel.setFillColor(sf::Color::White);
  joinButtonLabel.setPosition(
      640 + 160 - joinButtonLabel.getLocalBounds().width / 2,
      300 + 27 - joinButtonLabel.getLocalBounds().height / 2);

  // IP field setup (initially empty)
  ipFieldBackground.setSize(sf::Vector2f(320, 50));
  ipFieldBackground.setFillColor(sf::Color(60, 60, 60));
  ipFieldBackground.setPosition(640, 400);
  ipFieldText.setFont(font);
  ipFieldText.setString(""); // Empty initially
  ipFieldText.setCharacterSize(30);
  ipFieldText.setFillColor(sf::Color::White);
  ipFieldText.setPosition(640 + 10, 400 + 10);
  // Cursor for IP field
  cursor.setSize(sf::Vector2f(2, 30)); // A thin vertical line (cursor)
  cursor.setFillColor(sf::Color::White);
  cursor.setPosition(ipFieldText.getPosition().x +
                         ipFieldText.getGlobalBounds().width,
                     ipFieldText.getPosition().y);

  // Back button setup
  backButton.setSize(sf::Vector2f(320, 70));
  backButton.setFillColor(sf::Color(100, 150, 100));
  backButton.setPosition(640, 500);
  backButtonLabel.setFont(font);
  backButtonLabel.setString("Back");
  backButtonLabel.setCharacterSize(40);
  backButtonLabel.setFillColor(sf::Color::White);
  backButtonLabel.setPosition(
      640 + 160 - backButtonLabel.getLocalBounds().width / 2,
      500 + 27 - backButtonLabel.getLocalBounds().height / 2);
}

// Inside OnlineMenu.cpp (updated to reflect hover logic)
void OnlineMenu::show() {
  sf::Clock cursorClock;

  while (window.isOpen()) {
    sf::Event event;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    sf::Vector2f mouseWorldPos = window.mapPixelToCoords(mousePosition);
    //  bool mouseOverButton = false;

    // Check if any button is hovered
    hostButtonLabel.setFillColor(sf::Color::White); // Default color
    joinButtonLabel.setFillColor(sf::Color::White);
    backButtonLabel.setFillColor(sf::Color::White);
    portLink.setFillColor(sf::Color::White);

    if (hostButton.getGlobalBounds().contains(mouseWorldPos.x,
                                              mouseWorldPos.y)) {
      hostButtonLabel.setFillColor(
          sf::Color::Yellow); // Hover color for Host button
    }
    if (joinButton.getGlobalBounds().contains(mouseWorldPos.x,
                                              mouseWorldPos.y)) {
      joinButtonLabel.setFillColor(
          sf::Color::Yellow); // Hover color for Join button
    }
    if (backButton.getGlobalBounds().contains(mouseWorldPos.x,
                                              mouseWorldPos.y)) {
      backButtonLabel.setFillColor(
          sf::Color::Yellow); // Hover color for Back button
    }
    if (portLink.getGlobalBounds().contains(mouseWorldPos.x, mouseWorldPos.y)) {
      portLink.setFillColor(sf::Color::Yellow); // Hover color for Back button
    }

    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed &&
          event.mouseButton.button == sf::Mouse::Left) {
        Sounds::playSound("click");
        handleButtonClicks(mouseWorldPos.x, mouseWorldPos.y);
      }
      if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) { // Backspace
          // Remove last character from the string
          std::string currentText = ipFieldText.getString();
          if (!currentText.empty()) {
            currentText.pop_back();
            ipFieldText.setString(currentText);
          }
        } else if (event.text.unicode < 128 && event.text.unicode != 13) {
          std::string currentText = ipFieldText.getString();
          if (currentText.length() < maxCharacterLimit) {
            currentText += static_cast<char>(event.text.unicode);
            ipFieldText.setString(currentText);
          }
        }
      }
      // Handle Enter key
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Enter) {
        handleButtonClicks(
            joinButton.getPosition().x + joinButton.getSize().x / 2,
            joinButton.getPosition().y + joinButton.getSize().y / 2);
      }

      cursor.setPosition(ipFieldText.getPosition().x +
                             ipFieldText.getGlobalBounds().width,
                         ipFieldText.getPosition().y);

      if (backPressed) {
        return; // Break out of the loop, returning control to the main menu
      }
    }
    window.clear(sf::Color(85, 115, 85)); // Sage green background
    window.draw(ipAddressText);
    window.draw(portText);
    window.draw(portLink);
    window.draw(hostButton);
    window.draw(joinButton);
    window.draw(backButton);
    window.draw(ipFieldBackground);
    window.draw(ipFieldText);
    window.draw(hostButtonLabel);
    window.draw(joinButtonLabel);
    window.draw(backButtonLabel);

    if (cursorVisible) {
      window.draw(cursor);
    }
    if (showStatus &&
        statusTimer.getElapsedTime().asSeconds() < statusDuration) {
      window.draw(statusText);
    }

    window.display();
  }
}

void OnlineMenu::handleButtonClicks(int x, int y) {
  Peer peer;
  if (hostButton.getGlobalBounds().contains(x, y)) {

    peer.setStatusCallBack(
        [this](const std::string &msg) { this->setStatusMessage(msg); });

    if (peer.beginHosting(54000)) {
      // Show "Waiting for connection..." screen
      while (!peer.isConnected() && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            window.close();
            return;
          }
          if (event.type == sf::Event::KeyPressed &&
              event.key.code == sf::Keyboard::Escape) {
            return; // Allow user to cancel
          }
        }

        window.clear(sf::Color(85, 115, 85));

        sf::Text waitingText;
        waitingText.setFont(font);
        waitingText.setString("Waiting for opponent to join...");
        waitingText.setCharacterSize(40);
        waitingText.setFillColor(sf::Color::White);
        waitingText.setPosition(100, 400);
        window.draw(waitingText);

        window.display();

        peer.tryAccept(); // Non-blocking attempt to accept connection
        sf::sleep(sf::milliseconds(100));
      }

      if (!window.isOpen())
        return;

      Board board(11, 11, 40, window);
      Game game(board, peer, window);
      ::runOnlineGame(board, game, peer);
    }

  } else if (joinButton.getGlobalBounds().contains(x, y)) {
    std::string ip = ipFieldText.getString().toAnsiString();
    if (peer.join(ip)) {
      std::string message = "Joining host at " + ip;
      if (peer.statusCallBack) {
        peer.statusCallBack(message);
      }
      Board board(11, 11, 40, window);
      Game game(board, peer, window);
      ::runOnlineGame(board, game, peer);
    }
  } else if (backButton.getGlobalBounds().contains(x, y)) {
    backPressed = true;
  } else if (portLink.getGlobalBounds().contains(x, y)) {
// Open the port forwarding link in browser
#ifdef _WIN32
    system("start https://portforward.com/");
#elif __APPLE__
    system("open https://portforward.com/");
#elif __linux__
    system("xdg-open https://portforward.com/");
#endif
  }
}

void OnlineMenu::updateIPText() {
  // Get the user's IP address and display it
  sf::IpAddress ip = sf::IpAddress::getPublicAddress();
  ipAddress = "Your IP: " + ip.toString();
  ipAddressText.setString(ipAddress);
}

void OnlineMenu::setStatusMessage(const std::string &message) {
  statusText.setString(message);
  statusTimer.restart();
  showStatus = true;
}
