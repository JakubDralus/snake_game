#pragma once
#include <vector>
#include <SDL.h>
#include "coords.h"
#include "util.h"

enum class facing {
    up = 0,
    down,
    left,
    right
};

class Snake {
private:
    coords m_coords;
    facing m_facing;
    std::vector<coords> m_body;
    SDL_Color m_snake_color;
    SDL_Color m_apple_color;

    int szerokosc = 231; //TODO: do klasy game
    int wysokosc = 231;

public:
    Snake(int x, int y);

    int get_head_x();
    int get_head_y();
    int get_size();
    SDL_Color get_snake_color();
    coords last();
    facing get_facing();

    void turn(facing f);
    void update_pos();
    bool check_collection(coords prev);
    void display(bool collected, coords prev_coords, SDL_Color background_color);
    void check_game_over(bool& done);
};