/*
 * Utils.h
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>


SDL_Surface* LoadSurface(const char*imageFile);

void DrawSurface(SDL_Renderer*renderer, SDL_Surface*surface);

#endif /* UTILS_H_ */
