#include "lib/LOETest.hpp"

int Main(int argc, char* argv[]) {
	GameLOE game;
	game.init();
	game.mainLoop();
	return 0;
}