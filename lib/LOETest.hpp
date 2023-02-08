#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "fps_lim.hpp"

enum GameState {
	START, END, PLAYING
};

enum Direction {
	UP = 1, 
	DOWN = (1 << 1), 
	LEFT = (1 << 2), 
	RIGHT = (1 << 3)
};

class GameLOE;
class PlayerRect;

class PlayerRect {
public:
	void move(Direction direction);
	void stop(Direction direction);
	void run();
	void setPos(int x, int y);
	void getPos(int& x, int& y);
	void setFather(GameLOE* father);
	SDL_Rect* getRect();
private:
	int xspeed = 0;
	int yspeed = 0;
	SDL_Rect Rect{
		0, 0, 240, 25
	};
	GameLOE* father = nullptr;
};

class GameLOE {
public:
	GameLOE();
	~GameLOE();
	void init();
	void mainLoop();
	void render();
	void loadIMG(const char* file);
	void loadTexture(SDL_Texture* &Texture);
	void handleEvent();
	void getWndWH(int& w, int& h);
private:
	SDL_Window* gMainWnd;
	SDL_Renderer* gMainRenderer;
	SDL_Surface* gBGSurface;
	SDL_Texture* gBGTexture1;
	SDL_Texture* gBGTexture2;
	SDL_Texture* gRectPic;

	FILE* gLog;
	GameState gState;
	bool doQuit = false;
	SDL_Event event{};
	PlayerRect rect{};

	int gMWndH;
	int gMWndW;
};

GameLOE::GameLOE() :
	gLog(nullptr),
	gState(START),
	gMainWnd(nullptr),
	gMainRenderer(nullptr),
	gBGSurface(nullptr),
	gBGTexture1(nullptr),
	gBGTexture2(nullptr)
{
	fopen_s(&gLog, "GameLOE.log", "w");

	if (gLog == nullptr) {
		exit(-1);
	}
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	this->rect.setFather(this);
}

GameLOE::~GameLOE() {
	if (gLog) {
		fclose(gLog);
	}

	if (gBGSurface) {
		SDL_FreeSurface(gBGSurface);
	}

	if (gBGTexture1) {
		SDL_DestroyTexture(gBGTexture1);
	}

	if (gBGTexture2) {
		SDL_DestroyTexture(gBGTexture2);
	}

	if (gMainRenderer) {
		SDL_DestroyRenderer(gMainRenderer);
	}

	if (gMainWnd) {
		SDL_DestroyWindow(gMainWnd);
	}

	IMG_Quit();

	SDL_Quit();
}

void GameLOE::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(gLog, "Failed to initialize SDL2!\nError: %s\n", SDL_GetError());
		exit(-2);
	}

	this->loadIMG("assests/Pictures/ena.jpg");

	gMWndH = gBGSurface->h;
	gMWndW = gBGSurface->w;

	rect.setPos(gMWndW, gMWndH);

	gMainWnd = SDL_CreateWindow(
		"LOETest",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		gMWndW, gMWndH,
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

	this->loadTexture(gBGTexture1);
	SDL_FreeSurface(gBGSurface);
	this->loadIMG("assests/Pictures/rin.png");
	this->loadTexture(gBGTexture2);
	SDL_FreeSurface(gBGSurface);
	this->loadIMG("assests/Pictures/rect.png");
	this->loadTexture(gRectPic);
	SDL_FreeSurface(gBGSurface);
}

void GameLOE::mainLoop() {

	FPSCapper capper;
	capper.setTargetFPS(maxFPS);

	while (!doQuit) {
		capper.start();
		rect.run();
		render();
		handleEvent();
		capper.cap();
	}
}

void GameLOE::render() {
	SDL_SetRenderDrawColor(
		gMainRenderer,
		0xFF, 0xFF, 0xFF, 0xFF
	);
	SDL_RenderClear(gMainRenderer);

	switch (gState) {
	case START:
		SDL_RenderCopy(gMainRenderer, gBGTexture1, nullptr, nullptr);
		break;
	case END:
		SDL_RenderCopy(gMainRenderer, gBGTexture2, nullptr, nullptr);
		break;
	}

	SDL_RenderCopy(gMainRenderer, gRectPic, nullptr, this->rect.getRect());

	SDL_RenderPresent(gMainRenderer);
}

void GameLOE::loadIMG(const char* file) {
	gBGSurface = IMG_Load(file);
	if (!gBGSurface) {
		fprintf(gLog, "Cannot open %s!\n", file);
		exit(-5);
	}
}

void GameLOE::loadTexture(SDL_Texture*& Texture) {
	Texture = SDL_CreateTextureFromSurface(gMainRenderer, gBGSurface);
	if (!Texture) {
		fprintf(gLog, "Cannot create texture!\n");
		exit(-6);
	}
	SDL_FreeSurface(gBGSurface);
	gBGSurface = nullptr;
}

void GameLOE::handleEvent() {
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			doQuit = true;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				if (gState != END) {
					gState = END;
				}
				else {
					gState = START;
				}
				break;
			case SDLK_UP:
			case SDLK_w:
				rect.move(UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				rect.move(DOWN);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				rect.move(LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				rect.move(RIGHT);
				break;
			}
			break;

		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
			case SDLK_w:
				rect.stop(UP);
				break;
			case SDLK_DOWN:
			case SDLK_s:
				rect.stop(DOWN);
				break;
			case SDLK_LEFT:
			case SDLK_a:
				rect.stop(LEFT);
				break;
			case SDLK_RIGHT:
			case SDLK_d:
				rect.stop(RIGHT);
				break;
			}
			break;
		}
	}
}

void PlayerRect::move(Direction direction) {
	switch (direction) {
	case UP:
		yspeed = -10;
		break;
	case DOWN:
		yspeed = 10;
		break;
	case LEFT:
		xspeed = -10;
		break;
	case RIGHT:
		xspeed = 10;
		break;
	}
}

void PlayerRect::stop(Direction direction) {
	switch (direction) {
	case UP:
	case DOWN:
		yspeed = 0;
		break;
	case LEFT:
	case RIGHT:
		xspeed = 0;
		break;
	}
}

void PlayerRect::run() {
	int w = 1, h = 1;
	this->father->getWndWH(w, h);
	this->Rect.x = (Rect.x + xspeed + w) % w;
	this->Rect.y = (Rect.y + yspeed + h) % h;
}

void PlayerRect::setPos(int x, int y) {
	this->Rect.x = x / 2.0 - this->Rect.w;
	this->Rect.y = y * 4.0 / 5 - this->Rect.h;
}

void PlayerRect::getPos(int& x, int& y) {
	x = this->Rect.x;
	y = this->Rect.y;
}

SDL_Rect* PlayerRect::getRect() {
	return &(this->Rect);
}

void GameLOE::getWndWH(int& w, int& h) {
	w = this->gMWndW;
	h = this->gMWndH;
}

void PlayerRect::setFather(GameLOE* father) {
	this->father = father;
}