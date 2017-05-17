/*
 * Map.h
 *
 *  Created on: 3 de fev de 2017
 *      Author: fabiano
 */

#ifndef INCLUDE_MAP_H_
#define INCLUDE_MAP_H_

#include <stdint.h>

#include "Sprite.h"

struct Map;
typedef struct Map Map;

Map*OpenMap(const char*filename);

void CloseMap(Map*map);

uint16_t GetMapWidth(Map*map);

uint16_t GetMapHeight(Map*map);

uint16_t GetMapValue(Map*map, uint16_t x, uint16_t y);

void Map_AddSprite(Sprite*sprite);

void Map_RemoveSprite(Sprite*sprite);


#endif /* INCLUDE_MAP_H_ */
