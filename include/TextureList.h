#ifndef INCLUDE_TEXTURE_LIST_H
#define INCLUDE_TEXTURE_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "Texture.h"

typedef struct w3d_TextureListSt w3d_TextureList;

w3d_TextureList* w3d_TextureList_Create(uint16_t size);

void w3d_TextureList_Destroy(w3d_TextureList* textureList);

uint16_t w3d_TextureList_Pop(w3d_TextureList* textureList, w3d_Texture* texture);

w3d_Texture* w3d_TextureList_GetByIndex(w3d_TextureList* textureList, uint16_t index);

#endif /* INCLUDE_TEXTURE_LIST_H */
