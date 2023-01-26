#ifndef __LOETEST_H__
#define __LOETEST_H__

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#ifdef __WIN32
#define Main SDL_main
#endif

#ifndef __WIN32
#define Main main
#endif

enum GameState {
	START, END, PLAYING
};

class GameLOE {
public:
	GameLOE();
	~GameLOE();
	void init();
	void mainLoop();
	void render();
private:
	SDL_Window* gMainWnd;
	SDL_Renderer* gMainRenderer;
	SDL_Surface* gBGSurface;
	SDL_Texture* gBGTexture;

	const int MainWndH = 700;
	const int MainWndW = 700;

	FILE* gLog;
	GameState gState;
};

#endif