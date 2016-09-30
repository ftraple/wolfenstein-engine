/*
 ============================================================================
 Name        : MyDukeNukem.c
 Author      : Fabiano Traple
 Version     :
 Copyright   : Blah!
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Defs.h"
#include "Camera.h"


bool Move(float frameTime) {

	SDL_Event event;

	while( SDL_PollEvent(&event) != 0 ) {
		if( event.type == SDL_QUIT ) {
			return FALSE;
		}
		else if( event.type == SDL_KEYDOWN ) {
			MoveCamera(&event, frameTime);
		}
	}
	return TRUE;
}

int main(void) {

	// Start SDL library
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Start SDL_image library
	if (SDL_Init(IMG_INIT_PNG) != 0) {
		printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return EXIT_FAILURE;
	}

	 //Initialize SDL_ttf
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}

	// Setup window
	SDL_Window*window = SDL_CreateWindow("Engine3D",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Setup renderer
	SDL_Renderer* renderer = NULL;
	renderer =  SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

	// Create a new camera
	CreateCamera(640, 480);

	Uint32 time = SDL_GetTicks();
	Uint32 oldTime = time;
	Uint32 delay = 0;
	float frameTime = 0;

	while(Move(frameTime)) {
		RenderCamera(renderer);
		SDL_RenderPresent(renderer);
		oldTime = time;
		time = SDL_GetTicks();
		SDL_Delay(delay);
		if((time-oldTime) < 20) {
			delay = 20-(time-oldTime);
		}
		else {
			delay = 0;
		}
		frameTime = (time-oldTime) / 1000.0;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


