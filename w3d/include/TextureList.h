#ifndef W3D_TEXTURE_LIST_H_
#define W3D_TEXTURE_LIST_H_

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.h"

typedef struct w3d_TextureListSt w3d_TextureList;

w3d_TextureList* w3d_TextureList_Create(uint16_t size);

void w3d_TextureList_Destroy(w3d_TextureList* textureList);

uint16_t w3d_TextureList_Pop(w3d_TextureList* textureList, w3d_Texture* texture);

w3d_Texture* w3d_TextureList_GetByIndex(w3d_TextureList* textureList, uint16_t index);

#endif  // W3D_TEXTURE_LIST_H_
