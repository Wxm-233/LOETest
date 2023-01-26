#include "lib/LOETest.hpp"

GameLOE::GameLOE() : 
	gLog(nullptr), 
	gState(START), 
	gMainWnd(nullptr), 
	gMainRenderer(nullptr), 
	gBGSurface(nullptr), 
	gBGTexture(nullptr) {
	fopen_s(&gLog,"GameLOE.log", "w");
	if (gLog == nullptr) {
		exit(-1);
	}
}

GameLOE::~GameLOE() {
	if (gLog) {
		fclose(gLog);
	}

	if (gBGSurface) {
		SDL_FreeSurface(gBGSurface);
	}

	if (gBGTexture) {
		SDL_DestroyTexture(gBGTexture);
	}

	if (gMainRenderer) {
		SDL_DestroyRenderer(gMainRenderer);
	}

	if (gMainWnd) {
		SDL_DestroyWindow(gMainWnd);
	}

	SDL_Quit();
}

void GameLOE::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(gLog, "Failed to initialize SDL2!\nError: %s\n", SDL_GetError());
		exit(-2);
	}

	gMainWnd = SDL_CreateWindow(
		"LOETest",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		MainWndW, MainWndH,
		SDL_WINDOW_SHOWN
	);

	if (!gMainWnd) {
		fprintf(gLog, "Cannot create window!\nError: %s\n", SDL_GetError());
		exit(-3);
	}

	gMainRenderer = SDL_CreateRenderer(
		gMainWnd,
		-1,
		SDL_RENDERER_ACCELERATED
	);

	if (!gMainRenderer) {
		fprintf(gLog, "Cannot create renderer!\nError: %s\n", SDL_GetError());
		exit(-4);
	}
}

void GameLOE::mainLoop() {
	SDL_Event event;
	bool doQuit = false;
	while (!doQuit) {
		render();

		if (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				doQuit = true;
				break;
			case SDL_KEYDOWN:
				break;
			}
		}
	}
}

void GameLOE::render() {
	SDL_SetRenderDrawColor(
		gMainRenderer,
		0xFF, 0xFF, 0xFF, 0xFF
	);
	SDL_RenderClear(gMainRenderer);

	SDL_RenderPresent(gMainRenderer);
}

bool do_Quit = false;