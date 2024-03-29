#ifndef W3D_MAP_H_
#define W3D_MAP_H_

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.h"
#include "TextureList.h"

typedef struct w3d_MapSt w3d_Map;

w3d_Map *w3d_Map_CreateFromRaw(int width, int height, uint8_t *dataFloor, uint8_t *dataWall, uint8_t *dataCeiling, w3d_TextureList *textureList);

void w3d_Map_Close(w3d_Map *map);

int w3d_Map_GetWidth(w3d_Map *map);

int w3d_Map_GetHeight(w3d_Map *map);

uint8_t w3d_Map_GetValue(w3d_Map *map, int x, int y);

w3d_Texture *w3d_Map_GetFloorTexture(w3d_Map *map, int x, int y);

w3d_Texture *w3d_Map_GetWallTexture(w3d_Map *map, int x, int y);

w3d_Texture *w3d_Map_GetCeilingTexture(w3d_Map *map, int x, int y);

#endif  // W3D_MAP_H_
