#pragma once
#include <iostream>
#include "util.h"

using std::cout;
using std::endl;

SDL_Window* window;
SDL_Surface* screen;
SDL_Color background_color = { 170, 255, 120 };


void init_game(int szerokosc, int wysokosc) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("snake game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        szerokosc * 2, wysokosc * 2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    screen = SDL_GetWindowSurface(window);

    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        exit(false);
    }

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, background_color.r, background_color.g, background_color.b));
    SDL_UpdateWindowSurface(window);
}


void pause(bool& done) {
    std::cout << "pause\n";
    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
                return;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                done = true;
                return;
            }
        }
    }
}

int roundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

bool check_colors(SDL_Color c1, SDL_Color c2) {
    if (c1.r != c2.r) return false;
    if (c1.g != c2.g) return false;
    if (c1.b != c2.b) return false;
    return true;
}

void setPixel5x5(int x, int y, SDL_Color& color, SDL_Color background_color) {
    for (int i = -5; i <= 5; i++)
        for (int j = -5; j <= 5; j++)
            setPixel(x + i, y + j, color);

    //zaokraglanie rogow
    setPixel(x + 5, y - 5, background_color);
    setPixel(x + 5, y + 5, background_color);
    setPixel(x - 5, y + 5, background_color);
    setPixel(x - 5, y - 5, background_color);
}

void setPixel5x5(int x, int y, SDL_Color& color) {
    for (int i = -5; i <= 5; i++)
        for (int j = -5; j <= 5; j++)
            setPixel(x + i, y + j, color);
}

void setPixel(int x, int y, SDL_Color color) {
    setPixel(x, y, color.r, color.g, color.b);
}

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B)
{
    if ((x >= 0) && (y >= 0))
    {
        /* Zamieniamy poszczególne sk³adowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informacjê ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8* p1 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2) * bpp;
        Uint8* p2 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2) * bpp;
        Uint8* p3 = (Uint8*)screen->pixels + (y * 2) * screen->pitch + (x * 2 + 1) * bpp;
        Uint8* p4 = (Uint8*)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2 + 1) * bpp;

        /* Ustawiamy wartoœæ piksela, w zale¿no?ci od formatu powierzchni*/
        switch (bpp)
        {
        case 1: //8-bit
            *p1 = pixel;
            *p2 = pixel;
            *p3 = pixel;
            *p4 = pixel;
            break;

        case 2: //16-bit
            *(Uint16*)p1 = pixel;
            *(Uint16*)p2 = pixel;
            *(Uint16*)p3 = pixel;
            *(Uint16*)p4 = pixel;
            break;

        case 3: //24-bit
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p1[0] = (pixel >> 16) & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = pixel & 0xff;
                p2[0] = (pixel >> 16) & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = pixel & 0xff;
                p3[0] = (pixel >> 16) & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = pixel & 0xff;
                p4[0] = (pixel >> 16) & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = pixel & 0xff;
            }
            else {
                p1[0] = pixel & 0xff;
                p1[1] = (pixel >> 8) & 0xff;
                p1[2] = (pixel >> 16) & 0xff;
                p2[0] = pixel & 0xff;
                p2[1] = (pixel >> 8) & 0xff;
                p2[2] = (pixel >> 16) & 0xff;
                p3[0] = pixel & 0xff;
                p3[1] = (pixel >> 8) & 0xff;
                p3[2] = (pixel >> 16) & 0xff;
                p4[0] = pixel & 0xff;
                p4[1] = (pixel >> 8) & 0xff;
                p4[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4: //32-bit
            *(Uint32*)p1 = pixel;
            *(Uint32*)p2 = pixel;
            *(Uint32*)p3 = pixel;
            *(Uint32*)p4 = pixel;
            break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color = background_color;
    Uint32 col = 0;
    if ((x >= 0) && (y >= 0)) {
        //okreœlamy pozycjê
        char* pPosition = (char*)screen->pixels;

        //przesuniêcie wzglêdem y
        pPosition += (screen->pitch * y * 2);

        //przesuniêcie wzglêdem x
        pPosition += (screen->format->BytesPerPixel * x * 2);

        //kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        //konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return (color);
}
