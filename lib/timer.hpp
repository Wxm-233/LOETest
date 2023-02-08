#pragma once

#include <SDL.h>

class Timer {
public:
	Timer();
	void start();
	Uint32 getTicks();
private:
	Uint32 startTicks;
	bool started;
};

Timer::Timer() {
	startTicks = 0;
	started = false;
}

void Timer::start() {
	startTicks = SDL_GetTicks();
	started = true;
}

Uint32 Timer::getTicks() {
	Uint32 ret = 0;
	if (started) {
		ret = SDL_GetTicks() - startTicks;
	}
	return ret;
}