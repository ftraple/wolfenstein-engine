
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include "Defs.h"
#include "Utils.h"
#include "Camera.h"
#include "Texture.h"
#include "Map.h"

typedef struct FrameData {
	int32_t mapX;
	int32_t mapY;

	int32_t wallStart;
	int32_t wallEnd;

	uint8_t side; // Was a NS or a EW wall hit?
	float wallHitX;
	float perpWallDist;

	float rayDirectionX;
	float rayDirectionY;
} FrameData;

struct Camera {
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
	float wallDistance;
	w3d_Map* map;
	uint32_t*viewBuffer;
	size_t viewBufferSize;

	FrameData*frameData;
};


//#define NUM_SPRITES 19
//#define TEXT_WIDTH 64
//#define TEXT_HEIGHT 64

//typedef struct
//{
//  float x;
//  float y;
//  int texture;
//} Sprite;
//
//Sprite sprite[NUM_SPRITES] =
//{
//  {20.5, 11.5, 10}, //green light in front of player start
//  //green lights in every room
//  {18.5,4.5, 10},
//  {10.0,4.5, 10},
//  {10.0,12.5,10},
//  {3.5, 6.5, 10},
//  {3.5, 20.5,10},
//  {3.5, 14.5,10},
//  {14.5,20.5,10},
//
//  //row of pillars in front of wall: fisheye test
//  {18.5, 10.5, 9},
//  {18.5, 11.5, 9},
//  {18.5, 12.5, 9},
//
//  //some barrels around the map
//  {21.5, 1.5, 8},
//  {15.5, 1.5, 8},
//  {16.0, 1.8, 8},
//  {16.2, 1.2, 8},
//  {3.5,  2.5, 8},
//  {9.5, 15.5, 8},
//  {10.0, 15.1,8},
//  {10.5, 15.8,8},
//};

// ### HARD CODE
float ZBuffer[640];
int spriteOrder[100];
float spriteDistance[100];



void FloorRender();
//void SpriteRender();

//sort algorithm
void combSort(int*order, float*dist, int amount)
{
	int gap = amount;
	bool swapped = false;
	while (gap > 1 || swapped) {
		//shrink factor 1.3
		gap = (gap * 10) / 13;
		if (gap == 9 || gap == 10) {
			gap = 11;
		}
		if (gap < 1) {
			gap = 1;
		}
		swapped = false;
		for (int i = 0; i < amount - gap; i++) {
			int j = i + gap;
			if (dist[i] < dist[j]) {
				// Swap distance
				float distTmp = dist[j];
				dist[j] = dist[i];
				dist[i] = distTmp;
				// Swap order
				int orderTmp = order[j];
				order[j] = order[i];
				order[i] = orderTmp;
				swapped = true;
			}
		}
	}
}


Camera*CreateCamera(unsigned int width, unsigned int height) {

	// Allocate camera context
	Camera*camera = (Camera*)malloc(sizeof(Camera));
	if (camera == NULL) {
		printf("Allocate camera context memory fail!\n");
		return NULL;
	}

	// Set default values to the camera context
	camera->positionX = 5;
	camera->positionY = 5;
	camera->directionX = -1;
	camera->directionY = 0;
	camera->planeX = 0;
	camera->planeY = 0.60;
	camera->width = width;
	camera->height = height;
	camera->runSpeed = 0.1;
	camera->walkSpeed = 0.05;
	camera->moveSpeed = camera->walkSpeed;
	camera->rotationSpeed = 0.001;
	camera->wallDistance = 0.3;

	// Allocate camera view buffer
	camera->viewBufferSize = width*height*sizeof(uint32_t);
	camera->viewBuffer = (uint32_t*)malloc(camera->viewBufferSize);
	if (camera->viewBuffer == NULL) {
		printf("Allocate camera view buffer fail!\n");
		return NULL;
	}
	memset(camera->viewBuffer, 0, camera->viewBufferSize);

	camera->frameData = (FrameData*)malloc(camera->width*sizeof(FrameData));
	if (camera->frameData == NULL) {
		printf("Allocate camera frame data buffer fail!\n");
		return NULL;
	}

	return camera;
}

void DestroyCamera(Camera*camera) {

	if (camera == NULL) {
		printf("No active camera context.\n");
		return;
	}
	free(camera->frameData);
	free(camera->viewBuffer);
	free(camera);
}

void SetCameraMap(Camera*camera, w3d_Map* map) {
	camera->map = map;
}


void RenderCamera(Camera*camera) {

	if (camera->map == NULL) {
		return;
	}
	memset(camera->viewBuffer, 128, camera->viewBufferSize);

	float cameraX;
	float rayPositionX = camera->positionX;
	float rayPositionY = camera->positionY;

	// Length of ray from current position to next x or y-side
	float sideDistanceX;
	float sideDistanceY;

	//what direction to step in x or y-direction (either +1 or -1)
	int stepX;
	int stepY;

	uint32_t texturePositionX;
	uint32_t texturePositionY;
	uint32_t*texturePixel;
	uint16_t textureIndex;

	FrameData*frameData = camera->frameData;

	// Draw
	for(int x = 0; x < camera->width; x++) {

		// Step - Calculate ray position and direction
		cameraX = (2*x/(float)camera->width)-1;
		frameData->rayDirectionX = camera->directionX+(camera->planeX*cameraX);
		frameData->rayDirectionY = camera->directionY+(camera->planeY*cameraX);

		//which box of the map we're in
		frameData->mapX = (int32_t)rayPositionX;
		frameData->mapY = (int32_t)rayPositionY;

		//---> Step - Calculate the delta distance X and Y.
		float deltaDistX = sqrt(1+(frameData->rayDirectionY*frameData->rayDirectionY)/(frameData->rayDirectionX*frameData->rayDirectionX));
		float deltaDistY = sqrt(1+(frameData->rayDirectionX*frameData->rayDirectionX)/(frameData->rayDirectionY*frameData->rayDirectionY));

		//---> Step 2 - Calculate the initial step and the sideDistance.
		if (frameData->rayDirectionX < 0) {
			stepX = -1;
			sideDistanceX = (rayPositionX-frameData->mapX)*deltaDistX;
		}
		else {
			stepX = 1;
			sideDistanceX = (frameData->mapX+1.0-rayPositionX)*deltaDistX;
		}
		if (frameData->rayDirectionY < 0) {
			stepY = -1;
			sideDistanceY = (rayPositionY-frameData->mapY)*deltaDistY;
		}
		else {
			stepY = 1;
			sideDistanceY = (frameData->mapY+1.0-rayPositionY)*deltaDistY;
		}

		//---> Step 3 - Find a wall.
		bool hitWall = false;
		while (hitWall == false) {
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistanceX < sideDistanceY) {
				sideDistanceX += deltaDistX;
				frameData->mapX += stepX;
				frameData->side = 0;
			}
			else {
				sideDistanceY += deltaDistY;
				frameData->mapY += stepY;
				frameData->side = 1;
			}
			//Check if ray has hit a wall
			if (w3d_GetMapValue(camera->map, frameData->mapX, frameData->mapY) > 0) {
				hitWall = true;
			}
		}

		// Calculate distance projected on camera direction (oblique distance will give fish eye effect!)
		if (frameData->side == 0) {
			frameData->perpWallDist = (frameData->mapX-rayPositionX+(1-stepX)/2)/frameData->rayDirectionX;
		}
		else {
			frameData->perpWallDist = (frameData->mapY-rayPositionY+(1-stepY)/2)/frameData->rayDirectionY;
		}

		// Get texture surface
		textureIndex = w3d_GetMapValue(camera->map, frameData->mapX, frameData->mapY)-1;
		SDL_Surface*textureSurface = GetTextureSurface(textureIndex);
		if (textureSurface == NULL) {
			printf("textureSurface NULL!\n");
			continue;
		}

		// Calculate height of line to draw on screen
		int lineHeight = camera->height/frameData->perpWallDist;

		// calculate lowest and highest pixel to fill in current stripe
		frameData->wallStart = (-lineHeight/2)+(camera->height/2);
		if(frameData->wallStart < 0) {
			frameData->wallStart = 0;
		}
		frameData->wallEnd = (lineHeight/2)+(camera->height/2);
		if(frameData->wallEnd > camera->height) {
			frameData->wallEnd = camera->height;
		}

		// Calculate where exactly the wall was hit.
		if (frameData->side == 0) {
			frameData->wallHitX = rayPositionY + frameData->perpWallDist * frameData->rayDirectionY;
		}
		else {
			frameData->wallHitX = rayPositionX + frameData->perpWallDist * frameData->rayDirectionX;
		}
		frameData->wallHitX -= floor(frameData->wallHitX);

		// Calculate X coordinate on the texture
		texturePositionX = (int)(frameData->wallHitX*(double)textureSurface->w);
		if(frameData->side == 0 && frameData->rayDirectionX > 0) {
			texturePositionX = textureSurface->w - texturePositionX - 1;
		}
		if(frameData->side == 1 && frameData->rayDirectionY < 0) {
			texturePositionX = textureSurface->w - texturePositionX - 1;
		}

		// Draw the wall line
		for (int y = frameData->wallStart; y < frameData->wallEnd; ++y) {
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

		//SET THE ZBUFFER FOR THE SPRITE CASTING
        ZBuffer[x] = frameData->perpWallDist; //perpendicular distance is used

        frameData++;
	}

	FloorRender(camera);
	//SpriteRender(camera);
}


void FloorRender(Camera*camera) {

	SDL_Surface*FloorSurface = GetTextureSurface(3);
	SDL_Surface*CeilingSurface = GetTextureSurface(6);

	FrameData*frameData = camera->frameData;

	for(int x = 0; x < camera->width; x++) {

		//FLOOR CASTING -------------------------------------------------------------
		double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

		//4 different wall directions possible
		if(frameData->side == 0 && frameData->rayDirectionX > 0) {
			floorXWall = frameData->mapX;
			floorYWall = frameData->mapY + frameData->wallHitX;
		}
		else if(frameData->side == 0 && frameData->rayDirectionX < 0) {
			floorXWall = frameData->mapX + 1.0;
			floorYWall = frameData->mapY + frameData->wallHitX;
		}
		else if(frameData->side == 1 && frameData->rayDirectionY > 0) {
			floorXWall = frameData->mapX + frameData->wallHitX;
			floorYWall = frameData->mapY;
		}
		else {
			floorXWall = frameData->mapX + frameData->wallHitX;
			floorYWall = frameData->mapY + 1.0;
		}

		double distWall, distPlayer, currentDist;

		distWall = frameData->perpWallDist;
		distPlayer = 0.0;

		if (frameData->wallEnd < 0) {
			frameData->wallEnd = camera->height; //becomes < 0 when the integer overflows
		}

		//draw the floor from wallEnd to the bottom of the screen
		for(int y = frameData->wallEnd + 1; y < camera->height; y++) {
			currentDist = camera->height / (2.0 * y - camera->height); //you could make a small lookup table for this instead

			double weight = (currentDist - distPlayer) / (distWall - distPlayer);

			double currentFloorX = weight * floorXWall + (1.0 - weight) * camera->positionX;
			double currentFloorY = weight * floorYWall + (1.0 - weight) * camera->positionY;

			int floorTexX;
			int floorTexY;

			floorTexX = (int)(currentFloorX * FloorSurface->w) % FloorSurface->w;
			floorTexY = (int)(currentFloorY * FloorSurface->h) % FloorSurface->h;

			// Floor
			uint32_t*pixel = (uint32_t*)FloorSurface->pixels+(FloorSurface->w*floorTexY)+floorTexX;
			camera->viewBuffer[(camera->width*y)+x] = *pixel;
			// Ceiling
			pixel = (uint32_t*)CeilingSurface->pixels+(CeilingSurface->w*floorTexY)+floorTexX;
			camera->viewBuffer[(camera->width*(camera->height-y))+x] = *pixel;
		}
		frameData++;
	}
}

//void SpriteRender(Camera*camera) {
//
//	uint32_t spriteAmount = GetSpriteAmount();
//
//
//	// Sort sprites from far to close
//	for(int i = 0; i < spriteAmount; i++)
//	{
//		spriteOrder[i] = i;
//		spriteDistance[i] = ((camera->positionX - sprite[i].x) * (camera->positionX - sprite[i].x) + (camera->positionY - sprite[i].y) * (camera->positionY - sprite[i].y)); //sqrt not taken, unneeded
//	}
//	combSort(spriteOrder, spriteDistance, spriteAmount);
//
//	//after sorting the sprites, do the projection and draw them
//	for(int i = 0; i < spriteAmount; i++) {
//
//		SDL_Surface*spriteSurface = GetTextureSurface(sprite[spriteOrder[i]].texture);
//
//		//translate sprite position to relative to camera
//		float spriteX = sprite[spriteOrder[i]].x - camera->positionX;
//		float spriteY = sprite[spriteOrder[i]].y - camera->positionY;
//
//		//transform sprite with the inverse camera matrix
//		// [ planeX   dirX ] -1                                       [ dirY      -dirX ]
//		// [               ]       =  1/(planeX*dirY-dirX*planeY) *   [                 ]
//		// [ planeY   dirY ]                                          [ -planeY  planeX ]
//
//		float invDet = 1.0 / (camera->planeX * camera->directionY - camera->directionX * camera->planeY); //required for correct matrix multiplication
//
//		float transformX = invDet * (camera->directionY * spriteX - camera->directionX * spriteY);
//		float transformY = invDet * (-camera->planeY * spriteX + camera->planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D
//
//		int spriteScreenX = (int)((camera->width / 2) * (1 + transformX / transformY));
//
//		//calculate height of the sprite on screen
//		int spriteHeight = abs((int)(camera->height/(transformY))); //using "transformY" instead of the real distance prevents fisheye
//		//calculate lowest and highest pixel to fill in current stripe
//		int wallStartY = -spriteHeight / 2 + camera->height / 2;
//		if(wallStartY < 0) wallStartY = 0;
//		int wallEndY = spriteHeight / 2 + camera->height / 2;
//		if(wallEndY >= camera->height) wallEndY = camera->height - 1;
//
//		//calculate width of the sprite
//		int spriteWidth = abs( (int)(camera->height/(transformY)));
//		int wallStartX = -spriteWidth / 2 + spriteScreenX;
//		if(wallStartX < 0) wallStartX = 0;
//		int wallEndX = spriteWidth / 2 + spriteScreenX;
//		if(wallEndX >= camera->width) wallEndX = camera->width - 1;
//
//		//loop through every vertical stripe of the sprite on screen
//		for(int stripe = wallStartX; stripe < wallEndX; stripe++) {
//			int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * TEXT_WIDTH / spriteWidth) / 256;
//			//the conditions in the if are:
//			//1) it's in front of camera plane so you don't see things behind you
//			//2) it's on the screen (left)
//			//3) it's on the screen (right)
//			//4) ZBuffer, with perpendicular distance
//
//			if(transformY > 0 && stripe > 0 && stripe < camera->width && transformY < ZBuffer[stripe]) {
//				for(int y = wallStartY; y < wallEndY; y++) {
//					int d = (y) * 256 - camera->height * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
//					int texY = ((d * TEXT_HEIGHT) / spriteHeight) / 256;
//
//					uint32_t*pixelColor = (uint32_t*)spriteSurface->pixels+(spriteSurface->w*texY)+texX;
//					if ((*pixelColor & 0xFFFFFF00) != 0) { //paint pixel if it isn't black, black is the invisible color
//						camera->viewBuffer[(camera->width*y)+stripe] = *pixelColor;
//					}
//				}
//			}
//		}
//	}
//}

const void*GetCameraBuffer(Camera*camera) {
	return camera->viewBuffer;
}

void RotateCamera(Camera*camera, int32_t stepX) {
	float rotSpeed = (float)stepX*camera->rotationSpeed;
	float oldDirX = camera->directionX;
	float oldPlaneX = camera->planeX;
	 // Both camera direction and camera plane must be rotated
	camera->directionX = camera->directionX * cos(rotSpeed) - camera->directionY * sin(rotSpeed);
	camera->directionY = oldDirX * sin(rotSpeed) + camera->directionY * cos(rotSpeed);
	camera->planeX = camera->planeX * cos(rotSpeed) - camera->planeY * sin(rotSpeed);
	camera->planeY = oldPlaneX * sin(rotSpeed) + camera->planeY * cos(rotSpeed);
}

void MoveCameraForward(Camera*camera) {
	if(w3d_GetMapValue(camera->map, (int)(camera->positionX+camera->directionX*(camera->moveSpeed+camera->wallDistance)), (int)camera->positionY) == 0) {
		camera->positionX += camera->directionX*camera->moveSpeed;
	}
	if(w3d_GetMapValue(camera->map, (int)camera->positionX, (int)(camera->positionY+camera->directionY*(camera->moveSpeed+camera->wallDistance))) == 0) {
		camera->positionY += camera->directionY*camera->moveSpeed;
	}
}

void MoveCameraBackward(Camera*camera) {
	if(w3d_GetMapValue(camera->map, (int)(camera->positionX-camera->directionX*(camera->moveSpeed+camera->wallDistance)), (int)camera->positionY) == 0) {
		camera->positionX -= camera->directionX*camera->moveSpeed;
	}
	if(w3d_GetMapValue(camera->map, (int)camera->positionX, (int)(camera->positionY-camera->directionY*(camera->moveSpeed+camera->wallDistance))) == 0) {
		camera->positionY -= camera->directionY*camera->moveSpeed;
	}
}

void MoveCameraLeft(Camera*camera) {
	float rotation = 80.0;
	float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
	float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

	if(w3d_GetMapValue(camera->map, (int)(camera->positionX-directionX*(camera->moveSpeed+camera->wallDistance)), (int)camera->positionY) == 0) {
		camera->positionX -= directionX*camera->moveSpeed;
	}
	if(w3d_GetMapValue(camera->map, (int)camera->positionX, (int)(camera->positionY-directionY*(camera->moveSpeed+camera->wallDistance))) == 0) {
		camera->positionY -= directionY*camera->moveSpeed;
	}
}

void MoveCameraRight(Camera*camera) {
	double rotation = 80.0;
	float directionX = camera->directionX * cos(rotation) - camera->directionY * sin(rotation);
	float directionY = camera->directionX * sin(rotation) + camera->directionY * cos(rotation);

	if(w3d_GetMapValue(camera->map, (int)(camera->positionX+directionX*(camera->moveSpeed+camera->wallDistance)), (int)camera->positionY) == 0) {
		camera->positionX += directionX*camera->moveSpeed;
	}
	if(w3d_GetMapValue(camera->map, (int)camera->positionX, (int)(camera->positionY+directionY*(camera->moveSpeed+camera->wallDistance))) == 0) {
		camera->positionY += directionY*camera->moveSpeed;
	}
}


