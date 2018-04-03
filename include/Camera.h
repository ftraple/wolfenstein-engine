/*
 * Camera.h
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Map.h"

typedef struct Camera Camera;

Camera*CreateCamera(unsigned int width, unsigned int height);

void DestroyCamera(Camera*camera);

void SetCameraMap(Camera*camera, w3d_Map* map);

void RotateCamera(Camera*camera, int32_t stepX);

void MoveCameraForward(Camera*camera);

void MoveCameraBackward(Camera*camera);

void MoveCameraLeft(Camera*camera);

void MoveCameraRight(Camera*camera);

void RenderCamera(Camera*camera);

const void*GetCameraBuffer(Camera*camera);


#endif /* CAMERA_H_ */
