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
#include "Texture.h"
#include "TextureList.h"
#include "Sprite.h"
#include "SpriteList.h"

bool keyUpPressed = false;
bool keyDownPressed = false;
bool keyLeftPressed = false;
bool keyRightPressed = false;

uint8_t rawMapWall[10][10] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

uint8_t rawMapCeiling[10][10] = {
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 6, 6, 6, 6, 3, 3, 3},
	{3, 3, 3, 6, 6, 6, 6, 3, 3, 3},
	{3, 3, 3, 6, 6, 6, 6, 3, 3, 3},
	{3, 3, 3, 6, 6, 6, 6, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};

uint8_t rawMapFloor[10][10] = {
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 7, 7, 7, 7, 7, 7, 3, 3},
	{3, 3, 7, 3, 3, 3, 3, 7, 3, 3},
	{3, 3, 7, 3, 3, 3, 3, 7, 3, 3},
	{3, 3, 7, 3, 3, 3, 3, 7, 3, 3},
	{3, 3, 7, 3, 3, 3, 3, 7, 3, 3},
	{3, 3, 7, 7, 7, 7, 7, 7, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 3, 3, 3, 3, 3}};

// Return the texture index
w3d_Texture *LoadTexture(const char *textureName, const char *filename)
{
	// Load image
	SDL_Surface *sdlTextureSurface = IMG_Load(filename);
	if (sdlTextureSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", filename, IMG_GetError());
		return NULL;
	}
	// Convert surface to screen format
	SDL_Surface *sdlOptimazedTextureSurface = SDL_ConvertSurfaceFormat(sdlTextureSurface, SDL_PIXELFORMAT_ARGB8888, 0);
	// Create texture
	w3d_Texture *texture = w3d_Texture_CretateFromRaw(textureName,
													  sdlOptimazedTextureSurface->w, sdlOptimazedTextureSurface->h,
													  (uint32_t *)sdlOptimazedTextureSurface->pixels);
	// Release sdl textures
	SDL_FreeSurface(sdlOptimazedTextureSurface);
	SDL_FreeSurface(sdlTextureSurface);
	return texture;
}

void DrawTexture(SDL_Renderer *renderer,
				 TTF_Font *font,
				 int32_t positionX,
				 int32_t positionY,
				 const char *text,
				 SDL_Color *color)
{

	SDL_Surface *surface = TTF_RenderText_Solid(font, text, *color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

	int textureWidth = 0;
	int textureHeight = 0;
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	SDL_Rect destinyRect = {positionX, positionY, textureWidth, textureHeight};
	SDL_RenderCopy(renderer, texture, NULL, &destinyRect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

bool Move(SDL_Window *window, w3d_Camera *camera)
{

	SDL_Event event;

	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
		{
			return false;
		}
		if (event.type == SDL_KEYDOWN)
		{
			switch (event.key.keysym.sym)
			{
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
		if (event.type == SDL_KEYUP)
		{
			switch (event.key.keysym.sym)
			{
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
		if (event.type == SDL_MOUSEMOTION)
		{
			w3d_Camera_Rotate(camera, (SCREEN_WIDTH / 2) - event.motion.x);
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		}
	}

	// Run just one time per frame.
	if (keyUpPressed == true)
	{
		w3d_Camera_MoveForward(camera);
	}
	else if (keyDownPressed == true)
	{
		w3d_Camera_MoveBackward(camera);
	}
	if (keyLeftPressed == true)
	{
		w3d_Camera_MoveLeft(camera);
	}
	else if (keyRightPressed == true)
	{
		w3d_Camera_MoveRight(camera);
	}

	return true;
}

int main(void)
{

	// Start SDL library
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	// Start SDL_image library
	if (SDL_Init(IMG_INIT_PNG) != 0)
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return EXIT_FAILURE;
	}

	// Initialize SDL_ttf
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return EXIT_FAILURE;
	}

	// Setup window
	SDL_Window *window = SDL_CreateWindow("Engine3D",
										  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,
										  SDL_WINDOW_SHOWN); // |SDL_WINDOW_FULLSCREEN_DESKTOP); // Fullscreen test
	if (window == NULL)
	{
		printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	SDL_ShowCursor(SDL_DISABLE);
	SDL_WarpMouseInWindow(window, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

	// Setup renderer
	SDL_Renderer *renderer = NULL;
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	// Load textures
	w3d_TextureList *textureList = w3d_TextureList_Create(20);
	w3d_TextureList_Pop(textureList, LoadTexture("RedBrick", "resource/image/original/red-brick.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("Eagle", "resource/image/original/eagle.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("PurpleStone", "resource/image/original/purple-stone.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("GreyStone", "resource/image/original/grey-stone.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("BlueStone", "resource/image/original/blue-stone.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("Mossy", "resource/image/original/mossy.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("Wood", "resource/image/original/wood.png"));
	w3d_TextureList_Pop(textureList, LoadTexture("ColorStone", "resource/image/original/color-stone.png"));

	// Create map
	w3d_Map *mapLevel1 = w3d_Map_CreateFromRaw(10, 10, (uint8_t *)&rawMapFloor, (uint8_t *)&rawMapWall, (uint8_t *)&rawMapCeiling, textureList);

	// Create sprites
	w3d_Sprite *barrelSprite = w3d_Sprite_CreateFromRaw("Barrel", 3, 3, W3D_SPRITE_ANGLE_0, true, LoadTexture("Barrel", "resource/image/original/barrel.png"));
	w3d_Sprite *pillarSprite = w3d_Sprite_CreateFromRaw("Pillar", 3, 7, W3D_SPRITE_ANGLE_0, true, LoadTexture("Pillar", "resource/image/original/pillar.png"));
	w3d_Sprite *greenLightSprite = w3d_Sprite_CreateFromRaw("GreenLight", 5, 5, W3D_SPRITE_ANGLE_0, true, LoadTexture("Barrel", "resource/image/original/green-light.png"));

	w3d_SpriteList *spriteList = w3d_SpriteList_Create(3);
	w3d_SpriteList_Add(spriteList, barrelSprite);
	w3d_SpriteList_Add(spriteList, pillarSprite);
	w3d_SpriteList_Add(spriteList, greenLightSprite);

	// Create a texture
	SDL_Texture *cameraTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Create a new camera
	w3d_Camera *camera = w3d_Camera_Create(SCREEN_WIDTH, SCREEN_HEIGHT);
	w3d_Camera_SetMap(camera, mapLevel1);
	w3d_Camera_SetSpriteList(camera, spriteList);

	TTF_Font *font = TTF_OpenFont("resource/font/FreeSansBold.ttf", 32);

	float time;
	float lastTime = SDL_GetTicks();
	uint32_t frameRate = 0;
	uint32_t frameCount = 0;

	char frameRateText[20];
	SDL_Color color = {255, 255, 255, 255};

	void *mPixels = NULL;
	int mPitch;

	while (Move(window, camera))
	{

		SDL_RenderClear(renderer);

		SDL_LockTexture(cameraTexture, NULL, &mPixels, &mPitch);
		w3d_Camera_Render(camera, mPixels);
		SDL_UnlockTexture(cameraTexture);
		SDL_RenderCopy(renderer, cameraTexture, NULL, NULL);

		snprintf(frameRateText, sizeof(frameRateText), "%d", frameRate);
		DrawTexture(renderer, font, 10, 10, frameRateText, &color);

		SDL_RenderPresent(renderer);

		// Calculate the frame rate
		frameCount++;
		time = SDL_GetTicks();
		if (lastTime < time - 1000)
		{
			lastTime = time;
			frameRate = frameCount;
			frameCount = 0;
		}
	}

	w3d_Camera_Destroy(camera);
	SDL_DestroyTexture(cameraTexture);
	w3d_Map_Close(mapLevel1);

	TTF_CloseFont(font);
	SDL_DestroyTexture(cameraTexture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
