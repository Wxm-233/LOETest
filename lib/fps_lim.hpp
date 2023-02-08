#pragma once

#include <SDL.h>
#include "timer.hpp"

const int maxFPS = 60;

class FPSCapper {
public:
	int getTargetFPS() const;
	void setTargetFPS(const int&);
	void start();
	void cap();
private:
	int targetFPS;
	int targetTicks;
	Timer t;
};

int FPSCapper::getTargetFPS() const {
	return targetFPS;
}

void FPSCapper::setTargetFPS(const int& FPS) {
	targetFPS = FPS;
	targetTicks = 1000 / FPS;
}

void FPSCapper::start() {
	t.start();
}

void FPSCapper::cap() {
	int frameTicks = t.getTicks();
	if (frameTicks < targetTicks) {
		SDL_Delay(targetTicks - frameTicks);
	}
}