/*
 * Camera.h
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <SDL2/SDL.h>

void CreateCamera(unsigned int width, unsigned int height);

void MoveCamera(SDL_Event*event, float frameTime);

void RenderCamera(SDL_Renderer*renderer);

void DestroyCamera();

#endif /* CAMERA_H_ */
