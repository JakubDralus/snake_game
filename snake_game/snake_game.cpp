#include <exception>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <SDL.h>
#include "util.cpp"
#include "Apple.h"
#include "Snake.h"

using std::cout;
using std::endl;

//makes a new apple and checks it to not make it on top of the snake
Apple* new_apple(int wysokosc, int szerokosc) {
    int x = roundUp(rand() % szerokosc - 12, 11) + 5;
    int y = roundUp(rand() % szerokosc - 12, 11) + 5;
    SDL_Color color = getPixel(x, y);
    SDL_Color snake_color = { 0, 255, 0 }; //do naprawy (nieladne)

    if (!check_colors(color, snake_color)) //if not on snake: make one
        return new Apple(x, y);
    else 
        new_apple(wysokosc, szerokosc); //another try
}

int main(int argc, char* argv[]) {
    srand(time(NULL));

    int szerokosc = 231;
    int wysokosc  = 231; //21x21
    init_game(szerokosc, wysokosc); //TODO: zrobic klase game 

    SDL_Event event;
    int i = 0;
    bool collected = false;
    bool paused = false;
    bool done = false;
    Snake* snake = new Snake(szerokosc / 2, wysokosc / 2); //create a snake
    Apple* apple = new_apple(wysokosc, szerokosc); //pierwsze jabłko

    // główna pętla programu
    while (!done) {
        cout << i++ << ",  " << snake->get_head_x()/11 << " " << snake->get_head_y()/11 << "\n";
        cout << "length: " << snake->get_size() << endl;

        SDL_UpdateWindowSurface(window); //window update a potem troche czasu na reakje
        Sleep(100); //ms

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                done = true;
                break;

            case SDL_KEYDOWN:
                cout << "key pressed!: " << event.key.keysym.sym << endl;
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    done = true;

                //TODO: zeby nie dalo sie zawracac przez przypadek
                if (event.key.keysym.sym == SDLK_UP && snake->get_facing() != facing::down)
                    snake->turn(facing::up);

                if (event.key.keysym.sym == SDLK_DOWN && snake->get_facing() != facing::up)
                    snake->turn(facing::down);

                if (event.key.keysym.sym == SDLK_LEFT && snake->get_facing() != facing::right)
                    snake->turn(facing::left);

                if (event.key.keysym.sym == SDLK_RIGHT && snake->get_facing() != facing::left)
                    snake->turn(facing::right);

                if (event.key.keysym.sym == SDLK_SPACE)
                    pause(done);

            default:
                break;
            }
        }
        coords prev_c = snake->last(); //for erasing last position
        snake->update_pos();
        snake->check_game_over(done);
        collected = snake->check_collection(prev_c);
        snake->display(collected, prev_c, background_color);

        //apple spawn
        if (collected) {
            delete apple;
            apple = new_apple(wysokosc, szerokosc);
        }
    }

    delete apple;
    delete snake;
    if (screen) {
        SDL_FreeSurface(screen);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return 0;
}
