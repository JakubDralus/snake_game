#pragma once

void init_game(int szerokosc, int wysokosc);

void pause(bool& done);

int roundUp(int numToRound, int multiple);

bool check_colors(SDL_Color c1, SDL_Color c2);

void setPixel5x5(int x, int y, SDL_Color& color, SDL_Color background_color);
void setPixel5x5(int x, int y, SDL_Color& color);
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
void setPixel(int x, int y, SDL_Color color);

SDL_Color getPixel(int x, int y);