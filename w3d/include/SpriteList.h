#ifndef W3D_SPRITE_LIST_H_
#define W3D_SPRITE_LIST_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Sprite.h"

typedef struct w3d_SpriteListSt w3d_SpriteList;

w3d_SpriteList* w3d_SpriteList_Create(size_t size);

void w3d_SpriteList_Destroy(w3d_SpriteList* spriteList);

void w3d_SpriteList_Add(w3d_SpriteList* spriteList, w3d_Sprite* sprite);

size_t w3d_SpriteList_GetSize(w3d_SpriteList* spriteList);

w3d_Sprite* w3d_SpriteList_GetSprite(w3d_SpriteList* spriteList, size_t index);

void w3d_SpriteList_SortFromFarToClose(w3d_SpriteList* spriteList, double positionX, double positionY);

#endif  // W3D_SPRITE_LIST_H_
