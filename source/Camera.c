/*
 * Camera.c
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL_image.h>

#include "Defs.h"
#include "Utils.h"
#include "Camera.h"

int worldMap[MAP_WIDTH][MAP_HEIGHT]=
{
	{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
	{4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	{4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	{4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
	{4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
	{4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
	{4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
	{4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	{4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
	{4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
	{4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
	{4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
	{6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	{8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
	{6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
	{4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	{4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
	{4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
	{4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
	{4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
	{4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};


typedef struct {
	float positionX;
	float positionY;
	float directionX;
	float directionY;
	float planeX;
	float planeY;

	int width;
	int height;
} CameraContext;

CameraContext*camera = NULL;

SDL_Surface *texture[8];

void CreateCamera(unsigned int width, unsigned int height) {

	// Allocate camera context
	camera = (CameraContext*)malloc(sizeof(CameraContext));
	if (camera == NULL) {
		printf("Allocate camera memory fail!\n");
		return;
	}

	// Set default values to the camera context
	camera->positionX = 22;
	camera->positionY = 11.5;
	camera->directionX = -1;
	camera->directionY = 0;
	camera->planeX = 0;
	camera->planeY = 0.66;
	camera->width = width;
	camera->height = height;

    // Load textures
	texture[0] = LoadSurface("resource/image/eagle.png");
	texture[1] = LoadSurface("resource/image/redbrick.png");
	texture[2] = LoadSurface("resource/image/purplestone.png");
	texture[3] = LoadSurface("resource/image/greystone.png");
	texture[4] = LoadSurface("resource/image/bluestone.png");
	texture[5] = LoadSurface("resource/image/mossy.png");
	texture[6] = LoadSurface("resource/image/wood.png");
	texture[7] = LoadSurface("resource/image/colorstone.png");

}

void RenderCamera(SDL_Renderer*renderer) {

	// Clean renderer
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_Rect rect = {0, 0, camera->width, camera->height/2};
	SDL_RenderFillRect(renderer, &rect);

	float cameraX = 0;
	float rayPosX = camera->positionX;
	float rayPosY = camera->positionY;
	float rayDirX = 0;
	float rayDirY = 0;

	//length of ray from current position to next x or y-side
	float sideDistX;
	float sideDistY;

	float perpWallDist;

	// Draw
	for(int x = 0; x < camera->width; x++) {

		// Calculate ray position and direction
		cameraX = (2*x/(float)camera->width)-1;
		rayDirX = camera->directionX + camera->planeX * cameraX;
		rayDirY = camera->directionY + camera->planeY * cameraX;

		//which box of the map we're in
		int mapX = (int)rayPosX;
		int mapY = (int)rayPosY;

		//length of ray from one x or y-side to next x or y-side
		float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;

		//calculate step and initial sideDist
		if (rayDirX < 0)
		{
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}
		if (rayDirY < 0)
		{
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}

		//perform DDA
		bool hitWall = FALSE;
		int side; //was a NS or a EW wall hit?

		while (hitWall == FALSE)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (worldMap[mapX][mapY] > 0) {
				hitWall = TRUE;
			}
		}

		//Calculate distance projected on camera direction (oblique distance will give fish eye effect!)
		if (side == 0) {
			perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
		}
		else {
			perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
		}

		//Calculate height of line to draw on screen
		int lineHeight = (int)(camera->height / perpWallDist);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + camera->height / 2;
		if(drawStart < 0)drawStart = 0;
		int drawEnd = lineHeight / 2 + camera->height / 2;
		if(drawEnd > camera->height)drawEnd = camera->height;


		//choose wall color
		//give x and y sides different brightness
		Uint8 color = 254;
		if (side == 1) {
			color = color / 2;
		}
		switch(worldMap[mapX][mapY])
		{
			case 1: {
				SDL_SetRenderDrawColor( renderer, color, 0, 0, 255 );
				break;
			}
			case 2: {
				SDL_SetRenderDrawColor( renderer, 0, color, 0, 255 );
				break;
			}
			case 3: {
				SDL_SetRenderDrawColor( renderer, 0, 0, color, 255 );
				break;
			}
			case 4:  {
				SDL_SetRenderDrawColor( renderer, color, color, color, 255 );
				break;
			}
			default: {
				SDL_SetRenderDrawColor( renderer, color, 0, color, 255 );
				break;
			}
		}

		//texturing calculations
		int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!

		//calculate value of wallX
		float wallX; //where exactly the wall was hit
		if (side == 0) {
			wallX = rayPosY + perpWallDist * rayDirY;
		}
		else {
			wallX = rayPosX + perpWallDist * rayDirX;
		}
		wallX -= floor((wallX));

		//x coordinate on the texture
		int texX = (int)(wallX*(float)TEXTURE_WIDTH);
		if(side == 0 && rayDirX > 0) texX = TEXTURE_WIDTH - texX - 1;
		if(side == 1 && rayDirY < 0) texX = TEXTURE_WIDTH - texX - 1;

		for(int y = drawStart; y < drawEnd; ++y) {
			int d = (y * 256) - (camera->height * 128) + (lineHeight * 128);  //256 and 128 factors to avoid floats
			int texY = ((d * TEXTURE_HEIGHT) / lineHeight) / 256;
			Uint8*pixel = (Uint8*)texture[texNum]->pixels+(((TEXTURE_HEIGHT*texY)+texX)*4);
			//make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
			//if(side == 1) color = (color >> 1) & 8355711;
			SDL_SetRenderDrawColor(renderer, pixel[3], pixel[2], pixel[1], 255);
			SDL_RenderDrawPoint(renderer, x, y);
		}
	}
}

void MoveCamera(SDL_Event*event, float frameTime) {

	//speed modifiers
	float moveSpeed = frameTime * 5.0; //the constant value is in squares/second
	float rotSpeed = frameTime * 3.0; //the constant value is in radians/second
	float oldDirX = camera->directionX;
	float oldPlaneX = camera->planeX;

	switch( event->key.keysym.sym ) {
		case SDLK_UP:
			if(worldMap[(int)(camera->positionX+camera->directionX*moveSpeed)][(int)camera->positionY] == 0) {
				camera->positionX += camera->directionX*moveSpeed;
			}
			if(worldMap[(int)camera->positionX][(int)(camera->positionY+camera->directionY*moveSpeed)] == 0) {
				camera->positionY += camera->directionY*moveSpeed;
			}
			break;
		case SDLK_DOWN:
			if(worldMap[(int)(camera->positionX-camera->directionX*moveSpeed)][(int)camera->positionY] == 0) {
				camera->positionX -= camera->directionX*moveSpeed;
			}
			if(worldMap[(int)camera->positionX][(int)(camera->positionY-camera->directionY*moveSpeed)] == 0) {
				camera->positionY -= camera->directionY*moveSpeed;
			}
			break;
		case SDLK_LEFT:
			 // Both camera direction and camera plane must be rotated
			camera->directionX = camera->directionX * cos(rotSpeed) - camera->directionY * sin(rotSpeed);
			camera->directionY = oldDirX * sin(rotSpeed) + camera->directionY * cos(rotSpeed);
			camera->planeX = camera->planeX * cos(rotSpeed) - camera->planeY * sin(rotSpeed);
			camera->planeY = oldPlaneX * sin(rotSpeed) + camera->planeY * cos(rotSpeed);
			break;
		case SDLK_RIGHT:
			 // Both camera direction and camera plane must be rotated
			camera->directionX = camera->directionX * cos(-rotSpeed) - camera->directionY * sin(-rotSpeed);
			camera->directionY = oldDirX * sin(-rotSpeed) + camera->directionY * cos(-rotSpeed);
			camera->planeX = camera->planeX * cos(-rotSpeed) - camera->planeY * sin(-rotSpeed);
			camera->planeY = oldPlaneX * sin(-rotSpeed) + camera->planeY * cos(-rotSpeed);
			break;
		default:
			break;
	}
}

