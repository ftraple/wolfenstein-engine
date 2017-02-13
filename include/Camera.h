/*
 * Camera.h
 *
 *  Created on: 23 de set de 2016
 *      Author: fabiano
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Map.h"

void CreateCamera(unsigned int width, unsigned int height, MapContext*mapContext);

void RotateCameraLeft();

void RotateCameraRight();

void MoveCameraForward();

void MoveCameraBackward();

void MoveCameraLeft();

void MoveCameraRight();

void RenderCamera();

const void*GetCameraBuffer();

void DestroyCamera();

#endif /* CAMERA_H_ */
