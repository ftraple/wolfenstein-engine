/*
 * Camera.h
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>

#include "Map.h"

typedef struct w3d_CameraSt w3d_Camera;

w3d_Camera* w3d_CreateCamera(int width, int height);

void w3d_DestroyCamera(w3d_Camera* camera);

void w3d_SetCameraMap(w3d_Camera* camera, w3d_Map* map);

void w3d_RotateCamera(w3d_Camera* camera, int32_t stepX);

void w3d_MoveCameraForward(w3d_Camera* camera);

void w3d_MoveCameraBackward(w3d_Camera* camera);

void w3d_MoveCameraLeft(w3d_Camera* camera);

void w3d_MoveCameraRight(w3d_Camera* camera);

void w3d_RenderCamera(w3d_Camera* camera, void* viewBuffer);


#endif /* CAMERA_H_ */
