# hexfml

Etymology: Hex + SFML not hex + the acronym fml that has a much different meaning, but definitely reflects the development experience so far.

## Learning outcomes
- C++
- Game development
- Networking (sockets)

## Hex board

![Hex board made in C++ with SFML](docs/hexboard.png)

I am definitely not a front end dev and doing this project only proves to me that if I were to ever want to do front end, I'd need a lot more practice. The hex board code is atrocious and I hope it does not set the precedent for the game and networking code. Even with all this negative talk, I am at least extremely happy with how the board came out. I think in terms of purely aesthetics, it looks pretty great. I do not envy the developer that will have to maintain the code. Oh wait.

## Pieces

![Working piece placement!](docs/pieces.png)

What a euphoric feeling.


## Project structure (WIP)
/hexfml/

│── /src/

│   ├── main.cpp

│   ├── Game.cpp

│   ├── Game.hpp

│   ├── Board.cpp

│   ├── Board.hpp

│   ├── InputHandler.cpp

│   ├── InputHandler.hpp

│── /include/

│── /assets/

│   ├── textures/

│   ├── fonts/

│   ├── sounds/

│── /shaders/          # TBD

│── /config/           # TBD

│   ├── settings.ini

│── /bin/

│── /obj/

│── Makefile

│── README.md


multiplayer online game of Hex. Most likely written in C++ and as barebones as possible (work on anything). Sockets? for game?

sudo apt install libsfml-dev

make

make clean

./bin/hex++

## Credits
https://opengameart.org/node/21051 for wood texture
https://www.istockphoto.com/vector/black-metal-texture-background-vector-illustration-gm928819670-254735270
https://www.istockphoto.com/vector/abstract-gray-and-white-color-gradient-background-vector-illustration-gm1704870086-539106620
