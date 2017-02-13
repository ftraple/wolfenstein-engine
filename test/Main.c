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
#include "Map.h"
#include "Camera.h"

int32_t glMouseX = 0;
int32_t glMouseY = 0;

bool keyUpPressed = FALSE;
bool keyDownPressed = FALSE;
bool keyLeftPressed = FALSE;
bool keyRightPressed = FALSE;

bool Move() {

	SDL_Event event;

	while( SDL_PollEvent(&event) != 0 ) {
		if( event.type == SDL_QUIT ) {
			return FALSE;
		}
		if( event.type == SDL_KEYDOWN ) {
			switch( event.key.keysym.sym ) {
				case SDLK_ESCAPE:
					return FALSE;
				case SDLK_UP:
					keyUpPressed = TRUE;
					break;
				case SDLK_DOWN:
					keyDownPressed = TRUE;
					break;
				case SDLK_LEFT:
					keyLeftPressed = TRUE;
					break;
				case SDLK_RIGHT:
					keyRightPressed = TRUE;
					break;
				default:
					break;
			}
		}
		if( event.type == SDL_KEYUP ) {
			switch( event.key.keysym.sym ) {
				case SDLK_UP:
					keyUpPressed = FALSE;
					break;
				case SDLK_DOWN:
					keyDownPressed = FALSE;
					break;
				case SDLK_LEFT:
					keyLeftPressed = FALSE;
					break;
				case SDLK_RIGHT:
					keyRightPressed = FALSE;
					break;
				default:
					break;
			}
		}


		if (keyUpPressed == TRUE) {
			MoveCameraForward();
		}
		else if (keyDownPressed == TRUE) {
			MoveCameraBackward();
		}
		if (keyLeftPressed == TRUE) {
			MoveCameraLeft();
		}
		else if (keyRightPressed == TRUE) {
			MoveCameraRight();
		}


		if (event.type == SDL_MOUSEMOTION) {
			if( glMouseX > event.motion.x ) {
				RotateCameraLeft();
			}
			if( glMouseX < event.motion.x ) {
				RotateCameraRight();
			}
			glMouseX = event.motion.x;
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
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);

	// Open map
	MapContext*level1 = OpenMap("resource/map/level_01.map");

	// Create a new camera
	CreateCamera(SCREEN_WIDTH, SCREEN_HEIGHT, level1);

	// Create a texture
	SDL_Texture*cameraTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);

	RenderCamera();

	Uint32 time = SDL_GetTicks();
	Uint32 oldTime = time;
	Uint32 delay = 0;
	float frameTime = 0;

	SDL_Delay(300);
	SDL_UpdateTexture(cameraTexture, NULL, GetCameraBuffer(), SCREEN_WIDTH*sizeof(Uint32));
	SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(5000);

//	while(Move()) {
//
//		RenderCamera();
//		SDL_UpdateTexture(cameraTexture, NULL, GetCameraBuffer(), SCREEN_WIDTH*sizeof(Uint32));
//		SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);
//
//		SDL_RenderPresent(renderer);
//		oldTime = time;
//		time = SDL_GetTicks();
//		SDL_Delay(delay);
//		if((time-oldTime) < 20) {
//			delay = 20-(time-oldTime);
//		}
//		else {
//			delay = 0;
//		}
//		printf("Delay = %d\n", delay);
//		frameTime = (time-oldTime) / 1000.0;
//	}

	DestroyCamera();
	CloseMap(level1);

	SDL_DestroyTexture(cameraTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


