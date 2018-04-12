#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


SDL_Surface* LoadSurface(const char*imageFile);

void DrawSurface(SDL_Renderer*renderer, SDL_Surface*surface);

#endif /* UTILS_H */
