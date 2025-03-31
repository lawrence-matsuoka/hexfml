#ifndef BOARD_HPP
#define BOARD_HPP

#include <SFML/Graphics.hpp>

class Board {
private:
  int rows, columns;
  float radius;
  sf::Texture woodTexture;
  sf::CircleShape hexagon;
  sf::View view;

public:
  Board(int rows, int columns, float radius, sf::RenderWindow &window);
  void updateView(sf::RenderWindow &window, int newWidth, int newHeight);
  void draw(sf::RenderWindow &window, unsigned int windowX,
            unsigned int windowY);
  void drawBackgroundBorder(sf::RenderWindow &window, sf::Color color,
                            sf::Vector2f size, sf::Vector2f position,
                            float angle);
};

#endif // BOARD_HPP
