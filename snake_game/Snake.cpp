#include <iostream>
#include "Snake.h"

using std::cout;
using std::endl;

Snake::Snake(int x, int y) {
    m_body.emplace_back(std::move(coords{ x, y })); //head
    m_facing = facing::up;
    m_snake_color = { 0, 255, 0 };
    m_apple_color = { 255, 0, 0 };
}

int Snake::get_head_x() { return m_body[0].x; }
int Snake::get_head_y() { return m_body[0].y; }
int Snake::get_size() { return m_body.size(); }
SDL_Color Snake::get_snake_color() { return m_snake_color; }
coords Snake::last() { return m_body[m_body.size() - 1]; }
facing Snake::get_facing() { return m_facing; }
void Snake::turn(facing f) { m_facing = f; }

void Snake::update_pos() {
    for (int i = m_body.size() - 1; i > 0; i--) {
        m_body[i] = m_body[i - 1];
    }

    switch (m_facing)
    {
    case facing::up:
        m_body[0].y -= 11;
        break;
    case facing::down:
        m_body[0].y += 11;
        break;
    case facing::left:
        m_body[0].x -= 11;
        break;
    case facing::right:
        m_body[0].x += 11;
        break;
    default:
        break;
    }

    //przeniesienia przy wyjsciu poza ekran
    for (int i = 0; i < m_body.size(); i++) {
        if (m_body[i].x >= szerokosc - 1) m_body[i].x = 5;
        if (m_body[i].y >= wysokosc - 1)  m_body[i].y = 5;
        if (m_body[i].x <= 0) m_body[i].x = szerokosc - 6;
        if (m_body[i].y <= 0) m_body[i].y = wysokosc - 6;
    }
}

bool Snake::check_collection(coords prev) {
    if (check_colors(m_apple_color, getPixel(get_head_x(), get_head_y()))) {
        cout << "collected!\n";
        m_body.emplace_back(coords{ prev.x, prev.y }); //make snake longer by 1
        return true;
    }
    else {
        return false;
    }
}

void Snake::display(bool collected, coords prev_coords, SDL_Color background_color) {
    //for every snake piece: display
    for (int i = 0; i < m_body.size(); i++) {
        setPixel5x5(m_body[i].x, m_body[i].y, m_snake_color, background_color);
    }

    setPixel(get_head_x(), get_head_y(), { 0, 0, 0 }); //srodkowa kropka

    //if snake dont collect any: erase last pos
    if (!collected)
        setPixel5x5(prev_coords.x, prev_coords.y, background_color);
}

void Snake::check_game_over(bool& done) {
    if (check_colors(m_snake_color, getPixel(get_head_x(), get_head_y()))) {
        cout << "game over!" << endl;
        cout << "score: " << this->get_size() << endl;

        done = true;
        pause(done);
    }
}
