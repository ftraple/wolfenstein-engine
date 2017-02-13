/*
 * Camera.c
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Defs.h"
#include "Utils.h"
#include "Camera.h"
#include "Texture.h"


typedef struct {
	float positionX;
	float positionY;
	float directionX;
	float directionY;
	float planeX;
	float planeY;
	int width;
	int height;
	float runSpeed;
	float walkSpeed;
	float moveSpeed;
	float rotationSpeed;
	MapContext*mapContext;
	uint32_t*viewBuffer;
	size_t viewBufferSize;
} CameraContext;

CameraContext*camera = NULL;

void CreateCamera(unsigned int width, unsigned int height, MapContext*mapContext) {

	// Allocate camera context
	camera = (CameraContext*)malloc(sizeof(CameraContext));
	if (camera == NULL) {
		printf("Allocate camera context memory fail!\n");
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
	camera->runSpeed = 0.2;
	camera->walkSpeed = 0.1;
	camera->moveSpeed = 0.1;
	camera->rotationSpeed = 0.03;

	camera->mapContext = mapContext;
	// Allocate camera view buffer
	camera->viewBufferSize = width*height*sizeof(uint32_t);
	camera->viewBuffer = (uint32_t*)malloc(camera->viewBufferSize);
	if (camera == NULL) {
		printf("Allocate camera view buffer fail!\n");
		return;
	}
}

void DestroyCamera() {

	if (camera == NULL) {
		printf("No active camera context.\n");
		return;
	}
	free(camera->viewBuffer);
	free(camera);
}

void RenderCamera() {

	memset(camera->viewBuffer, 128, camera->viewBufferSize);

	float cameraX = 0;
	float rayPosX = camera->positionX;
	float rayPosY = camera->positionY;
	float rayDirX = 0;
	float rayDirY = 0;

	//length of ray from current position to next x or y-side
	float sideDistX;
	float sideDistY;

	float perpWallDist;

	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;
	int side; //was a NS or a EW wall hit?

	uint32_t texturePositionX;
	uint32_t texturePositionY;
	uint32_t*texturePixel;
	uint16_t textureIndex;

	// Draw
	for(int x = 0; x < camera->width; x++) {

		// Calculate ray position and direction
		cameraX = (2*x/(float)camera->width)-1;
		rayDirX = camera->directionX + camera->planeX * cameraX;
		rayDirY = camera->directionY + camera->planeY * cameraX;

		//printf("cameraX = %f   %f   %f\n", cameraX, rayDirX, rayDirY);

		//which box of the map we're in
		int mapX = (int)rayPosX;
		int mapY = (int)rayPosY;

		//---> Step 1 - Calculate the delta distance X and Y.
		float deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
		float deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));

		//---> Step 2 - Calculate the initial step and the sideDist.
		if (rayDirX < 0) {
			stepX = -1;
			sideDistX = (rayPosX - mapX) * deltaDistX;
		}
		else {
			stepX = 1;
			sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
		}
		if (rayDirY < 0) {
			stepY = -1;
			sideDistY = (rayPosY - mapY) * deltaDistY;
		}
		else {
			stepY = 1;
			sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
		}

		//---> Step 3 - Find a wall.
		bool hitWall = FALSE;
		while (hitWall == FALSE) {
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY) {
				sideDistX += deltaDistX;
				mapX += stepX;
				side = 0;
			}
			else {
				sideDistY += deltaDistY;
				mapY += stepY;
				side = 1;
			}
			//Check if ray has hit a wall
			if (GetMapValue(camera->mapContext, mapX, mapY) > 0) {
				hitWall = TRUE;
			}
		}

		// Calculate distance projected on camera direction (oblique distance will give fish eye effect!)
		if (side == 0) {
			perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
		}
		else {
			perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;
		}

		// Get texture surface
		textureIndex = GetMapValue(camera->mapContext, mapX, mapY);
		SDL_Surface*textureSurface = GetTextureSurface(textureIndex);
		if (textureSurface == NULL) {
			printf("textureSurface NULL!\n");
			continue;
		}

		//Calculate height of line to draw on screen
		int lineHeight = camera->height/perpWallDist;

		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + camera->height / 2;
		if(drawStart < 0) {
			drawStart = 0;
		}
		int drawEnd = lineHeight / 2 + camera->height / 2;
		if(drawEnd > camera->height) {
			drawEnd = camera->height;
		}

		// Calculate where exactly the wall was hit.
		double wallHitX;
		if (side == 0) {
			wallHitX = rayPosY + perpWallDist * rayDirY;
		}
		else {
			wallHitX = rayPosX + perpWallDist * rayDirX;
		}
		wallHitX -= floor((wallHitX));

		// Calculate X coordinate on the texture
		texturePositionX = (int)(wallHitX*(double)textureSurface->w);
		if(side == 0 && rayDirX > 0) {
			texturePositionX = textureSurface->w - texturePositionX - 1;
		}
		if(side == 1 && rayDirY < 0) {
			texturePositionX = textureSurface->w - texturePositionX - 1;
		}

		// Draw the wall line
		for(int y = drawStart; y < drawEnd; ++y) {
			// 256 and 128 factors to avoid floats
			uint32_t d = (y * 256) - (camera->height * 128) + (lineHeight * 128);
			texturePositionY = ((d * textureSurface->h) / lineHeight) / 256;
			// Get the pixel color in the texture buffer
			uint32_t textureBufferOffset = (textureSurface->h*texturePositionY)+texturePositionX;
			texturePixel = (uint32_t*)textureSurface->pixels+textureBufferOffset;
			// Set the pixel color in the camera view buffer
			uint32_t viewBufferOffset = (camera->width*y)+x;
			camera->viewBuffer[viewBufferOffset] = *texturePixel;
		}

//		//FLOOR CASTING -------------------------------------------------------------
//		double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall
//
//		//4 different wall directions possible
//		if(side == 0 && rayDirX > 0) {
//			floorXWall = mapX;
//			floorYWall = mapY + wallX;
//		}
//		else if(side == 0 && rayDirX < 0) {
//			floorXWall = mapX + 1.0;
//			floorYWall = mapY + wallX;
//		}
//		else if(side == 1 && rayDirY > 0) {
//			floorXWall = mapX + wallX;
//			floorYWall = mapY;
//		}
//		else {
//			floorXWall = mapX + wallX;
//			floorYWall = mapY + 1.0;
//		}
//
//		double distWall, distPlayer, currentDist;
//
//		distWall = perpWallDist;
//		distPlayer = 0.0;
//
//		if (drawEnd < 0) drawEnd = camera->height; //becomes < 0 when the integer overflows
//
//		SDL_Surface*FloorSurface = GetTextureSurface(4);
//		SDL_Surface*CeilingSurface = GetTextureSurface(7);
//
//		//draw the floor from drawEnd to the bottom of the screen
//		for(int y = drawEnd + 1; y < camera->height; y++) {
//			currentDist = camera->height / (2.0 * y - camera->height); //you could make a small lookup table for this instead
//
//			double weight = (currentDist - distPlayer) / (distWall - distPlayer);
//
//			double currentFloorX = weight * floorXWall + (1.0 - weight) * camera->positionX;
//			double currentFloorY = weight * floorYWall + (1.0 - weight) * camera->positionY;
//
//			int floorTexX;
//			int floorTexY;
//
//			floorTexX = (int)(currentFloorX * FloorSurface->w) % FloorSurface->w;
//			floorTexY = (int)(currentFloorY * FloorSurface->h) % FloorSurface->h;
//
//			// Floor
//			uint32_t*pixel = (uint32_t*)FloorSurface->pixels+(FloorSurface->w*floorTexY)+floorTexX;
//			camera->viewBuffer[(camera->width*y)+x] = *pixel;
//			// Ceiling
//			pixel = (uint32_t*)CeilingSurface->pixels+(CeilingSurface->w*floorTexY)+floorTexX;
//			camera->viewBuffer[(camera->width*(camera->height-y))+x] = *pixel;
//		}
	}
}

const void*GetCameraBuffer() {
	return camera->viewBuffer;
}

void RotateCameraLeft() {
	float rotSpeed = 0.03; //the constant value is in radians/second
	float oldDirX = camera->directionX;
	float oldPlaneX = camera->planeX;
	 // Both camera direction and camera plane must be rotated
	camera->directionX = camera->directionX * cos(rotSpeed) - camera->directionY * sin(rotSpeed);
	camera->directionY = oldDirX * sin(rotSpeed) + camera->directionY * cos(rotSpeed);
	camera->planeX = camera->planeX * cos(rotSpeed) - camera->planeY * sin(rotSpeed);
	camera->planeY = oldPlaneX * sin(rotSpeed) + camera->planeY * cos(rotSpeed);
}

void RotateCameraRight() {
	float rotSpeed = 0.03; //the constant value is in radians/second
	float oldDirX = camera->directionX;
	float oldPlaneX = camera->planeX;
    // Both camera direction and camera plane must be rotated
	camera->directionX = camera->directionX * cos(-rotSpeed) - camera->directionY * sin(-rotSpeed);
	camera->directionY = oldDirX * sin(-rotSpeed) + camera->directionY * cos(-rotSpeed);
	camera->planeX = camera->planeX * cos(-rotSpeed) - camera->planeY * sin(-rotSpeed);
	camera->planeY = oldPlaneX * sin(-rotSpeed) + camera->planeY * cos(-rotSpeed);
}

void MoveCameraForward() {
	if(GetMapValue(camera->mapContext, (int)(camera->positionX+camera->directionX*camera->moveSpeed), (int)camera->positionY) == 0) {
		camera->positionX += camera->directionX*camera->moveSpeed;
	}
	if(GetMapValue(camera->mapContext, (int)camera->positionX, (int)(camera->positionY+camera->directionY*camera->moveSpeed)) == 0) {
		camera->positionY += camera->directionY*camera->moveSpeed;
	}
}

void MoveCameraBackward() {
	float moveSpeed = camera->walkSpeed;
	if(GetMapValue(camera->mapContext, (int)(camera->positionX-camera->directionX*moveSpeed), (int)camera->positionY) == 0) {
		camera->positionX -= camera->directionX*moveSpeed;
	}
	if(GetMapValue(camera->mapContext, (int)camera->positionX, (int)(camera->positionY-camera->directionY*moveSpeed)) == 0) {
		camera->positionY -= camera->directionY*moveSpeed;
	}
}

void MoveCameraLeft() {
	float rotation = 80.0;
	float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
	float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

	if(GetMapValue(camera->mapContext, (int)(camera->positionX-directionX*camera->moveSpeed), (int)camera->positionY) == 0) {
		camera->positionX -= directionX*camera->moveSpeed;
	}
	if(GetMapValue(camera->mapContext, (int)camera->positionX, (int)(camera->positionY-directionY*camera->moveSpeed)) == 0) {
		camera->positionY -= directionY*camera->moveSpeed;
	}
}

void MoveCameraRight() {
	double rotation = 80.0;
	float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
	float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

	if(GetMapValue(camera->mapContext, (int)(camera->positionX+directionX*camera->moveSpeed), (int)camera->positionY) == 0) {
		camera->positionX += directionX*camera->moveSpeed;
	}
	if(GetMapValue(camera->mapContext, (int)camera->positionX, (int)(camera->positionY+directionY*camera->moveSpeed)) == 0) {
		camera->positionY += directionY*camera->moveSpeed;
	}
}


