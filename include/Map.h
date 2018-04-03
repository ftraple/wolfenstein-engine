#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct w3d_MapSt w3d_Map;

w3d_Map* w3d_CreateMapFromRaw(int width, int height, uint8_t* data);

void w3d_CloseMap(w3d_Map* map);

int w3d_GetMapWidth(w3d_Map* map);

int w3d_GetMapHeight(w3d_Map* map);

uint8_t w3d_GetMapValue(w3d_Map* map, int x, int y);

#endif /* INCLUDE_MAP_H_ */
