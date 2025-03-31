#include <SFML/Graphics.hpp>
#include <cmath>

int rows = 11;
int columns = 11;

int main() {
  sf::RenderWindow window({1600, 900}, "hex++", sf::Style::Default,
                          sf::ContextSettings(0, 0, 8));

  int radius = 20;
  // Hexagon is equivalent to circle with 6 sides
  sf::CircleShape hexagon(radius, 6);
  hexagon.setOutlineColor(sf::Color::Black);
  hexagon.setOutlineThickness(1);
  hexagon.setFillColor(sf::Color::White);
  hexagon.setOrigin(radius, radius);

  // Load wood texture
  sf::Texture woodTexture;
  if (!woodTexture.loadFromFile("assets/textures/light-wood.jpg")) {
    return -1; // Error handling
  }
  hexagon.setTexture(&woodTexture);

  // Set an aspect ratio for dynamic window sizes
  sf::View view = window.getDefaultView(); // aspect ratio

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
      //
      if (event.type == sf::Event::Resized) {
        float newWidth = event.size.width;
        float newHeight = event.size.height;

        float aspectRatio =
            static_cast<float>(newWidth) / static_cast<float>(newHeight);
        view.setSize(800 * aspectRatio, 800);
        //        view.setCenter(view.getSize().x / 2, view.getSize().y / 2);

        // Centre the grid of hexagons in the middle of the window
        float gridCentreX = (rows - 1) * 1.5f * radius;
        float gridCentreY = (columns - 1) * radius;
        //        float gridCentreY = (columns - 1) * radius / 2.0f;

        view.setCenter(gridCentreX, gridCentreY);

        view.zoom(0.55f);
        window.setView(view);
      }
    }

    window.clear(sf::Color::White);

    // Set background colour
    //    window.clear(sf::Color(100, 150, 200)); // light blue
    window.clear(sf::Color(85, 115, 85)); // Dark sage green
    //    window.clear(sf::Color(120, 120, 120)); // Dark light gray

    // Create 11 hexagons connected to each other in a row
    // Shift hexagons to the right by 1/2 hexagon size further for each colum n
    int gridOffset = 36; // 36
    int shiftGrid = gridOffset;

    for (int y = 0; y < columns; ++y) {
      for (int x = 0; x < rows; ++x) {
        // The horizontal/vertical offsets of 50 and 40
        // might feel off, but that's due to the way SFML
        // calculates the circle outline
        //        hexagon.setPosition((y % 2 ? 1.5 * gridOffset : gridOffset) +
        //        x * gridOffset, 50 + y * (gridOffset - 3));
        hexagon.setPosition(shiftGrid + x * gridOffset,
                            50 + y * (gridOffset - 4));
        hexagon.setFillColor(sf::Color::White);
        window.draw(hexagon);
      }

      shiftGrid += (gridOffset / 2);
    }
    window.display();
  }
}
