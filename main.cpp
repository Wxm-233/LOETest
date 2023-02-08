#include "lib/LOETest.hpp"

int main(int argc, char* argv[]) {
	GameLOE game;
	game.init();
	game.mainLoop();
	return 0;
}