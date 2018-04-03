#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Defs.h"
#include "Map.h"
#include "Camera.h"
#include "Scene.h"
#include "Texture.h"


bool keyUpPressed = false;
bool keyDownPressed = false;
bool keyLeftPressed = false;
bool keyRightPressed = false;


uint8_t rawMap[10][10] = {
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1},
};

void DrawTexture(SDL_Renderer*renderer,
			     TTF_Font*font,
				 int32_t positionX,
				 int32_t positionY,
				 const char*text,
				 SDL_Color*color) {

	SDL_Surface*surface = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture*texture = SDL_CreateTextureFromSurface(renderer,surface);

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	SDL_Rect destinyRect = {positionX, positionY, textureWidth, textureHeight};
	SDL_RenderCopy(renderer, texture, NULL, &destinyRect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

bool Move(SDL_Window*window, Camera*camera) {

	SDL_Event event;

	while( SDL_PollEvent(&event) != 0 ) {
		if( event.type == SDL_QUIT ) {
			return false;
		}
		if( event.type == SDL_KEYDOWN ) {
			switch( event.key.keysym.sym ) {
				case SDLK_ESCAPE:
					return false;
				case SDLK_UP:
					keyUpPressed = true;
					break;
				case SDLK_DOWN:
					keyDownPressed = true;
					break;
				case SDLK_LEFT:
					keyLeftPressed = true;
					break;
				case SDLK_RIGHT:
					keyRightPressed = true;
					break;
				default:
					break;
			}
		}
		if( event.type == SDL_KEYUP ) {
			switch( event.key.keysym.sym ) {
				case SDLK_UP:
					keyUpPressed = false;
					break;
				case SDLK_DOWN:
					keyDownPressed = false;
					break;
				case SDLK_LEFT:
					keyLeftPressed = false;
					break;
				case SDLK_RIGHT:
					keyRightPressed = false;
					break;
				default:
					break;
			}
		}

		// Run multiple times per frame.
		if (event.type == SDL_MOUSEMOTION) {
			RotateCamera(camera, (SCREEN_WIDTH/2)-event.motion.x);
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
		}
	}

	// Run just one time per frame.
	if (keyUpPressed == true) {
		MoveCameraForward(camera);
	}
	else if (keyDownPressed == true) {
		MoveCameraBackward(camera);
	}
	if (keyLeftPressed == true) {
		MoveCameraLeft(camera);
	}
	else if (keyRightPressed == true) {
		MoveCameraRight(camera);
	}

	return true;
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
			SDL_WINDOW_SHOWN); // |SDL_WINDOW_FULLSCREEN_DESKTOP); // Fullscreen test
	if (window == NULL) {
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

    SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouseInWindow(window, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);

	// Setup renderer
	SDL_Renderer*renderer = NULL;
	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_SOFTWARE);

	// Open map
	w3d_Map* mapLevel1 = w3d_CreateMapFromRaw(10, 10, (uint8_t*)&rawMap);

    // Load textures
	OpenTexture("resource/image/redbrick.png");
	OpenTexture("resource/image/eagle.png");
	OpenTexture("resource/image/purplestone.png");
	OpenTexture("resource/image/greystone.png");
	OpenTexture("resource/image/bluestone.png");
	OpenTexture("resource/image/mossy.png");
	OpenTexture("resource/image/wood.png");
	OpenTexture("resource/image/colorstone.png");

	OpenTexture("resource/image/barrel.png");
	OpenTexture("resource/image/pillar.png");
	OpenTexture("resource/image/greenlight.png");

	// Create a texture
	SDL_Texture*cameraTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Create a new camera
	Camera*camera1 = CreateCamera(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Create a scene
	Scene*scene1 = Scene_Create(mapLevel1);
	Scene_AddCamera(scene1, camera1);

	TTF_Font*font = TTF_OpenFont("resource/font/FreeSansBold.ttf", 32);


	Scene_Render(scene1);

	float time;
	float lastTime = SDL_GetTicks();
	uint32_t frameRate = 0;
	uint32_t frameCount = 0;

//	SDL_Delay(300);
//	SDL_UpdateTexture(cameraTexture, NULL, GetCameraBuffer(camera1), SCREEN_WIDTH*sizeof(Uint32));
//	SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);
//	SDL_RenderPresent(renderer);
//	SDL_Delay(5000);

	char frameRateText[20];
	SDL_Color color = {255, 255, 255, 255};

	while(Move(window, camera1)) {

		SDL_RenderClear(renderer);

		Scene_Render(scene1);
		SDL_UpdateTexture(cameraTexture, NULL, GetCameraBuffer(camera1), SCREEN_WIDTH*sizeof(Uint32));
		SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);

		snprintf(frameRateText, sizeof(frameRateText), "%d", frameRate);
		DrawTexture(renderer, font, 10, 10,frameRateText, &color);

		SDL_RenderPresent(renderer);

		// Calculate the frame rate
		frameCount++;
		time = SDL_GetTicks();
		if (lastTime < time-1000) {
			lastTime = time;
			frameRate = frameCount;
			frameCount = 0;
		}
	}

	DestroyCamera(camera1);
	SDL_DestroyTexture(cameraTexture);
	w3d_CloseMap(mapLevel1);

	TTF_CloseFont(font);
	SDL_DestroyTexture(cameraTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}


