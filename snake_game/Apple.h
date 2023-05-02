#pragma once
#include <SDL.h>
#include "coords.h"

class Apple {
private:
	coords m_coords;
	SDL_Color m_apple_color = { 255, 0, 0 };
	SDL_Color background_color = { 170, 255, 120 };

public:
	Apple(int x, int y);

private:
	void display();
};