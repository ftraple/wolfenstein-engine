#ifndef W3D_CAMERA_H_
#define W3D_CAMERA_H_

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Map.h"
#include "Sprite.h"
#include "SpriteList.h"

typedef struct w3d_CameraSt w3d_Camera;

w3d_Camera* w3d_Camera_Create(int width, int height);

void w3d_Camera_Destroy(w3d_Camera* camera);

void w3d_Camera_SetMap(w3d_Camera* camera, w3d_Map* map);

void w3d_Camera_SetSpriteList(w3d_Camera* camera, w3d_SpriteList* spriteList);

void w3d_Camera_Rotate(w3d_Camera* camera, int32_t stepX);

void w3d_Camera_MoveForward(w3d_Camera* camera);

void w3d_Camera_MoveBackward(w3d_Camera* camera);

void w3d_Camera_MoveLeft(w3d_Camera* camera);

void w3d_Camera_MoveRight(w3d_Camera* camera);

void w3d_Camera_Render(w3d_Camera* camera, void* viewBuffer);

#endif  // W3D_CAMERA_H_
