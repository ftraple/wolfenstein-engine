/*
 * Utils.c
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#include "Utils.h"

SDL_Surface* LoadSurface(const char*imageFile) {
	//Load image at specified path
	SDL_Surface* surface = IMG_Load(imageFile);
	if (surface == NULL) {
		printf( "Unable to load image %s! SDL_image Error: %s\n", imageFile, IMG_GetError() );
		return NULL;
	}
	//Convert surface to screen format
	SDL_Surface*optimizedSurface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
	if (optimizedSurface == NULL) {
		printf( "Unable to optimize image %s! SDL Error: %s\n", imageFile, SDL_GetError() );
		return NULL;
	}
	//Delete loaded surface
	SDL_FreeSurface(surface);
	return optimizedSurface;
}


void DrawSurface(SDL_Renderer*renderer, SDL_Surface*surface) {
	Uint8*pixel = (Uint8*)surface->pixels;
	int pos = 0;
	for (int y = 0; y < surface->h; ++y) {
		for (int x = 0; x < surface->w; ++x) {
			SDL_SetRenderDrawColor(renderer, pixel[pos+3], pixel[pos+2], pixel[pos+1], 255);
			SDL_RenderDrawPoint(renderer, x, y);
			pos = pos+4;
		}
	}
}
