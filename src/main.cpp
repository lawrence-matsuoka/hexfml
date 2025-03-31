#include <SFML/Graphics.hpp>
#include "../include/Board.hpp"

int main() {
    sf::RenderWindow window({1600, 900}, "hex++", sf::Style::Default, sf::ContextSettings(0, 0, 8));
    
    Board board(11, 11, 20, window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                board.updateView(window, event.size.width, event.size.height);
            }
        }

        board.draw(window);
        window.display();
    }
}

